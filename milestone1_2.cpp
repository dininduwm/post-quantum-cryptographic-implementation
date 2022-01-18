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

long large = 0;

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
            // cout <<" [DEBUG] " <<  public_key->A(i, j) << endl;
        }
    }

    // avarage of A's coiff should be close to q/2
    cout << "[DEBUG] Min of A : " << public_key->A.minCoeff() << " Max of A : " << public_key->A.maxCoeff() << endl;
    cout << "[LOG] Done" << endl;

    // genarating the s matrix
    cout << "[LOG] Generating Matrix s" << endl;
    for (long col = 0; col < private_key->sT.cols(); col++)
    {
        private_key->sT(col) = uniform_dist_A_s(gen);
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
        eT(col) = uniform_dist(gen);
        total += eT(col);
    }
    cout << "[DEBUG] min e: " << eT.minCoeff() << " max e: " << eT.maxCoeff() << " total :" << total << endl;

    // calculating bT
    public_key->bT = (private_key->sT) * (public_key->A) + eT;

    // Testing
    cout << "A_rows :" << (public_key->A).rows() << " A_cols :" << (public_key->A).cols() << endl;
    cout << "s_rows :" << private_key->sT.rows() << " s_cols :" << private_key->sT.cols() << endl;
    cout << "e_rows :" << eT.rows() << " e_cols :" << eT.cols() << endl;
    cout << "b'_rows :" << public_key->bT.rows() << " b'_cols :" << public_key->bT.cols() << endl;

    // taking the modulus values of bT
    for (long col = 0; col < public_key->bT.cols(); col++)
    {
        public_key->bT(col) = ((public_key->bT(col) % q) + q)%q;
    }

    cout << "[DEBUG] Min of B : " << public_key->bT.minCoeff() << " Max of B : " << public_key->bT.maxCoeff() << endl;

    // sharig A among public and private key
    private_key->A = public_key->A;

    //debugging
    cout << endl << "A" << endl;
    for (long dr = 0; dr < private_key->A.rows(); dr++) {
        for (long dc = 0; dc < private_key->A.cols(); dc++) {
            cout << private_key->A(dr, dc) << " ";
        }
        cout << endl;
    }
    cout << endl << "eT" << endl;
    for (long dr = 0; dr < eT.rows(); dr++) {
        for (long dc = 0; dc < eT.cols(); dc++) {
            cout << eT(dr, dc) << " ";
        }
        cout << endl;
    }

    cout << endl << "bT" << endl;
    for (long dr = 0; dr < public_key->bT.rows(); dr++) {
        for (long dc = 0; dc < public_key->bT.cols(); dc++) {
            cout << public_key->bT(dr, dc) << " ";
        }
        cout << endl;
    }

    cout << endl << "sT" << endl;
    for (long dr = 0; dr < private_key->sT.rows(); dr++) {
        for (long dc = 0; dc < private_key->sT.cols(); dc++) {
            cout << private_key->sT(dr, dc) << " ";
        }
        cout << endl;
    }
}

// Encrypting Function
cipherText encrypt(publicKey public_key, long message_bit)
{
    struct cipherText cipher_text;
    // Genarating the X matrix with random values
    Matrix<long, m, 1> X;
    for (long i = 0; i < m; i++)
    {
        X(i) = rand() % 2;
    }
    // cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
    // u = AX
    // should take mod q
    cipher_text.u = (public_key.A) * X;
    // cout<<"[DEBUG] min of u : "<<cipher_text.u.minCoeff()<<" max of u : "<<cipher_text.u.maxCoeff()<<endl;
    long bTx = (((public_key.bT * X) % q)+q)%q;

    // encrypting the bit
    cipher_text.u_ = (((bTx + (message_bit * (q / 2))) % q)+q)%q;

    //cout<<"[DEBUG] u' : " <<cipher_text.u_ <<endl;

    // debugging
    cout << endl << "X" << endl;
    for (long dr = 0; dr < X.rows(); dr++) {
        for (long dc = 0; dc < X.cols(); dc++) {
            cout << X(dr, dc) << " ";
        }
        cout << endl;
    }

    cout << endl << "u" << endl;
    for (long dr = 0; dr < cipher_text.u.rows(); dr++) {
        for (long dc = 0; dc < cipher_text.u.cols(); dc++) {
            cout << cipher_text.u(dr, dc) << " ";
        }
        cout << endl;
    }

    cout << "u_ = " << cipher_text.u_ << endl;

    return cipher_text;
}

// Decrypting Funciton
long decrypt(privateKey private_key, cipherText cipher_text)
{

    long sTu = ((((private_key.sT) * (cipher_text.u)) % q)+q)%q;

    // recovering the single bit message
    long recovered = 1;
    if ((abs(cipher_text.u_ - sTu) % q) <= q / 4)
    { // bit is 1
        recovered = 0;
    }
    
    if(sTu > cipher_text.u_ )
    large++;

    //cout << "[DEBUG] sTu % : " << sTu << endl;
    //cout << "[DEBUG] Decypering before % : " << cipher_text.u_ - sTu << ". After % : " << (cipher_text.u_ - sTu) % q << endl;
    //cout << "[DEBUG] Recovered : " << recovered << endl;
    //cout << "\n" << endl;
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
    // encrypting the message
    // cipherText cipher_text = encrypt(public_key, message);
    // decrypting the message
    // long recovered_message = decrypt(private_key, cipher_text);
    // cout << "recovered : " << recovered_message << endl;

     // Matrix <long,n,1>u;
     // encrypt(&A,&b,plain,cipher,&u);

    // cout<<"plaintext letter :"<<plain<<" Recovered letter :"<<decrypt(&A,&u,cipher,&s)<<endl;

    // // evaluating the success rate
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
        else {
            cout << "recoverd message = " << recovered_message << endl;
            return 0;
        }
     }
     cout<<"Encryption and Decryption works "<<(success/rounds)* 100<<"% of time."<<endl;
     cout<<large<<endl;
    return 0;
}
