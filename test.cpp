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
// #define q 2000
long q = 0;
// #define n 30
// #define m 270
#define n 2000
#define m 2000
#define e_min -1
#define e_max 1
#define PI 3.14
// define the number of bits in the bit stream
#define numberBits 128
// structures
// public key

Matrix<long, n, m> A;
Matrix<long, 1, m> bT;

Matrix<long, 1, numberBits> bTx;

// Matrix<long, n, m> A;
Matrix<long, 1, n> sT;

// Matrix<long, n, m> A;
// Matrix<long, 1, n> sT;

Matrix<long, 1, m> eT;

Matrix<long, 1, numberBits> sTu;

Matrix<long, m, numberBits> X;

Matrix<long, n, numberBits> u;
Matrix<long, 1, numberBits> u_;

// struct publicKey
// {
//     Matrix<long, n, m> A;
//     Matrix<long, 1, m> bT;
// };

// // private key
// struct privateKey
// {
//     Matrix<long, n, m> A;
//     Matrix<long, 1, n> sT;
// };

// // cipher text
// struct cipherText
// {
//     Matrix<long, n, numberBits> u;
//     Matrix<long, 1, numberBits> u_;
// };

// modulus function
long mod(long value, long mod_value)
{
    return ((value % mod_value) + mod_value) % mod_value;
}

// genarate uniform random numbers including the boundaries
long genUniformRandomlong(long lowerBound, long upperBound)
{
    long range = (upperBound - lowerBound) + 1;
    long randomNumber;
    randomNumber = randombytes_uniform(range);
    long randomNumberModified = ((long)randomNumber) + lowerBound;
    return randomNumberModified;
}

long gaussian(double sigma)
{

    mt19937 gen(randombytes_random());
    normal_distribution<double> gauss_dis(0, sigma);
    double val = gauss_dis(gen);
    if (val > 0.5)
        val = val - 1.0;
    else if (val < -0.5)
        val = val + 1;
    return (long)(val * q);
}

// function to genarate keys
void genarateKeys()
{
    cout << "[LOG] Generating Matrix A" << endl;

    // Genarating the matrix A
    long number = 0;
    for (long i = 0; i < n; i++)
    {
        for (long j = 0; j < m; j++)
        {
            // filling the A matrix from the numbers taken from the distribution
            A(i, j) = genUniformRandomlong(0, q - 1);
        }
    }

    // avarage of A's coiff should be close to q/2
    // cout << "[DEBUG] Min of A : " << A.minCoeff() << " Max of A : " << A.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;

    // genarating the s matrix
    // cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < sT.cols(); col++)
    {
        sT(col) = genUniformRandomlong(0, q - 1);
    }
    // cout << "[DEBUG] Min of s : " << sT.minCoeff() << " Max of s : " << sT.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;

    // genarating the error matrix
    // cout << "[LOG] Generating Matrix e" << endl;
    double alpha = sqrt(double(n)) / q;
    double sigma = alpha / sqrt(2 * PI);
    // Matrix<long, 1, m> eT;
    // long total = 0;
    for (long col = 0; col < eT.cols(); col++)
    {
        // e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
        eT(col) = gaussian(sigma);
        // total += eT(col);
    }
    // cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

    // calculating bT
    bT = (sT) * (A) + eT;

    // taking the modulus values of bT
    for (long col = 0; col < bT.cols(); col++)
    {
        bT(col) = mod(bT(col), q);
    }

    // cout << "[DEBUG] Min of B : " << bT.minCoeff() << " Max of B : " << bT.maxCoeff() << endl;

    // sharig A among public and private key
    // A = A;
}

// Encrypting Function
void encrypt(long message_bit[numberBits])
{
    // struct cipherText ;
    // Genarating the X matrix with random values

    for (long i = 0; i < m; i++)
    {
        for (long j = 0; j < numberBits; j++)
        {
            X(i, j) = genUniformRandomlong(0, 1);
        }
    }
    // cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
    // u = AX
    // should take mod q
    u = (A)*X;
    // cout<<"[DEBUG] min of u : "<<u.minCoeff()<<" max of u : "<<u.maxCoeff()<<endl;
    // defining bTx
    // Matrix<long, 1, numberBits> bTx;
    bTx = bT * X;
    // taking the modulus of bTx
    for (long i = 0; i < numberBits; i++)
    {
        bTx(0, i) = mod(bTx(0, i), q);
    }

    // encrypting the bits
    for (long i = 0; i < numberBits; i++)
    {
        u_(0, i) = mod((bTx(0, i) + (message_bit[i] * (q / 2))), q);
    }

    // cout<<"[DEBUG] u' : " <<u_ <<endl;
    // return ;
}

// Decrypting Funciton
long *decrypt()
{
    // defining sTu
    // Matrix<long, 1, numberBits> sTu;
    sTu = (sT) * (u);
    // array to hold the recoverd bits
    static long recovered[numberBits];
    long difference = 0;

    for (long i = 0; i < numberBits; i++)
    {
        sTu(0, i) = mod(sTu(0, i), q);
        // recovering the single bit message
        difference = mod(u_(0, i) - sTu(0, i), q);
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
    // // q
    // unsigned long tmp1 = 1;
    // tmp1 = tmp1 << 31;

    // // unsigned long tmp2 = 1;
    // // tmp2 = tmp2 << 14;
    // q = tmp1 - 19;
    // q = 5000;
    // // cout << "tmp1 = " << tmp1 << endl;
    // // cout << "tmp2 = " << tmp2 << endl;
    // cout << "q = " << q << endl;

    // // genarating keys
    // // struct privateKey ;
    // // struct publicKey ;
    // genarateKeys();
    // // =====================================================================

    // cout << "Testing for message bits: ";
    // // printBits(message);
    // cout << endl;

    // // evaluating the success rate
    // double success = 0;
    // char result;
    // long rounds = 10;
    // for (size_t i = 0; i < rounds; i++)
    // {
    //     // sample character to test
    //     long message[numberBits];
    //     for (long i = 0; i < numberBits; i++)
    //     {
    //         message[i] = genUniformRandomlong(0, 1);
    //     }
    //     //  cout << "iteration = " << i << endl;
    //     encrypt(message);
    //     long *recovered_message = decrypt();

    //     cout << "Encrypted: ";
    //     printBits(message);
    //     cout << " Decrypted: ";
    //     printBits(recovered_message);

    //     if (checkAnswer(message, recovered_message))
    //     {
    //         success++;
    //         cout << "  correct";
    //     }
    //     else
    //     {
    //         cout << "   incorrect";
    //     }

    //     cout << endl;
    // }
    // cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

    // return 0;
}