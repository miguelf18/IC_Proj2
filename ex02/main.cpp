#include "bitStream.hpp"

int main(int argc, char **argv)
{
    BitStream bit_out_stream("output.out", "out");

    cout << endl << "Writing bit by bit: " << endl;

    // writes from most significant to least significant bit
    bit_out_stream.write_bit(0);
    cout << endl << "0" << endl;
    bit_out_stream.write_bit(0);
    cout << endl << "0" << endl;   
    bit_out_stream.write_bit(1);
    cout << endl << "1" << endl;
    bit_out_stream.write_bit(0);
    cout << endl << "0" << endl;
    bit_out_stream.write_bit(0);
    cout << endl << "0" << endl;
    bit_out_stream.write_bit(0);
    cout << endl << "0" << endl;
    bit_out_stream.write_bit(1);
    cout << endl << "1" << endl;
    bit_out_stream.write_bit(1);
    cout << endl << "1" << endl;

    cout << endl << "Writing 8 bits: 1100 1100 " << endl;

    // writes from most significant to least significant bit
    unsigned char write_bits[] = {1, 1, 0, 0, 1, 1, 0, 0};

    bit_out_stream.write_n_bits(write_bits, 8);

    cout << endl << "Closing file... " << endl;

    bit_out_stream.close();

    cout << endl << "Opening file... " << endl;

    BitStream bit_in_stream("output.out", "in");

    cout << endl << "Reading bit by bit: " << endl;

    // reads from least significant to most significant bit
    unsigned char bit = 0;
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);
    cout << endl << (bit == 1) << endl;   
    bit_in_stream.read_bit(&bit);  
    cout << endl << (bit == 1) << endl;   

    unsigned char read_bits[8];

    bit_in_stream.read_n_bits(read_bits, 8);

    cout << endl << "Reading 8 bits: " << endl;

    for(auto bit : read_bits)
        cout << endl << (bit == 1) << endl;   

    cout << endl << "Closing file... " << endl;

    bit_in_stream.close();

    return 0;
}