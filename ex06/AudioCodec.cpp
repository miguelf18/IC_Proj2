#include "AudioCodec.hpp"
#include "AudioFile.h"

// constructor
AudioCodec::AudioCodec(string file_name, string file_mode, int m)
{
    this->file_name = file_name;
    this->m = m;
    if(file_mode == "out")
    {
        this->file_mode = 1;
    }
    else
    {
        this->file_mode = 0;
    }
}

// display histogram
void AudioCodec::show_histogram(map<int, int>* original_map, map<int, int>* residual_map)
{
    // display original data histogram
    ofstream output_original("original_data.txt");
    cout << "Displaying original data: " << endl;
    for(auto item : *original_map)
    {
        cout << item.first << ": ";
        cout << item.second << endl;
        output_original << item.first << ": ";
        output_original << item.second << endl;
    }
    output_original.close();
    // display residual data histogram
    ofstream output_residual("residual_data.txt");
    cout << "Displaying residual data: " << endl;
    for(auto item : *residual_map)
    {
        cout << item.first << ": ";
        cout << item.second << endl;
        output_residual << item.first << ": ";
        output_residual << item.second << endl;
    }
    output_residual.close();
    cout << "Original data saved at >> original_data.txt" << endl;
    cout << "Residual data saved at >> residual_data.txt" << endl;
}

// get total entropy
void AudioCodec::compute_entropy(map<int, int>* original_map, map<int, int>* residual_map, double* original_entropy, double* residual_entropy, int* count)
{
    // compute original data entropy
    for(auto item : *original_map)
    {
        int item_value = item.second;
        double probability = static_cast<double>(item_value) / (*count);
        (*original_entropy) -= probability * log2(probability);
    }
    // compute residual data entropy
    for(auto item : *residual_map)
    {
        int item_value = item.second;
        double probability = static_cast<double>(item_value) / (*count);
        (*residual_entropy) -= probability * log2(probability);
    }
}

// compress the audio file
bool AudioCodec::Compress()
{
    cout << "Compressing...." << endl;
    if(!file_mode)
        return 1;
    // read wav file
    AudioFile<float> input_audio(file_name);
    int channels = input_audio.getNumChannels();
    int samples_per_channel = input_audio.getNumSamplesPerChannel();
    int sample_rate = input_audio.getSampleRate();
    golomb = new Golomb(m, "encoded_audio.bin", file_mode);
    // encode golomb parameter
    golomb->encode(&m);
    // encode some audio metadata
    golomb->encode(&channels);
    golomb->encode(&samples_per_channel);
    golomb->encode(&sample_rate);
    // keeping track of previous sample values for predictors
    int last_samples[3] = {0, 0, 0};
    map<int, int> residual_frequency;
    map<int, int> original_frequency;
    // encode audio samples
    // assume wav audio with 2 channels and 16 bits data
    for(int i = 0; i < samples_per_channel; i++)
    {
        // golomb only works with integers
        // convert float to int (8 bits)
        int left = input_audio.samples[0][i] * 255;
        int right = input_audio.samples[1][i] * 255;
        // channel redundancy
        int merged_sample = floor((left + right) / 2.0);
        // apply polynomial predictors (temporal redundancy)
        int estimate = 0;
        int residual = 0;
        if(i == 0)
        {
            // first order predictor
            estimate = 0;
            last_samples[0] = merged_sample;
        }
        else if(i == 1)
        {
            // second order predictor
            estimate = last_samples[0];
            last_samples[1] = merged_sample;
        }
        else if(i == 2)
        {
            // third order predictor
            estimate = 2 * last_samples[1] - last_samples[0];
            last_samples[2] = merged_sample;
        }
        else
        {
            // fourth order predictor
            estimate = 3 * last_samples[2] - 3 * last_samples[1] + last_samples[0];
            last_samples[0] = last_samples[1];
            last_samples[1] = last_samples[2];
            last_samples[2] = merged_sample;
        }
        // compute residuals
        residual = merged_sample - estimate;
        original_frequency[merged_sample]++;
        residual_frequency[residual]++;
        // encode residuals
        golomb->encode(&residual);
    }
    golomb->close();
    int count = samples_per_channel;
    double original_entropy = 0;
    double residual_entropy = 0;
    compute_entropy(&original_frequency, &residual_frequency, &original_entropy, &residual_entropy, &count);
    show_histogram(&original_frequency, &residual_frequency);
    cout << "Original entropy >> " << original_entropy << endl;
    cout << "Residual entropy >> " << residual_entropy << endl;
    cout << "Compressing finished with no errors" << endl;
    return 0;
}

// decompress the given audio file
bool AudioCodec::Decompress()
{
    cout << "Decompressing...." << endl;
    if(file_mode)
        return 1;
    AudioFile<float> output_audio;
    bool error = 0;
    int value = 0;
    golomb = new Golomb(m, file_name, file_mode);
    // decode golomb parameter
    error = golomb->decode(&value);
    int decoded_m = value;
    if(m != decoded_m)
        return 1;
    // decode file metadata
    error = golomb->decode(&value);
    int channels = value;
    error = golomb->decode(&value);
    int samples_per_channel = value;
    error = golomb->decode(&value);
    int sample_rate = value;
    if(error)
        return 1;
    output_audio.setNumChannels(channels);
    output_audio.setNumSamplesPerChannel(samples_per_channel);
    output_audio.setSampleRate(sample_rate);
    // keeping track of previous sample values for predictors
    int last_samples[3] = {0, 0, 0};
    // decode audio samples 
    // assume wav audio with 2 channels and 16 bits data
    for(int i = 0; i < samples_per_channel; i++)
    {
        // decode next residual
        error = golomb->decode(&value);
        if(error)
            continue;
        int residual = value;
        int estimate = 0;
        int merged_sample = 0;
        if(i == 0)
        {
            // first order predictor
            estimate = 0;
            merged_sample = residual + estimate;
            last_samples[0] = merged_sample;
        }
        else if(i == 1)
        {
            // second order predictor
            estimate = last_samples[0];
            merged_sample = residual + estimate;
            last_samples[1] = merged_sample;
        }
        else if(i == 2)
        {
            // third order predictor
            estimate = 2 * last_samples[1] - last_samples[0];
            merged_sample = residual + estimate;
            last_samples[2] = merged_sample;
        }
        else
        {
            // fourth order predictor
            estimate = 3 * last_samples[2] - 3 * last_samples[1] + last_samples[0];
            merged_sample = residual + estimate;
            last_samples[0] = last_samples[1];
            last_samples[1] = last_samples[2];
            last_samples[2] = merged_sample;
        }
        // channel redundancy
        output_audio.samples[0][i] = merged_sample / 255.0f;
        output_audio.samples[1][i] = merged_sample / 255.0f;
    }
    output_audio.save("output.wav");
    cout << "Decompressing finished with no errors" << endl;
    return 0;
}

