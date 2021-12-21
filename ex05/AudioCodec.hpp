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


    public:

        // constructor
        AudioCodec(string file_name, string file_mode, int m);
        // compress the audio file
        bool Compress();
        // decompress the given audio file
        bool Decompress();

};