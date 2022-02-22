/*Dual LWE CryptoSystem*/


// A is a public key

// Reciever genarates x

// Reciever sends A, u = Ax as public keys to the sender. 

//                              sender sends b' = s'A+e' (ciphertext -preamble)
//                              sender sends payload b' = s'u + e' + bit*q/2

//Reciever calculates b'-b'x. then decide 


#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>
#include "sodium.h"
#include <cmath>

using namespace std;
using Eigen::Matrix;
using Eigen::MatrixXd;

// defining the parameters
#define q 2000
// #define n 30
// #define m 270
#define n 30
#define m 270
#define e_min -7
#define e_max 7
#define PI 3.14
#define numberBits 128

// structures
// public key

struct publicKey
{
    Matrix<long, n, m> A;
    Matrix<long, n, numberBits> u;
};

struct privateKey
{
    Matrix<long, n, m> A;
    Matrix<long, m, numberBits> x;
};

struct cipherText
{
    Matrix<long, 1, m> bT;
    Matrix<long, 1, numberBits> b_;
};

// modulus function
long mod(long value, long mod_value) {
    return ((value % mod_value) + mod_value) % mod_value;
}

// genarate uniform random numbers including the boundaries
long genUniformRandomLong(int lowerBound, int upperBound) {
    long range = (upperBound - lowerBound) + 1;
    uint32_t randomNumber;
    randomNumber = randombytes_uniform(range);
    long randomNumberModified = ((long) randomNumber) + lowerBound;
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
void genarateKeys(privateKey* private_key, publicKey* public_key)
{
    // cout << "[LOG] Generating Matrix A" << endl;
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

    // genarating the x matrix
    // cout << "[LOG] Generating Matrix x" << endl;
    for (long row = 0; row < private_key->x.rows(); row++)
    {
        for (long col = 0; col < private_key->x.cols(); col++)
        {
            private_key->x(row, col) =  genUniformRandomLong(0, 1);
        }

    }
    // cout << "[DEBUG] Min of s : " << private_key->x.minCoeff() << " Max of s : " << private_key->x.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;    
    

    // sharig A among public and private key
    private_key->A = public_key->A;
    //generate u as public key

    public_key->u = ((private_key->A)*(private_key->x));

    for (long row = 0; row < (public_key->u).rows(); row++)
    {
        for (long col = 0; col < (public_key->u).cols(); col++)
        {
            public_key->u(row, col) = mod(public_key->u(row, col), q);
        }
 
    }
    
}

// Encrypting Function
cipherText encrypt(publicKey public_key, short message_bit[numberBits])
{
    struct cipherText cipher_text;

    // cout << "[LOG] Generating Matrix e" << endl;
    Matrix<long, 1, m> eT;
    // long total = 0;
    double alpha = sqrt(double(n))/q;
    double sigma = alpha/sqrt(2*PI);

    for (long col = 0; col < eT.cols(); col++)
    {
        // e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
        eT(col) = gaussian(sigma);
        // total += eT(col);
    }
    // cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

    // genarating the s matrix
    Matrix<long, 1, n> sT;

    // cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < sT.cols(); col++)
    {
        sT(col) = genUniformRandomLong(0, q-1);
    }


    // cout << "[DEBUG] Min of s : " << sT.minCoeff() << " Max of s : " << sT.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;

    cipher_text.bT = sT * (public_key.A) + eT;

    for (long col = 0; col < cipher_text.bT.cols(); col++) 
    {
        cipher_text.bT(0, col) = mod(cipher_text.bT(0, col), q);
    }

    Matrix<long, 1, numberBits> sTu = sT * (public_key.u); 

    // generate e'
    Matrix<long, 1, numberBits> e_;

    for(long col = 0; col < numberBits; col++)
    {
        e_(0, col) = gaussian(sigma);
        cipher_text.b_(0, col) = mod((sTu(0, col) + e_(0, col) + (message_bit[col] * (q / 2))), q);
    }


    // cout<<abs(e_- eT*private_key.x)<<endl;
    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
    return cipher_text;
}

// Decrypting Funciton
short* decrypt(privateKey private_key, cipherText cipher_text)
{
    Matrix<long, 1, numberBits> bTx;
    static short recovered[numberBits];

    bTx = cipher_text.bT * private_key.x;

    long difference = 0;

    for (long col = 0; col < numberBits; col++)
    {
       bTx(0 ,col) = mod(bTx(0, col), q);
       difference = mod(cipher_text.b_(0, col) - bTx(0, col), q);

        // recovering the single bit message
        if ((difference > (q / 4)) & (difference < (3 * q / 4)))
        { // bit is 1
            recovered[col] = 1;
        }
        else
            recovered[col] = 0;
    }


    return recovered;
}

// check the correctness
bool checkAnswer(short message_bits[numberBits], short recovered_bits[numberBits])
{
    long correctBits = 0;
    for (long i = 0; i < numberBits; i++)
    {
        if (message_bits[i] == recovered_bits[i])
        {
            correctBits++;
        }
    }
    // cout << "correct bits: " << correctBits << endl;
    if (correctBits == numberBits)
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

int main(int argc, char const *argv[])
{
    // genarating keys
    struct privateKey private_key;
    struct publicKey public_key;
    genarateKeys(&private_key, &public_key);
    // =====================================================================

    cout << "Testing for message bits: ";
    // printBits(message);
    cout << endl;

    // evaluating the success rate
    double success = 0;
    char result;
    long rounds = 100;
    for (size_t i = 0; i < rounds; i++)
    {
        // sample character to test
        short message[numberBits];
        for (long i = 0; i < numberBits; i++)
        {
            message[i] = (short)genUniformRandomLong(0, 1);
        }
        //  cout << "iteration = " << i << endl;
        cipherText cipher_text = encrypt(public_key, message);
        short *recovered_message = decrypt(private_key, cipher_text);

        //cout << "Encrypted: " ;
        //printBits(message);
        //cout << " Decrypted: ";
        //printBits(recovered_message);

        if (checkAnswer(message, recovered_message))
        {
            success++;
            cout << "  correct";
        } else {
            cout << "   incorrect";
        }

        cout << endl;
    }
    cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

    return 0;
}