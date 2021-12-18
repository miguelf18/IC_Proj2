#include "Golomb.h"
#include <iostream>
#include <cmath>

using namespace std;

// constructor
Golomb::Golomb(int _i, int _m)
{
    i = _i;
    m = _m;
    checkValues(i, m);
}

Golomb::Golomb()
{
    i = 0;
    m = 1;
}

// functions
void Golomb::setValues(int _i, int _m)
{
    i = _i;
    m = _m;
    checkValues(i, m);
}

int Golomb::checkValues(int i,int m)
{
    if(i < 0)
    {
        cout << "i tem de ser maior ou igual a 0" << endl;
        exit(0);
    }
    if(m <= 0)
    {
        cout << "m tem de ser maior que 0" << endl;
        exit(1);
    }
    return 0;
}

int binArrLength = 0;
int unArrLength = 0;

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
            //cout << "dif = " << dif << endl;
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

    unArrLength = n+1;

    return res;
}

int Golomb::binaryToDecimal(int* bin) {
    int sum = 0;
	int n = binArrLength-1;
	int power = 1;

	while(n>0) { 
        if (bin[n]==1) sum = sum + pow(2, power);
        power++;
        n--;
    }

	if (bin[binArrLength-1]==1) sum=sum+1;

	return sum;
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

int * Golomb::encode() {

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
            //cout << "r pertence aos primeiros " << rValsSeparation << " valores de r" << endl;

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

int Golomb::decode(int* cw) {
    int r;
    int q = unArrLength-1;      // unario p decimal

    int cwLength = unArrLength+binArrLength;

    int *bin = new int[binArrLength];
    int bidx = 0;
    for(int i=unArrLength; i<cwLength; i++) {
        // cout << cw[i];
        bin[bidx] = cw[i];
        bidx++;
    }

    // caso m seja potencia de 2 -> passar p decimal o array bin
    if (ceil(log2(m)) == floor(log2(m))) { // m é potencia de 2
        r = binaryToDecimal(bin);   // conv bin to decimal
    }
    else { // m não é potencia de 2
        r = 1234567890;
    }

    return m*q+r; // i/m -> i = m*q+r
}
