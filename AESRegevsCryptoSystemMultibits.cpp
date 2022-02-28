#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <string>
#include <cstdlib>
#include <crypto++/cryptlib.h>
#include <crypto++/hex.h>
#include <crypto++/filters.h>
#include <crypto++/aes.h>
#include <crypto++/ccm.h>
#include "assert.h"
#include <crypto++/osrng.h>
#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>
#include "sodium.h"
#include <cmath>

using namespace std;
using Eigen::Matrix;
using Eigen::MatrixXd;
using std::string;
using std::exit;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using std::cout;
using std::cerr;
using std::endl;
using CryptoPP::CTR_Mode;
using CryptoPP::AES;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Exception;
using CryptoPP::AutoSeededRandomPool;

#define AESKeyLength 32
// defining the parameters
#define q 2000
// #define n 30
// #define m 270
#define n 30
#define m 270
#define e_min -1
#define e_max 1
#define PI 3.14
// define the number of bits in the bit stream
#define numberBits 256

#define rounds 1000

// structures
// public key
struct publicKeyRegev
{
	Matrix<long, n, m> A;
	Matrix<long, 1, m> bT;
};

// private key
struct privateKeyRegev
{
	Matrix<long, n, m> A;
	Matrix<long, 1, n> sT;
};

// cipher text
struct cipherTextRegev
{
	Matrix<long, n, numberBits> u;
	Matrix<long, 1, numberBits> u_;    
};

struct AESPayload
{
	string AESCipherText;
	cipherTextRegev AESKey;
	cipherTextRegev AESIv;
};

// modulus function
long mod(long value, long mod_value)
{
	return ((value % mod_value) + mod_value) % mod_value;
}

short * binConvert(byte input[AESKeyLength]) 
{

  short*  bitstream = new short[AESKeyLength * 8];

  
  for (int i = 0; i < AESKeyLength; ++i)
   {
	for (int j = 0; j < 8; ++j) 
		{
		  bitstream[ (i*8 - j + 7)] = (input[i] >> j) & 1;    
		}
   } 
   
   return bitstream;
}

byte* binToByteConvert(short bitstream[AESKeyLength]) 
{

  byte*  bytestream = new byte[AESKeyLength];

  short binValues[8] = {128, 64, 32, 16, 8, 4, 2, 1};
  short total = 0;
  short bytePosition = 0;   

  for (int i = 1; i < AESKeyLength * 8 + 1; ++i)
   {
		total = total + bitstream[i - 1] * binValues[(i - 1) % 8];

		if(i % 8 == 0)
		{
			bytestream[bytePosition] = (byte)total;
			total = 0;
			bytePosition++;
		}
   }   

   return bytestream;
}

// genarate uniform random numbers including the boundaries
long genUniformRandomLong(int lowerBound, int upperBound)
{
	long range = (upperBound - lowerBound) + 1;
	uint32_t randomNumber;
	randomNumber = randombytes_uniform(range);
	long randomNumberModified = ((long)randomNumber) + lowerBound;
	return randomNumberModified;
}

long gaussian(double sigma){

	mt19937 gen(randombytes_random()); 
	normal_distribution<double> gauss_dis{0,sigma};
	double val = gauss_dis(gen);
	if (val > 0.5)
		val = val -1.0;
	else if(val<-0.5)
		val = val+1;
	return long(val*q); 

}

// function to genarate keys
void genarateRegevKeys(privateKeyRegev *private_key, publicKeyRegev *public_key)
{
	cout << "[LOG] Generating Matrix A" << endl;

	// Genarating the matrix A
	long number = 0;
	for (long i = 0; i < n; i++)
	{
		for (long j = 0; j < m; j++)
		{
			// filling the A matrix from the numbers taken from the distribution
			public_key->A(i, j) = genUniformRandomLong(0, q - 1);
		}
	}

	// avarage of A's coiff should be close to q/2
	// cout << "[DEBUG] Min of A : " << public_key->A.minCoeff() << " Max of A : " << public_key->A.maxCoeff() << endl;
	// cout << "[LOG] Done" << endl;

	// genarating the s matrix
	// cout << "[LOG] Generating Matrix s" << endl;
	for (long col = 0; col < private_key->sT.cols(); col++)
	{
		private_key->sT(col) = genUniformRandomLong(0, q - 1);
	}
	// cout << "[DEBUG] Min of s : " << private_key->sT.minCoeff() << " Max of s : " << private_key->sT.maxCoeff() << endl;
	// cout << "[LOG] Done" << endl;

	// genarating the error matrix
	// cout << "[LOG] Generating Matrix e" << endl;
	double alpha = sqrt(double(n))/q;
	double sigma = alpha/sqrt(2*PI);
	Matrix<long, 1, m> eT;
	// long total = 0;
	for (long col = 0; col < eT.cols(); col++)
	{
		// e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
		eT(col) = gaussian(sigma);
		// total += eT(col);
	}
	// cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

	// calculating bT
	public_key->bT = (private_key->sT) * (public_key->A) + eT;

	// taking the modulus values of bT
	for (long col = 0; col < public_key->bT.cols(); col++)
	{
		public_key->bT(col) = mod(public_key->bT(col), q);
	}

	// cout << "[DEBUG] Min of B : " << public_key->bT.minCoeff() << " Max of B : " << public_key->bT.maxCoeff() << endl;

	// sharig A among public and private key
	private_key->A = public_key->A;
}

