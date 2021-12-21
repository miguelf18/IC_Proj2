#include "AudioCodec.hpp"

int main(int argc, char **argv)
{
    // compress audio file
    AudioCodec codec("sample01.wav", "out", 15);
    codec.Compress();
    // decompress encoded audio file
    AudioCodec codec2("encoded_audio.bin", "in", 15);
    codec2.Decompress();
    return 0;
}