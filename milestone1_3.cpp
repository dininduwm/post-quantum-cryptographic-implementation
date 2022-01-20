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

