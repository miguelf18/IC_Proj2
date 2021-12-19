#ifndef GOLOMB_H
#define GOLOMB_H

class Golomb
{
    public:
        Golomb(int _i, int _m);
        Golomb(int _m);
        Golomb();

        void setValues(int _i,int _m);
        int checkValues(int i,int m);

        int geti() { return i; }
        int getm() { return m; }

        int cwArrLength;

        int * encode();
        int decode(int* c);

    private:
        int i, m;
        int binArrLength, unArrLength;
        int * decimalToBinary(int n, int length = -1);
        int * decimalToUnary(int n);
        int * codeword(int* u, int uLength, int* b, int bLength);
        int binaryToDecimal(int* bin);
};

#endif // GOLOMB_H