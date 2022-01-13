#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Matrix;

// defining the parameters
#define q 512
#define n 30
#define m 270
#define e_min -1
#define e_max 1

// Encrypting Function
void encrypt(Matrix<int,n,m>* A,Matrix<int,m,1>* b,char plain, int* cipher, Matrix<int,n,1>* u){
    // Genarating the X matrix with random values
    Matrix <int,m,1> X;
    for (int i = 0; i < m; i++)
    {
        X(i) = rand()%2;
    }
    // Taking the transpose of bb
    Matrix <int,1,m> bT = b->transpose();

    // u = AX
    // should take mod q
    *u = (*A)*X;
    // tempory varable for handling characters
    unsigned char shift = 1;
    int btx = (bT*X)%q;
    // change this to encrypt single bit message
    // itarating through the bits of the character and encrypting them
    for (int i = 0; i < 8; i++)
    {
        cipher[i] = (btx + ((plain&shift)>>i)*q/2)%q;
        shift = shift<<1;

    }
    // cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
    
    // should output u and cipher[0]

}

// Decrypting Funciton
char decrypt(Matrix<int,n,m> *A,Matrix<int,n,1> *u, int* cipher,Matrix<int,1,n> *s){
    // holding the recovered character
    char recovered = 0;
    // tempary variable to handdle the shifting oparation
    unsigned char shift = 1;
    int sTu = ((*s)*(*u))%q;
    int temp;

    // recovering the character step by step
    // recovering the single bit message
    for (size_t i = 0; i < 8; i++)
    {
        temp = (cipher[i]-sTu +q)%q;
        if (temp > q/4){ //bit is 1
            recovered  = recovered | shift; 
        }
        shift = shift<<1;
        
    }
    return recovered;    
}



int main(int argc, char const *argv[])
{  
    // A and s should be choose from uniform distribution over Zq(0, q-1)  
    srand(time(0));
    // defining randome number genarator with the seed as time
    default_random_engine generator;
    generator.seed(time(0));
    // strong random number genarator other than default random number genarator in c++

    // define a normal distribution to choose values 
    normal_distribution<double> distribution(64.0,16.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniform_dist(e_min, e_max);

    cout<<"[LOG] Generating Matrix A"<<endl;

    // Genarating the matrix A
    int number = 0;
    Matrix <int,n,m> A;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            // filling the A matrix from the numbers taken from the distribution
            A(i,j) = (int(distribution(generator)))%q;
        }      
    }

    // avarage of A's coiff should be close to q/2
    cout<<"[DEBUG] Min of A : "<<A.minCoeff()<<" Max of A : "<<A.maxCoeff()<<endl;
    cout<<"[LOG] Done"<<endl;
  
    cout<<"[LOG] Generating Matrix s"<<endl;

    // genarating the s matrix
    Matrix <int,1,n>s;
    for (int i = 0; i < n; i++)
    {
        s(i) = (int(distribution(generator)))%q;
    }
    cout<<"[LOG] Done"<<endl;
    
    
    cout<<"[LOG] Generating Matrix e"<<endl;
    // genarating the error matrix
    Matrix <int,1,m> e;
    int total = 0;
    for (int i = 0; i < m; i++)
    {   
        // e should be small and should choosen between -7,7 (discreate gausisan distribution [ignore for now])
        e(i) = uniform_dist(gen);
        total+=e(i);
    }
    cout<<"[DEBUG] min e: "<<e.minCoeff()<<" max e: "<<e.maxCoeff()<<" total :"<<total<<endl;

    // defining b and b transpose matrices
    Matrix <int,m,1> b;
    Matrix <int,1,m> bT;
    
    // calculating bT 
    bT = s*A + e ;
    b = bT.transpose();
    // public key -> bT and A
    // private key -> s and e
    
    // Testing 
    cout<<"A_rows :"<<A.rows()<<" A_cols :"<<A.cols()<<endl;
    cout<<"s_rows :"<<s.rows()<<" s_cols :"<<s.cols()<<endl;
    cout<<"e_rows :"<<e.rows()<<" e_cols :"<<e.cols()<<endl;
    cout<<"b_rows :"<<b.rows()<<" b_cols :"<<b.cols()<<endl;
    cout<<"b'_rows :"<<bT.rows()<<" b'_cols :"<<bT.cols()<<endl;
    
    // taking the modulus values of b
    int b_rows = b.rows();
    for (int i = 0; i < b_rows; i++)
    {
        b(i) = b(i)%q;
    }
    
    // sample character to test
    char plain = 's';
    // single bit
    int cipher[8];

    Matrix <int,n,1>u;
    encrypt(&A,&b,plain,cipher,&u);

    cout<<"plaintext letter :"<<plain<<" Recovered letter :"<<decrypt(&A,&u,cipher,&s)<<endl;

    // evaluating the success rate
    int success = 0;
    char result;
    for (size_t i = 0; i < 1000; i++)
    {
        encrypt(&A,&b,plain,cipher,&u);
        result = decrypt(&A,&u,cipher,&s);
        if(result == plain)
            success++;
    }
    cout<<"Encryption and Decryption works "<<success/10<<"% of time."<<endl;
        

    return 0;
}