// Encrypting Function
cipherTextRegev RegevEncrypt(publicKeyRegev public_key, short message_bit[numberBits])
{
	struct cipherTextRegev cipher_text;
	// Genarating the X matrix with random values
	Matrix<long, m, numberBits> X;
	for (long i = 0; i < m; i++)
	{
		for (long j = 0; j < numberBits; j++)
		{
			X(i, j) = genUniformRandomLong(0, 1);
		}
	}
	// cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
	// u = AX
	// should take mod q
	cipher_text.u = (public_key.A) * X;
	// cout<<"[DEBUG] min of u : "<<cipher_text.u.minCoeff()<<" max of u : "<<cipher_text.u.maxCoeff()<<endl;
	// defining bTx
	Matrix<long, 1, numberBits> bTx;
	bTx = public_key.bT * X;
	// taking the modulus of bTx
	for (long i = 0; i < numberBits; i++)
	{
		bTx(0, i) = mod(bTx(0, i), q);
	}

	// encrypting the bits
	for (long i = 0; i < numberBits; i++)
	{
		cipher_text.u_(0, i) = mod((bTx(0, i) + (message_bit[i] * (q / 2))), q);
	}

	// cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
	return cipher_text;
}

// Decrypting Funciton
byte *RegevDecrypt(privateKeyRegev private_key, cipherTextRegev cipher_text)
{
	// defining sTu
	Matrix<long, 1, numberBits> sTu;
	sTu = (private_key.sT) * (cipher_text.u);
	// array to hold the recoverd bits
	short*  recovered = new short[numberBits];
	long difference = 0;

	for (long i = 0; i < numberBits; i++)
	{
		sTu(0, i) = mod(sTu(0, i), q);
		// recovering the single bit message
		difference = mod(cipher_text.u_(0, i) - sTu(0, i), q);
		if ((difference > (q / 4)) & (difference < (3 * q / 4)))
		{ // bit is 1
			recovered[i] = 1;
		}
		else
		{
			recovered[i] = 0;
		}
	}

	return binToByteConvert(recovered);
}

// check the correctness
bool checkAnswer(string message, string recovered)
{
	if (!message.compare(recovered))
	{
		return true;
	}
	return false;
}

// print an array of bits
void printBits(short bit_array[numberBits])
{
	for (long i = 0; i < numberBits; i++)
	{
		cout << bit_array[i];
	}
}

string convertToHex(string& input)
{
	string converted;

	converted.clear();
	StringSource(input, true,
		new HexEncoder(
			new StringSink(converted)
		) // HexEncoder
	); // StringSource
	return converted;
}

string convertFromHex(string& input)
{
	string converted;

	converted.clear();
	StringSource(input, true,
		new HexEncoder(
			new StringSink(converted)
		) // HexEncoder
	); // StringSource

	return converted;
}

void generateAESKey(byte (&key)[AESKeyLength], byte (&iv)[AESKeyLength])
{
	AutoSeededRandomPool rnd;
	rnd.GenerateBlock(key, AESKeyLength);
	rnd.GenerateBlock(iv, AESKeyLength);    
}

string AESencrypt(byte key[AESKeyLength], byte iv[AESKeyLength], string plaintext)
{
	string cipher;
	try
	{
		CTR_Mode< AES >::Encryption e;
		e.SetKeyWithIV(key, AESKeyLength, iv);

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plaintext, true, 
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
	return cipher;
}


string AESDecrypt(byte key[AESKeyLength], byte iv[AESKeyLength], string cipher)
{
	string recovered;
	try
	{
		CTR_Mode< AES >::Decryption d;
		d.SetKeyWithIV(key, AESKeyLength, iv);

		// The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(cipher, true, 
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			) // StreamTransformationFilter
		); // StringSource

	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

	return recovered;
}



int main(int argc, char* argv[])
{
	byte key[AESKeyLength], iv[AESKeyLength];

	struct AESPayload AES;

	string message = "123 test ";

	generateAESKey(key, iv);

	// genarating regev system keys
	struct privateKeyRegev private_key;
	struct publicKeyRegev public_key;
	genarateRegevKeys(&private_key, &public_key);
	short *binKey = binConvert(key);
	short *binIV = binConvert(iv);

	double success = 0;

   for (int i = 0; i < rounds; ++i)
   {
   		AES.AESCipherText = AESencrypt(key, iv, message);

		AES.AESKey = RegevEncrypt(public_key, binKey);
		AES.AESIv = RegevEncrypt(public_key, binIV);

		byte *recovered_AESKey = RegevDecrypt(private_key, AES.AESKey);
		byte *recovered_AESIv = RegevDecrypt(private_key, AES.AESIv);

		string recovered = AESDecrypt(recovered_AESKey, recovered_AESIv, AES.AESCipherText);

		//delete[] binKey; delete[] binIV;

		if (checkAnswer(message, recovered))
        {
            success++;
        }
   }

    delete[] binKey; delete[] binIV;

	//cout<<"Message : "<<message<<endl;
	//cout<<"Recovered : "<<recovered<<endl;
   
	cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

	return 0;
}

