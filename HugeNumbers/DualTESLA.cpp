/*Dual LWE CryptoSystem*/


// A is a public key

// Reciever genarates x

// Reciever sends A, u = Ax as public keys to the sender. 

//                              sender sends b' = s'A+e' (ciphertext -preamble)
//                              sender sends payload b' = s'u + e' + bit*q/2

//Reciever calculates b'-b'x. then decide 


#include <iostream>
#include <random>
#include <ctime>
#include "sodium.h"
#include "../Matrix/Matrix.h"
#include <cmath>

using namespace std;

// defining the parameters
dtype q = 2000;
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
    // Matrix<long, n, m> A;
    // Matrix<long, n, numberBits> u;
    dtype **A;
    dtype **u;
};

struct privateKey
{
    // Matrix<long, n, m> A;
    // Matrix<long, m, numberBits> x;
    dtype **A;
    dtype **x;
};

struct cipherText
{
    // Matrix<long, 1, m> bT;
    // Matrix<long, 1, numberBits> b_;
    dtype **bT;
    dtype **b_;
};

// genarate uniform random numbers including the boundaries
dtype genUniformRandomlong(dtype lowerBound, dtype upperBound)
{
    dtype range = (upperBound - lowerBound) + 1;
    dtype randomNumber;
    randomNumber = randombytes_uniform(range);
    dtype randomNumberModified = ((dtype)randomNumber) + lowerBound;
    return randomNumberModified;
}

dtype half(dtype q){
    if (q&1==1){
        return (q>>1)+1;
    }else{
        return q>>1;
    }
}

dtype gaussian(double sigma)
{

    mt19937 gen(randombytes_random());
    normal_distribution<double> gauss_dis(0, sigma);
    double val = gauss_dis(gen);
    if (val > 0.5)
        val = val - 1.0;
    else if (val < -0.5)
        val = val + 1;
    return (dtype)(val * q);
}

// function to genarate keys
void genarateKeys(privateKey* private_key, publicKey* public_key)
{
    // cout << "[LOG] Generating Matrix A" << endl;
    // Genarating the matrix A
    // long number = 0;
    // initializing matrix A
    public_key->A = initMatrix(public_key->A, n, m);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // filling the A matrix from the numbers taken from the distribution
            public_key->A[i][j] = genUniformRandomlong(0, q - 1);
        }
    }

    // avarage of A's coiff should be close to q/2
    // cout << "[DEBUG] Min of A : " << public_key->A.minCoeff() << " Max of A : " << public_key->A.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;

    // genarating the x matrix
    // cout << "[LOG] Generating Matrix x" << endl;
    // initializing number x
    private_key->x = initMatrix(private_key->x, m, numberBits);
    for (long row = 0; row < m; row++)
    {
        for (long col = 0; col < numberBits; col++)
        {
            private_key->x[row][col] =  genUniformRandomlong(0, 1);
        }

    }
    // cout << "[DEBUG] Min of s : " << private_key->x.minCoeff() << " Max of s : " << private_key->x.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;    
    

    // sharig A among public and private key
    private_key->A = public_key->A;
    //generate u as public key

    // initializing public ket u
    public_key->u = initMatrix(public_key->u, n, numberBits);
    // public_key->u = ((private_key->A)*(private_key->x));
    matMul(private_key->A, private_key->x, public_key->u, n, m, numberBits, q);
    
}

// Encrypting Function
cipherText encrypt(publicKey public_key, short message_bit[numberBits])
{
    struct cipherText cipher_text;

    // cout << "[LOG] Generating Matrix e" << endl;
    // initializing eT Matrix
    // Matrix<long, 1, m> eT;
    dtype **eT;
    eT = initMatrix(eT, 1, m);

    // long total = 0;
    double alpha = sqrt(double(n))/q;
    double sigma = alpha/sqrt(2*PI);

    for (long col = 0; col < m; col++)
    {
        // e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
        eT[0][col] = gaussian(sigma);
        // total += eT(col);
    }
    // cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

    // genarating the s matrix
    // initializing sT matrix
    // Matrix<long, 1, n> sT;
    dtype **sT;
    sT = initMatrix(sT, 1, n);

    // cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < n; col++)
    {
        sT[0][n] = genUniformRandomlong(0, q-1);
    }


    // cout << "[DEBUG] Min of s : " << sT.minCoeff() << " Max of s : " << sT.maxCoeff() << endl;
    // cout << "[LOG] Done" << endl;

    // initializing bT
    cipher_text.bT = initMatrix(cipher_text.bT, 1, m);
    // cipher_text.bT = sT * (public_key.A) + eT;
    matMulAdd(sT, public_key.A, eT, cipher_text.bT, 1, n, m, q);

    // initializing sTu
    // Matrix<long, 1, numberBits> sTu = sT * (public_key.u); 
    dtype **sTu;
    sTu = initMatrix(sTu, 1, numberBits);
    matMul(sT, public_key.u, sTu, 1, n, numberBits, q);

    // generate e'
    // Matrix<long, 1, numberBits> e_;
    
    // initializing b_
    cipher_text.b_ = initMatrix(cipher_text.b_, 1, numberBits);
    // calculating b_
    for(long col = 0; col < numberBits; col++)
    {
        cipher_text.b_[0][col] = mod((sTu[0][col] + gaussian(sigma) + (message_bit[col] * (half(q)))), q);
    }


    // cout<<abs(e_- eT*private_key.x)<<endl;
    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;
    return cipher_text;
}

// Decrypting Funciton
short* decrypt(privateKey private_key, cipherText cipher_text)
{
    // Matrix<long, 1, numberBits> bTx;
    dtype **bTx;
    // initializing bTx
    bTx = initMatrix(bTx, 1, numberBits);
    static short recovered[numberBits];

    // bTx = cipher_text.bT * private_key.x;
    matMul(cipher_text.bT, private_key.x, bTx, 1, m, numberBits, q);

    long difference = 0;

    for (long col = 0; col < numberBits; col++)
    {
        difference = mod(cipher_text.b_[0][col] - bTx[0][col], q);

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

    // cout << "Testing for message bits: ";
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
            message[i] = (short)genUniformRandomlong(0, 1);
        }
        //  cout << "iteration = " << i << endl;
        cipherText cipher_text = encrypt(public_key, message);
        short *recovered_message = decrypt(private_key, cipher_text);

        // cout << "Encrypted: " ;
        // printBits(message);
        // cout << " Decrypted: ";
        // printBits(recovered_message);

        if (checkAnswer(message, recovered_message))
        {
            success++;
            // cout << "  correct";
        } else {
            cout << "   incorrect";
        }

        // cout << endl;
    }
    cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

    return 0;
}