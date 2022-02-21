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


// structures
// public key

struct publicKey
{
    Matrix<long, n, m> A;
    Matrix<long, n, 1> u;
};

struct privateKey
{
    Matrix<long, n, m> A;
    Matrix<long, m, 1> x;
};

struct cipherText
{
    Matrix<long, 1, m> bT;
    long b_;
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
        private_key->x(row) =  genUniformRandomLong(0, 1);
    }
    // cout << "[DEBUG] Min of s : " << private_key->x.minCoeff() << " Max of s : " << private_key->x.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;    
    

    // sharig A among public and private key
    private_key->A = public_key->A;
    //generate u as public key
    public_key->u = ((private_key->A)*(private_key->x));
    for (long row = 0; row < (public_key->u).rows(); row++)
    {
        public_key->u(row) = mod(public_key->u(row), q); 
    }
    
}

// Encrypting Function
cipherText encrypt(publicKey public_key, long message_bit)
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

    for (long col = 0; col < cipher_text.bT.cols(); col++) {
        cipher_text.bT(col) = mod(cipher_text.bT(col), q);
    }

    long sTu = sT * (public_key.u); 
    long e_ =  genUniformRandomLong(e_min, e_max);
    cipher_text.b_ = mod((sTu + e_ + (message_bit * (q / 2))), q); // what is e'? eT * X??

    // cout<<abs(e_- eT*private_key.x)<<endl;
    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
    return cipher_text;
}

// Decrypting Funciton
long decrypt(privateKey private_key, cipherText cipher_text)
{
    long bTx = mod(((cipher_text.bT) * (private_key.x)), q);

    // recovering the single bit message
    long recovered = 0;
    long difference = mod(cipher_text.b_ - bTx, q);
    if ((difference > (q / 4)) & (difference < (3 * q / 4)))
    { // bit is 1
        recovered = 1;
    }

    return recovered;
}

int main(int argc, char const *argv[])
{
    // genarating keys
    struct privateKey private_key;
    struct publicKey public_key;
    genarateKeys(&private_key, &public_key);
    // =====================================================================
    // sample character to test
    long message = 1;

    cout << "Testing for message bit: " << message << endl;

    // evaluating the success rate
    double success = 0;
    char result;
    long rounds = 1000;
    for (size_t i = 0; i < rounds; i++)
    {
        //  cout << "iteration = " << i << endl;
        cipherText cipher_text = encrypt(public_key, message);
        long recovered_message = decrypt(private_key, cipher_text);
        if (message == recovered_message)
            success++;
    }
    cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

    // =====================================================================
    // sample character to test
    message = 0;

    cout << "Testing for message bit: " << message << endl;

    // evaluating the success rate
    success = 0;
    for (size_t i = 0; i < rounds; i++)
    {
        //  cout << "iteration = " << i << endl;
        cipherText cipher_text = encrypt(public_key, message);
        long recovered_message = decrypt(private_key, cipher_text);
        if (message == recovered_message)
            success++;
    }
    cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

    return 0;
}