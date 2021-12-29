#include "Golomb.h"

// constructor
Golomb::Golomb(int _m, string file_name, unsigned char file_mode)
{
    m = _m;
    checkValues(m);
    this->file_name = file_name;
    this->file_mode = file_mode;
    if(file_mode)
        // write
        bit_stream = new BitStream(file_name, "out");
    else
        // read
        bit_stream = new BitStream(file_name, "in");
}

Golomb::Golomb(string file_name, unsigned char file_mode)
{
    m = 1;
    this->file_name = file_name;
    this->file_mode = file_mode;
    if(file_mode)
        // write
        bit_stream = new BitStream(file_name, "out");
    else
        // read
        bit_stream = new BitStream(file_name, "in");
}

// functions
void Golomb::setValues(int _m)
{
    m = _m;
    checkValues(m);
}

int Golomb::checkValues(int m)
{
    if(m <= 0) {
        cout << "m tem de ser maior que 0" << endl;
        exit(1);
    }
    return 0;
}

int * Golomb::decimalToBinary(int n)
{
    // write remainder truncated binary code
    // write remainder bits from most signiticant to least significant
    // compute minimum bit length for remainder
    int b = floor(log2(m));
    // cut off point for remainder length to avoid unused codes
    // if remainder < k: only need b bits to represent
    // if remainder >= k: need b+1 bits to represent
    int k = pow(2, (b+1)) - m;
    unsigned int mask = 1;
    if(n >= k)
    {
        // remainder needs one more bit
        b++;
        // add k offset
        n += k;
    }
    mask <<= b-1;
    int* res = new int[b];
    for(int i = 0; i < b; i++)
    {
        unsigned char masked_bits = (mask & n);
        n <<= 1;
        if(masked_bits > 0)
        {
            (*bit_stream).write_bit(1);
            res[i] = 1;
        }  
        else
        {
            (*bit_stream).write_bit(0);
            res[i] = 0;
        }
    }

    return res;
}

int * Golomb::decimalToUnary(int n) {

    int* res = new int[n+1];

    for(int i=0; i<n; i++) {
        res[i] = 1;
        bit_stream->write_bit(1);
    }
    res[n] = 0;
    bit_stream->write_bit(0);

    return res;
}

int Golomb::binaryToDecimal(int* remainder) {
    // read next remainder truncated binary code
    // compute minimum bit length for remainder
    int b = floor(log2(m));
    // cut off point for remainder length to avoid unused codes
    // if remainder < k: only need b bits to represent
    // if remainder >= k: need b+1 bits to represent
    int k = pow(2, (b+1)) - m;
    unsigned char bit = 0; 
    unsigned int remainder_count = 0;
    unsigned char remainder_bits = 0;
    bool error = 0;
    do
    {
        remainder_bits <<= 1;
        error = (*bit_stream).read_bit(&bit);
        if(error)
            // there was an error with the file or EOF was reached
            return error;
        remainder_bits |= (0x01 & bit);
        remainder_count++;
    }
    while(remainder_count < b);
    // check remainder length
    if(remainder_bits >= k)
    {
        // remainder needs one more bit
        remainder_bits <<= 1;
        error = (*bit_stream).read_bit(&bit);
        if(error)
            return error;
        remainder_bits |= (0x01 & bit);
        // remove k offset
        remainder_bits -= k;
    }
    (*remainder) = (int) remainder_bits;
	return 0;
}


void Golomb::encode(int* value) {

    int q, r;

    unsigned int absolute_value = abs(*value);

    q = floor(absolute_value / m); // quociente -> unario
    r = absolute_value % m;  // resto -> binario
    // cout << "q: " << q << endl;
    // cout << "r: " << r << endl;

    // write unary code
    int *qUnary = decimalToUnary(q);

    // write remainder truncated binary code
    decimalToBinary(r);

    // write sign bit (if value is 0, no sign bit is used)
    if(*value < 0)
        (*bit_stream).write_bit(1);
    else if(*value > 0)
        (*bit_stream).write_bit(0);
}

int Golomb::decode() {
    int r, i; // i/m -> i = m*q+r
    int q; // unario p decimal

    // read next quotient unary code
    unsigned int unary_count = 0;
    unsigned char bit = 0;
    bool error = 0;
    do
    {
        error = (*bit_stream).read_bit(&bit);
        if(error)
            // there was an error with the file or EOF was reached
            return error;
        if(bit != 0)
            unary_count++;
    } 
    while(bit == 1);

    q = unary_count;

    // read next remainder truncated binary code
    binaryToDecimal(&r);

    i = q * m + r;

    // check for sign bit (if value is 0, no sign bit is used)
    if(q + r != 0)
    {
        (*bit_stream).read_bit(&bit);
        if(error)
            return error;
        if(bit == 1)
            i *= (-1);
    }

    // cout << "r: " << r << endl;
    // (*value) = i;
    return 0;
}

bool Golomb::close()
{
    // add padding to last byte of data if needed
    for(unsigned char i = (*bit_stream).size(); i < 8; i++)
        (*bit_stream).write_bit(0);   
    return (*bit_stream).close();
}
