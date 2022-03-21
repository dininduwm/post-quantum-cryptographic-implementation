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
using std::cout;
using std::cerr;
using std::endl;
#include <cstdint>
// using std::bitset;
#include <bitset>
#include <crypto++/aes.h>
using CryptoPP::AES;
#include <crypto++/modes.h>
using CryptoPP::ECB_Mode;
#include "sodium.h"
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
#include "crypto++/osrng.h"
using CryptoPP::AutoSeededRandomPool;
#include "crypto++/cryptlib.h"
using CryptoPP::Exception;
#include "crypto++/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "crypto++/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include <string>
using std::string;
// typedef unsigned char byte

#define n 10
#define m 5

union un{
    byte buff[16];
    // std::bitset<128> bitset_buff;
    int int_buf[4];
    int16_t short_buff[8];
    char char_buf[16];
};

int128_t byteToint(std::byte seed[16]){
    int128_t retU = -1;
    memset(&retU,0,16);
    memcpy(&retU,&seed,16);
    return retU;
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
    std::bitset<128>seed;
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
    // printArray(b);
    int16_t i,j;
    /*
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
    */
    cout<<sizeof(i)<<sizeof(j)<<endl;

    AutoSeededRandomPool prng;
    union un u1;
    byte key[AES::DEFAULT_KEYLENGTH];
    cout<<AES::DEFAULT_KEYLENGTH<<endl;
    prng.GenerateBlock(u1.buff,sizeof(u1.buff));
    string plain = "Hello crypto1243";
    // cout<<seed<<endl;
    // memcpy(&key,&seed,sizeof(key));
    // cout<<u1.short_buff<<endl;
    printArray(u1.short_buff);
    string cipher,encoded,recovered;
    memcpy(&u1.buff,&key,sizeof(key));
    encoded.clear();
    StringSource(key,sizeof(key),true,new HexEncoder(new StringSink(encoded)));
    cout<<"key: "<<encoded<<" "<<sizeof(key)<<endl;
    cout<<"key: "<<key<<" "<<sizeof(key)<<endl;
    // int16_t b[8]= {1,2,0,0,0,0,0,0};
    b[0] = 'a';
    b[1] = 'c';
    b[3] = 'c';
    b[4] = 'c';
    b[5] = 'c';
    b[6] = 'c';
    b[7] = 0;
    string test;

    // memcpy(&test,&b,sizeof(b));
    // cout<<test<<endl;
    printArray(b);
    /*
        CREATE A PLAINTEXT FROM ARRAY OF SHORT INTEGERS -16 BIT
        ENCRYPT,DECRYPT AND CHECK THE SCHEME WORKS. 
    */
    try
	{
		cout << "plain text: " << plain <<" "<<sizeof(plain)/sizeof(char) <<endl;

		ECB_Mode< AES >::Encryption e;
		e.SetKey(key, sizeof(key));

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plain, true, 
			new StreamTransformationFilter(e,
				new StringSink(cipher)
			) // StreamTransformationFilter      
		); // StringSource
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	/*********************************\
	\*********************************/

	// Pretty print
	encoded.clear();
	StringSource(cipher, true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
    cout<<sizeof(int)<<endl;
    int c_arr[8];
    memcpy(&c_arr,&cipher,sizeof(c_arr));
    for (size_t i = 0; i < 8; i++)
    {
        cout<<c_arr[i]<<" ";
    }
    cout<<endl;
	cout << "cipher text: " << encoded<<" "<<sizeof(cipher)/sizeof(char) << endl; 
	/*********************************\
	\*********************************/

	try
	{
		ECB_Mode< AES >::Decryption d;
		d.SetKey(key, sizeof(key));

		// The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(cipher, true, 
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			) // StreamTransformationFilter
		); // StringSource

		cout << "recovered text: " << recovered << endl;
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}
    // prng.GenerateBlock(seed,sizeof(seed));
    
    // encrypt here with AES
    
    // cout<<b<<endl;
    // cout<<sizeof(seed)<<" "<<sizeof(b)<<endl;
    // for (i = 0; i < n; i++)
    // {
    //     for(j=0;j<m;j=j+8)
    //     {
    //         // b = {0,0,0,0,0,0,0,0};
    //         b[0] = i;
    //         b[1] = j;
    //         cout<<b[0]<<" ";
    //         printArray(b);
            

    //         // for (size_t k = 0; k < 8; k++)
    //         // {
    //         //     if (j+k<m)
    //         //         A[i,j+k] = b[k]mod q
    //         // }
            
    //     }
    // }
    return 0;
}
