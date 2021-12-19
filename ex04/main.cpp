#include "Golomb.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    cout << "Test Golomb Class" << endl << endl;

    cout << "Encoder" << endl;

    Golomb g1(3, 5);
    cout << "i: " << g1.geti() << endl;
    cout << "m: " << g1.getm() << endl;
    int* cw = g1.encode();  

    cout << "codeword: ";
    for(int i=0; i<g1.cwArrLength; i++) {
        cout << cw[i];
    }

    cout << endl << endl << "Decoder" << endl;

    Golomb g2(5);
    int i = g1.decode(cw);
    cout << "i = " << i;

    // Golomb g3;
    // g3.setValues(1,1);
    // cout << g3.geti() << endl;
    // cout << g3.getm() << endl;

    return 0;
}