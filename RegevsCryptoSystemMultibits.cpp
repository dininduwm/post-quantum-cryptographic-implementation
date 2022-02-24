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
#define e_min -1
#define e_max 1
#define PI 3.14
// define the number of bits in the bit stream
#define numberBits 128
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
    Matrix<long, n, numberBits> u;
    Matrix<long, 1, numberBits> u_;
};

// modulus function
long mod(long value, long mod_value)
{
    return ((value % mod_value) + mod_value) % mod_value;
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
    normal_distribution<double> gauss_dis(0,sigma);
    double val = gauss_dis(gen);
    if (val > 0.5)
        val = val -1.0;
    else if(val<-0.5)
        val = val+1;
    return long(val*q); 

}

// function to genarate keys
void genarateKeys(privateKey *private_key, publicKey *public_key)
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
cipherText encrypt(publicKey public_key, long message_bit[numberBits])
{
    struct cipherText cipher_text;
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
long *decrypt(privateKey private_key, cipherText cipher_text)
{
    // defining sTu
    Matrix<long, 1, numberBits> sTu;
    sTu = (private_key.sT) * (cipher_text.u);
    // array to hold the recoverd bits
    static long recovered[numberBits];
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

    return recovered;
}

// check the correctness
bool checkAnswer(long message_bits[numberBits], long recovered_bits[numberBits])
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
void printBits(long bit_array[numberBits])
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
    long rounds = 1000;
    for (size_t i = 0; i < rounds; i++)
    {
        // sample character to test
        long message[numberBits];
        for (long i = 0; i < numberBits; i++)
        {
            message[i] = genUniformRandomLong(0, 1);
        }
        //  cout << "iteration = " << i << endl;
        cipherText cipher_text = encrypt(public_key, message);
        long *recovered_message = decrypt(private_key, cipher_text);

        cout << "Encrypted: " ;
        printBits(message);
        cout << " Decrypted: ";
        printBits(recovered_message);

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