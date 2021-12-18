#include "Golomb.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    // if (argc != 3)
    // {
    //     cout << "Usage: " << argv[0] << " <i> <m>" << endl;
    //     return -1;
    // }

    // int i = stoi(argv[1]);
    // int m = stoi(argv[2]);

    // if (i < 0)
    // {
    //     cout << "i tem de ser maior ou igual a 0" << endl;
    //     return -2;
    // }
    // if(m <= 0)
    // {
    //     cout << "m tem de ser maior que 0" << endl;
    //     return -3;
    // }

    // Golomb g;
    // g.encoder(i, m);
    // cout << g.unArrLength << endl << g.binArrLength;

    Golomb g1(15, 4);
    cout << "i: " << g1.geti() << endl;
    cout << "m: " << g1.getm() << endl;
    int* cw = g1.encode();
    int d = g1.decode(cw);
    cout << d;

    // Golomb g2;
    // // g2.setValues(1,1);
    // cout << g2.geti() << endl;
    // cout << g2.getm() << endl;

    return 0;
}