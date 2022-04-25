#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <crypto++/files.h>
// #include <crypto++/cryptlib.h>
// #include <crypto++/filters.h>
// #include <crypto++/osrng.h>
// using namespace std;

// compile g++ ___.cpp -lcrypto++

using namespace std;

union un
{
    byte buff[16];
    int int_buf[4];
    int16_t short_buff[8];
};
union un1
{
    byte buff[32];
    int int_buf[8];
    int16_t short_buff[16];
};
using namespace CryptoPP;

// void initiate(byte *currentHash)
// {
//     SHA256 hash;
//     byte *output;
//     hash.Update(currentHash, sizeof(currentHash));
//     hash.Final(output);
//     memcpy(currentHash, output, sizeof(output));
//     // return currentHash;
// }



byte *populate(std::string message, int size, byte *initializer)
{
    return initializer;
}

void printBytes(byte *array, int byteSize)
{
    std::cout << "Size of array: " << sizeof(array) << std::endl;
    HexEncoder encoder(new FileSink(std::cout));
    encoder.Put(array, byteSize);
    encoder.MessageEnd();
    std::cout << std::endl;
}

string stringHash(string message)
{
    SHA256 hash;
    string digest;
    hash.Update((const byte*)message.data(), message.size());
    digest.resize(hash.DigestSize());
    hash.Final((byte*)&digest[0]);

    return digest;
}

void appendBytesToString(std::string &str, byte *array, size_t num_bytes)
{
    // This will concatanate the sigma(random key) and a given message in string format
    str.append((char *)array, num_bytes);
}

void addEntries(long **matrix, int n, int m, byte *input, byte *output)
{

   

    /*
    m=100,n=20
    count = ceil(n*m/256)
    output = hash(input)
    // check we need more iterations
    for i=0 -> count:
        256 m=2,
        output[] == 0:
            [m][n]=0
        else:
            [m][n]=1
        output = hash(output)

    */
}

void generateValues(size_t count, byte *array)
{
    // generate binary values
    int iterations = (count / 32);
    if (count % 32 != 0)
    {
        iterations++;
    }
    std::cout << iterations << std::endl;
}

void printArray(int *array, int length)
{

    for (size_t i = 0; i < length; i++)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

bool* generateR(short size, string key)
{
    if(size % 256 != 0)
    {
        size = 1024;
    }
    bool* r = new bool[size];
    short bitPosition;

    for(int segment = 0; segment < size/256; segment++)
    { 
       key = stringHash(key);

       for(short byte = 0; byte < 32; byte++)
        {
            bitPosition = byte * 8 + segment * 256;

            r[bitPosition] = key[byte] & 128;
            r[bitPosition + 1] = key[byte] & 64; 
            r[bitPosition + 2] = key[byte] & 32;
            r[bitPosition + 3] = key[byte] & 16;
            r[bitPosition + 4] = key[byte] & 8;
            r[bitPosition + 5] = key[byte] & 4;
            r[bitPosition + 6] = key[byte] & 2;
            r[bitPosition + 7] = key[byte] & 1;        
        }
    }

    return r;
}

int main(int argc, char const *argv[])
{
        short size = 1024;
        std::string msg = "Yoda said, Do or do not. There is no try.";
        bool* r = generateR(size ,msg);


        for (short bit = 0; bit < size; bit++)
        {
            cout<<"bit = " << bit + 1 << ": " << r[bit] <<endl;
        }


/*
        bool r[256];
        short bit;

        for (short byte = 0; byte < 32; byte++)
        {
            bit = byte * 8;

            r[bit] = digest[byte] & 128;
            r[bit + 1] = digest[byte] & 64; 
            r[bit + 2] = digest[byte] & 32;
            r[bit + 3] = digest[byte] & 16;
            r[bit + 4] = digest[byte] & 8;
            r[bit + 5] = digest[byte] & 4;
            r[bit + 6] = digest[byte] & 2;
            r[bit + 7] = digest[byte] & 1;        
        }

      for (short bit = 0; bit < 256; bit++)
        {
            cout<<"bit = " << bit + 1 << ": " << r[bit] <<endl;
        }
*/
/*
         int a = 5, b = 9;
  
    // The result is 00000001
    cout<<"a = " << a <<","<< " b = " << b <<endl;
    cout << "a & b = " << (a & b) << endl;
  
*/





}
