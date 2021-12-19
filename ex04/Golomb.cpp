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

Golomb::Golomb(int _m)
{
    i = 0;
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
    if(i < 0) {
        cout << "i tem de ser maior ou igual a 0" << endl;
        exit(0);
    }
    if(m <= 0) {
        cout << "m tem de ser maior que 0" << endl;
        exit(1);
    }
    return 0;
}

int binArrLength = 0;
int unArrLength = 0;
int cwArrLength = unArrLength+binArrLength;

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
	int power = 0;
	
	for(int i=binArrLength-1; i>=0; i--){
        sum = sum + bin[i]*pow(2, power);
        power++;
	}
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

    // cout << "codeword: ";
    // for(int i=0; i<uLength+bLength; i++) {
    //     cout << cw[i];
    // }
    // cout << endl;
    cwArrLength = uLength+bLength;

    return cw;
}

int * Golomb::encode() {

    int q, r;

    q = floor(i / m); // quociente -> unario
    r = i - (q * m);  // resto -> binario
    // cout << "q: " << q << endl;
    // cout << "r: " << r << endl;

    int *qUnary = decimalToUnary(q);

    if(ceil(log2(m)) == floor(log2(m))) { // m é potencia de 2
        int *rBinary = decimalToBinary(r);
        return codeword(qUnary, q+1, rBinary, binArrLength);
    }
    else { // m não é potencia de 2
        int b = ceil(log2(m));
        int unusedCodes = pow(2, b) - m;
        int *rBinary;

        if(r < unusedCodes) { // codificar resto com b-1 bits em binario
            //cout << "r pertence aos primeiros " << unusedCodes << " valores de r" << endl;
            rBinary = decimalToBinary(r, b-1);
        }
        else { // codificar a formula com b bits em binario
            int num = r + pow(2, b) - m;
            rBinary = decimalToBinary(num, b);
        }
        return codeword(qUnary, q+1, rBinary, binArrLength);
    }
}

int Golomb::decode(int* cw) {
    int r, i; // i/m -> i = m*q+r
    int q = unArrLength-1; // unario p decimal

    int cwLength = unArrLength+binArrLength;

    int *bin = new int[binArrLength];
    int bidx = 0;
    for(int i=unArrLength; i<cwLength; i++) {
        bin[bidx] = cw[i];
        bidx++;
    }

    if(ceil(log2(m)) == floor(log2(m))) { // m é potencia de 2
        r = binaryToDecimal(bin); // conv bin to decimal
        i = m*q+r;
    }
    else { // m não é potencia de 2
        int wrong_r = binaryToDecimal(bin);
        int b = ceil(log2(m));
        int unusedCodes = pow(2, b) - m;
        int *rBinary;

        if(wrong_r < unusedCodes) { // primeiros "unusedCodes" valores de r
            // cout << "r pertence aos primeiros " << unusedCodes << " valores de r" << endl;
            r = binaryToDecimal(bin);
            i = m*q+r;
        }
        else {
            i = wrong_r - unusedCodes;
        }
    }
    // cout << "r: " << r << endl;
    return i;
}
