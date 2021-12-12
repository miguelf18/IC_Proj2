#ifndef GOLOMB_H
#define GOLOMB_H

class Golomb
{
    private:
        // int i;
        // int m;

    public:
        Golomb(/*int i, int m*/);

        // int geti() { return i; }
        // int getm() { return m; }

        int binArrLength;
        int * decimalToBinary(int n, int length = -1);
        int * decimalToUnary(int n);
        int * codeword(int* u, int uLength, int* b, int bLength);
        int * encoder(int i, int m);
};

#endif // GOLOMB_H