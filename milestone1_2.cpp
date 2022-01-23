#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>
#include "sodium.h"

using namespace std;
using Eigen::Matrix;
using Eigen::MatrixXd;

// defining the parameters
#define q 1000000
// #define n 30
// #define m 270
#define n 30
#define m 270
#define e_min -1
#define e_max 1

// structures
// public key
struct publicKey
{
    Matrix<long, n, m> A;
    Matrix<long, 1, m> bT;
};

// private key
struct privateKey
{
    Matrix<long, n, m> A;
    Matrix<long, 1, n> sT;
};

// cipher text
struct cipherText
{
    Matrix<long, n, 1> u;
    long u_;
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


// function to genarate keys
void genarateKeys(privateKey* private_key, publicKey* public_key)
{
    cout << "[LOG] Generating Matrix A" << endl;

    // Genarating the matrix A
    long number = 0;
    for (long i = 0; i < n; i++)
    {
        for (long j = 0; j < m; j++)
        {
            // filling the A matrix from the numbers taken from the distribution
            public_key->A(i, j) = genUniformRandomLong(0,q-1);
        }
    }

    // avarage of A's coiff should be close to q/2
    cout << "[DEBUG] Min of A : " << public_key->A.minCoeff() << " Max of A : " << public_key->A.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;

    // genarating the s matrix
    cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < private_key->sT.cols(); col++)
    {
        private_key->sT(col) = genUniformRandomLong(0, q-1);
    }
    cout << "[DEBUG] Min of s : " << private_key->sT.minCoeff() << " Max of s : " << private_key->sT.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;

    // genarating the error matrix
    cout << "[LOG] Generating Matrix e" << endl;
    Matrix<long, 1, m> eT;
    long total = 0;
    for (long col = 0; col < eT.cols(); col++)
    {
        // e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
        eT(col) = genUniformRandomLong(e_min, e_max);
        total += eT(col);
    }
    cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

    // calculating bT
    public_key->bT = (private_key->sT) * (public_key->A) + eT;

    // taking the modulus values of bT
    for (long col = 0; col < public_key->bT.cols(); col++)
    {
        public_key->bT(col) = mod(public_key->bT(col), q);
    }

    cout << "[DEBUG] Min of B : " << public_key->bT.minCoeff() << " Max of B : " << public_key->bT.maxCoeff() << endl;

    // sharig A among public and private key
    private_key->A = public_key->A;
}

// Encrypting Function
cipherText encrypt(publicKey public_key, long message_bit)
{
    struct cipherText cipher_text;
    // Genarating the X matrix with random values
    Matrix<long, m, 1> X;
    for (long i = 0; i < m; i++)
    {
        X(i) = genUniformRandomLong(0,1);
    }
    // cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
    // u = AX
    // should take mod q
    cipher_text.u = (public_key.A) * X;
    // cout<<"[DEBUG] min of u : "<<cipher_text.u.minCoeff()<<" max of u : "<<cipher_text.u.maxCoeff()<<endl;
    long bTx = mod(((public_key.bT * X) % q), q);

    // encrypting the bit
    cipher_text.u_ = mod((bTx + (message_bit * (q / 2))), q);

    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
    return cipher_text;
}

// Decrypting Funciton
long decrypt(privateKey private_key, cipherText cipher_text)
{
    long sTu = mod(((private_key.sT) * (cipher_text.u)), q);

    // recovering the single bit message
    long recovered = 1;
    if ((abs(cipher_text.u_ - sTu) % q) <= q / 4)
    { // bit is 1
        recovered = 0;
    }

    return recovered;
}

int main(int argc, char const *argv[])
{
    // genarating keys
    struct privateKey private_key;
    struct publicKey public_key;
    genarateKeys(&private_key, &public_key);

    // sample character to test
    long message;

    cout << "Enter bit:: "; 
    cin >> message; 

    cout<<"################# INPUT:  "<< message <<endl;

    // evaluating the success rate
     double success = 0;
     char result;
     long rounds = 1000;
     for (size_t i = 0; i < rounds; i++)
     {
         cout << "iteration = " << i << endl;
         cipherText cipher_text = encrypt(public_key, message);
         long recovered_message = decrypt(private_key, cipher_text);
         if(message == recovered_message)
             success++;
     }
     cout<<"Encryption and Decryption works "<<(success/rounds)* 100<<"% of time."<<endl;

    return 0;
}