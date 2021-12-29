#include "bitStream.hpp"

#ifndef GOLOMB_H
#define GOLOMB_H

class Golomb
{
    public:
        Golomb(int _m, string file_name, unsigned char file_mode);
        Golomb(string file_name, unsigned char file_mode);

        void setValues(int _m);
        int checkValues(int m);

        int getm() { return m; }

        int cwArrLength;

        void encode(int* value);
        int decode();
        bool close();

    private:
        int m;
        //int binArrLength, unArrLength;
        int * decimalToBinary(int n);
        int * decimalToUnary(int n);
        //int * codeword(int* u, int uLength, int* b, int bLength);
        int binaryToDecimal(int* remainder);
        // bit stream
        BitStream* bit_stream;
        string file_name;
        // 1 for encoding; 0 for decoding
        unsigned char file_mode;
};

#endif // GOLOMB_H