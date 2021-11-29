#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// class interface to read/write bits to/from file
class BitStream
{

    private:

        // bit buffer, stores 8 bits or 1 byte of data
        struct BitBuffer
        {
            const int size = 8;
            int count = 0;
            unsigned char buffer = 0;
        };


        BitBuffer bit_buffer;
        ifstream input_file;
        ofstream output_file;
        // 0 for input, 1 for output
        unsigned char file_mode;

    public:

        // constructor
        BitStream(string file_name, string file_mode);
        // returns the current buffer count size
        unsigned char size();
        // writes bit to buffer
        // bits are written from most significant to least significant bit
        // if bits exceed buffer size, it will only contain the last buffer size bits
        void write_bit(unsigned char bit);
        // writes n bits to buffer
        // bits are written from most significant to least significant bit
        // if bits exceed buffer size, it will only contain the last buffer size bits
        void write_n_bits(unsigned char *bits, int bits_size);
        // writes 8 bits to open file and resets buffer
        // returns 1 if there was an error with the file, or 0 otherwise
        bool flush_file();
        // reads 1 bit from buffer and stores it in a given memory address
        // bits are read from least significant to most significant bit 
        // if buffer is empty, nothing happens
        void read_bit(unsigned char *bit);
        // reads n bits from buffer and stores it in a given memory address
        // bits are read from least significant to most significant bit 
        // if buffer is empty, nothing happens
        void read_n_bits(unsigned char *bits, int bits_size);
        // reads file and fills buffer with 1 byte of data, overwriting any existing buffer content
        // returns 1 if there was an error with the file, or 0 otherwise
        bool read_file();
        // try to close the file 
        // returns 1 if there was an error with the file, or 0 otherwise
        bool close();

};