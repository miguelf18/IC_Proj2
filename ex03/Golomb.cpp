#include "Golomb.h"
#include <iostream>
#include <cmath>

using namespace std;

// constructor
Golomb::Golomb(/*int i, int m*/)
{
    // i = i;
    // m = m;
}

// functions
int binArrLength;

int * Golomb::decimalToBinary(int n, int length)
{
    int* inv = new int[32];

    binArrLength = 0;
    while(n>0) {
        inv[binArrLength] = n % 2;
        n = n / 2;
        binArrLength++;
    }

    int* res = new int[binArrLength];
    int i = 0;
    for(int j=binArrLength-1; j>=0; j--) {
        res[i] = inv[j];
        i++;
    }

    if(length != -1) {
        if(binArrLength < length) {
            int *tmp = new int[length];
            int dif = length - binArrLength;
            cout << "dif = " << dif << endl;
            // meter 'dif' zeros antes do numero binário
            for(int i=0; i<dif; i++) {
                tmp[i] = 0;
            }
            for(int i=0; i<length; i++) {
                tmp[dif + i] = res[i];
            }
            res = tmp;
            binArrLength = length;
        }
    }
    

    return res;
}

int * Golomb::decimalToUnary(int n) {

    int* res = new int[n+1];

    for(int i=0; i<n; i++) {
        res[i] = 1;
    }
    res[n] = 0;

    return res;
}

int * Golomb::codeword(int* u, int uLength, int* b, int bLength) {

    int *cw = new int[uLength + bLength];

    for(int i=0; i<uLength; i++) {
        cw[i] = u[i];
    }
    for(int i=0; i<bLength; i++) {
        cw[uLength+i] = b[i];
    }

    cout << "codeword: ";
    for(int i=0; i<uLength+bLength; i++) {
        cout << cw[i];
    }
    cout << endl;

    return cw;
}

int * Golomb::encoder(int i, int m) {

    int q, r;

    q = floor(i / m); // quociente -> unario
    r = i - (q * m);  // resto -> binario
    cout << "q: " << q << endl;
    cout << "r: " << r << endl;

    int *qUnary = decimalToUnary(q);

    if (ceil(log2(m)) == floor(log2(m))) // m é potencia de 2
    {
        int *rBinary = decimalToBinary(r);
        return codeword(qUnary, q+1, rBinary, binArrLength);
    }
    else // m não é potencia de 2
    {
        int b = ceil(log2(m));
        int rValsSeparation = pow(2, b) - m;
        int *rBinary;

        if (r < rValsSeparation) // codificar resto com b-1 bits em binario
        {
            cout << "r pertence aos primeiros " << rValsSeparation << " valores de r" << endl;

            rBinary = decimalToBinary(r, b-1);
        }
        else // codificar a formula com b bits em binario
        {
            int num = r + pow(2, b) - m;
            rBinary = decimalToBinary(num, b);
        }
        return codeword(qUnary, q+1, rBinary, binArrLength);
    }
}
