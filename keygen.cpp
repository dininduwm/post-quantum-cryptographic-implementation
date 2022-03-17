/*
this algorithm uses short.
input :128 bit seed (128/8 = 16 bytes)
       128 /16 = 8
for i = 0; i<n;i++
    for j=0;j<m;J=J+8:
        b = [i][j]000...000 (need to be 128 bits.)
        b is 128 bit.
        encrypt b using seed.
        [ci,j][ci,j+1]...[ci,j+7] = AESseed(b). [cij] is 16 bit. result is also 16*8 = 128 bits
        for k=0;k<8;k++:
            Ai,j+k = (ci,j+k)modq 
randombytes_buf(&seed,sizeof(seed))
*/

#include <iostream>
#include <bitset>
#include <crypto++/aes.h>
#include "functions.h"
#include <crypto++/modes.h>
#include "sodium.h"
#include <boost/multiprecision/cpp_int.hpp>

using CryptoPP::AES;
using CryptoPP::ECB_Mode;
using namespace boost::multiprecision;
using namespace std;

#define n 30
#define m 370

int128_t byteToint(std::byte seed[16]){
    int128_t retU = -1;
    memset(&retU,0,16);
    memcpy(&retU,&seed,16);
    return retU;
}

string ECBMode_Encrypt(string text, byte key[], int keySize) {
    string cipher = "";
    //Encryption
    try
    {
        ECB_Mode<AES>::Encryption e;
        e.SetKey(key, keySize);
        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource(text, true, new StreamTransformationFilter(e, new StringSink(cipher))); // StringSource
    }
    catch(const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    return cipher;
}

void printArray(int16_t array[8]){
    cout<<"[ ";
    for (size_t i = 0; i < 8; i++)
    {
        cout<<array[i]<< " ,";
    }
    cout<<" ]"<<endl;
    
}



int main(int argc, char const *argv[])
{
    bitset<128>seed;
    cout<<sizeof(seed)<<endl;
    randombytes_buf(&seed,sizeof(seed));
    // coutzz<<hex<<seed
    cout<<seed<<endl;

    int16_t b[8] = {0,0,0,0,0,0,0,0};
    
    for (size_t i = 0; i < 128; i++)
    {
        cout<<seed[127-i];
    }
    cout<<endl;
    printArray(b);
    int16_t i,j;
    for (i = 0; i < 10; i++)
    {
        for(j=0;j<16;j=j+8)
        {
            // b = {0,0,0,0,0,0,0,0};
            b[0] = i;
            b[1] = j;
            cout<<b[0]<<" ";
            printArray(b);
            // encrypt here. 
            // for (size_t k = 0; k < 8; k++)
            // {
            //     if (j+k<m)
            //         A[i,j+k] = b[k]mod q
            // }
            
        }
    }
    cout<<sizeof(i)<<sizeof(j);

    // encrypt here with AES
    
    // cout<<b<<endl;
    // cout<<sizeof(seed)<<" "<<sizeof(b)<<endl;
    return 0;
}
