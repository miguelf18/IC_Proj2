#include "bitStream.hpp"

    // constructor
    // takes a file path and the mode in or out
    BitStream::BitStream(string file_name, string file_mode)
    {
        if(file_mode == "out")
        {
            this->file_mode = 1;
            output_file.open(file_name, ios::binary);
        }
        else
        {
            this->file_mode = 0;
            input_file.open(file_name, ios::binary);
        }
    }

    // returns the current buffer count size
    unsigned char BitStream::size()
    {
        return bit_buffer.count;
    }

    // writes bit to buffer
    // bits are written from most significant to least significant bit
    // if bits exceed buffer size, it will only contain the last buffer size bits
    void BitStream::write_bit(unsigned char bit)
    {
        // shift buffer for next bit
        bit_buffer.buffer <<= 1;
        // add bit to buffer
        bit_buffer.buffer |= (0x01 & bit);
        // update buffer count
        if(bit_buffer.count < bit_buffer.size)
            bit_buffer.count++;
    }

    // writes n bits to buffer
    // bits are written from most significant to least significant bit
    // if bits exceed buffer size, it will only contain the last buffer size bits
    void BitStream::write_n_bits(unsigned char *bits, int bits_size)
    {
        for(int i = 0; i < bits_size; i++)
        {
            // shift buffer for next bit
            bit_buffer.buffer <<= 1;
            // add bit to buffer
            bit_buffer.buffer |= (0x01 & (*bits));
            bits++;
            // update buffer count
            if(bit_buffer.count < bit_buffer.size)
                bit_buffer.count++;
        }
    }

    // writes 8 bits to open file and resets buffer
    // returns 1 if there was an error with the file, or 0 otherwise
    bool BitStream::flush_file()
    {
        if(!file_mode || !output_file.is_open() || bit_buffer.count != bit_buffer.size)
            return 1;
        output_file << bit_buffer.buffer;
        // buffer reset
        bit_buffer.buffer &= 0x00;
        // update buffer count
        bit_buffer.count = 0;
        return 0;
    }

    // reads 1 bit from buffer and stores it in a given memory address
    // bits are read from least significant to most significant bit 
    // if buffer is empty, nothing happens
    void BitStream::read_bit(unsigned char *bit)
    {
        if(bit_buffer.count != 0)
        {
            // make sure storage has no previous content
            (*bit) &= 0x00;
            // store bit from buffer
            (*bit) |= (0x01 & bit_buffer.buffer);
            // discard stored bit from buffer
            bit_buffer.buffer >>= 1;
            // update buffer count
            if(bit_buffer.count > 0)
                bit_buffer.count--;
        }
    }

    // reads n bits from buffer and stores it in a given memory address
    // bits are read from least significant to most significant bit 
    // if buffer is empty, nothing happens
    void BitStream::read_n_bits(unsigned char *bits, int bits_size)
    {
        if(bit_buffer.count != 0)
        {
            for(int i = 0; i < bits_size; i++)
            {
                // make sure storage has no previous content
                (*bits) &= 0x00;
                // store bit from buffer
                (*bits) |= (0x01 & bit_buffer.buffer);
                bits++;
                // discard stored bit from buffer
                bit_buffer.buffer >>= 1;
                // update buffer count
                if(bit_buffer.count > 0)
                    bit_buffer.count--;
            }
        }
    }

    // reads file and fills buffer with 1 byte of data, overwriting any existing buffer content
    // returns 1 if there was an error with the file, or 0 otherwise
    bool BitStream::read_file()
    {
        if(file_mode || !input_file.is_open())
            return 1;
        char byte = 0;
        input_file.read(&byte, 1);
        // fill buffer with data
        bit_buffer.buffer = byte;
        // update buffer count
        bit_buffer.count = 8;
        return 0;
    }

    // try to close the file 
    // returns 1 if there was an error, or 0 otherwise
    bool BitStream::close()
    {
        if(file_mode)
        {
            if(output_file.is_open())
            {
                output_file.close();
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if(input_file.is_open())
            {
                input_file.close();
            }
            else
            {
                return 1;
            }
        }
        return 0;
    }
