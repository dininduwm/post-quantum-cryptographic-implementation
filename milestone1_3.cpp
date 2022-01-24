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



// function to genarate keys
void genarateKeys(privateKey* private_key, publicKey* public_key)
{
    // A and s should be choose from uniform distribution over Zq(0, q-1)
    srand(time(0));
    // defining randome number genarator with the seed as time
    default_random_engine generator;
    generator.seed(time(0));
    // strong random number genarator other than default random number genarator in c++

    // define a normal distribution to choose values
    normal_distribution<double> distribution(64.0, 16.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniform_dist(e_min, e_max);
    std::uniform_int_distribution<> uniform_dist_A_s(0, q - 1);     // uniform distribution to A and s (0 to q - 1)
    

    cout << "[LOG] Generating Matrix A" << endl;

    // Genarating the matrix A
    long number = 0;
    for (long i = 0; i < n; i++)
    {
        for (long j = 0; j < m; j++)
        {
            // filling the A matrix from the numbers taken from the distribution
            public_key->A(i, j) = uniform_dist_A_s(gen);
        }
    }

    // avarage of A's coiff should be close to q/2
    cout << "[DEBUG] Min of A : " << public_key->A.minCoeff() << " Max of A : " << public_key->A.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;

    // genarating the s matrix
    cout << "[LOG] Generating Matrix x" << endl;
    for (long row = 0; row < private_key->x.rows(); row++)
    {
        private_key->x(row) = rand()%2;
    }
    cout << "[DEBUG] Min of s : " << private_key->x.minCoeff() << " Max of s : " << private_key->x.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;    
    

    // sharig A among public and private key
    private_key->A = public_key->A;
    //generate u as public key
    public_key->u = ((private_key->A)*(private_key->x));
    for (long row = 0; row < (public_key->u).rows(); row++)
    {
        public_key->u(row) = mod(public_key->u(row),q); 
    }
    
}

// Encrypting Function
cipherText encrypt(publicKey public_key, long message_bit)
{
    struct cipherText cipher_text;

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

    // genarating the s matrix
    Matrix<long, 1, n> sT;

    cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < sT.cols(); col++)
    {
        sT(col) = genUniformRandomLong(0, q-1);
    }
    cout << "[DEBUG] Min of s : " << sT.minCoeff() << " Max of s : " << sT.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;

    cipher_text.bT = sT * (public_key.A) + eT;

    long sTu = sT * (public_key.u); 
    cipher_text.b_ = mod((sTu + (message_bit * (q / 2))), q); // what is e'? eT * X??


    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
    return cipher_text;
}

// Decrypting Funciton
long decrypt(privateKey private_key, cipherText cipher_text)
{
    long bTx = mod(((cipher_text.bT) * (private_key.x)), q);

    // recovering the single bit message
    long recovered = 1;
    if ((abs(cipher_text.b_ - bTx) % q) <= q / 4)
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

