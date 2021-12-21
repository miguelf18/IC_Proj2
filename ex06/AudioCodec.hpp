#include "Golomb.h"
#include "AudioFile.h"

using namespace std;

// class interface to compress/decompress wav audio files
class AudioCodec
{

    private:

        string file_name;
        // 0 for decoding and 1 for encoding
        unsigned char file_mode;
        // parameter for golomb 
        int m;
        Golomb* golomb;

        // computes entropy between original data and residual and stores it on given memory address
        void compute_entropy(map<int, int>* original_map, map<int, int>* residual_map, double* original_entropy, double* residual_entropy, int* count);
        // display histogram and store it in file
        void show_histogram(map<int, int>* original_map, map<int, int>* residual_map);

    public:

        // constructor
        AudioCodec(string file_name, string file_mode, int m);
        // compress the audio file
        bool Compress();
        // decompress the given audio file
        bool Decompress();

};