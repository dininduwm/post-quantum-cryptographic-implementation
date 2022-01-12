#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Matrix;

#define q 128
#define n 30
#define m 180

void encrypt(Matrix<int,n,m>* A,Matrix<int,m,1>* b,char plain, int* cipher, Matrix<int,n,1>* u){
    Matrix <int,m,1> X;
    Matrix <int,1,m> bT = b->transpose();
    for (int i = 0; i < m; i++)
    {
        X(i) = rand()%2;
    }
    *u = (*A)*X;
    unsigned char shift = 1;
    int btx = (bT*X)%q;
    for (int i = 0; i < 8; i++)
    {
        cipher[i] = (btx + ((plain&shift)>>i)*q/2)%q;
        shift = shift<<1;

    }
    // cout<<"[DEBUG] min of X : "<<X.minCoeff()<<" max of X : "<<X.maxCoeff()<<endl;
    
}

char decrypt(Matrix<int,n,m> *A,Matrix<int,n,1> *u, int* cipher,Matrix<int,1,n> *s){
    char recovered = 0;
    unsigned char shift = 1;
    int sTu = ((*s)*(*u))%q;
    int temp;
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
    srand(time(0));
    default_random_engine generator;
    generator.seed(time(0));
    normal_distribution<double> distribution(64.0,16.0);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> uniform_dist(-7,7);

    cout<<"[LOG] Generating Matrix A"<<endl;
    int number = 0;
    Matrix <int,n,m> A;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            A(i,j) = (int(distribution(generator)))%q;
        }      
    }

    cout<<"[DEBUG] Min of A : "<<A.minCoeff()<<" Max of A : "<<A.maxCoeff()<<endl;
    cout<<"[LOG] Done"<<endl;
  
    cout<<"[LOG] Generating Matrix s"<<endl;
    Matrix <int,1,n>s;

    for (int i = 0; i < n; i++)
    {
        s(i) = (int(distribution(generator)))%q;
    }

    cout<<"[LOG] Done"<<endl;
    
    cout<<"[LOG] Generating Matrix e"<<endl;
    
    Matrix <int,1,m> e;
    int total = 0;
    for (int i = 0; i < m; i++)
    {   
        e(i) = uniform_dist(gen);
        total+=e(i);
    }
    cout<<"[DEBUG] min e: "<<e.minCoeff()<<" max e: "<<e.maxCoeff()<<" total :"<<total<<endl;

    Matrix <int,m,1> b;
    Matrix <int,1,m> bT;
    
    bT = s*A + e ;
    b = bT.transpose();
    
    cout<<"A_rows :"<<A.rows()<<" A_cols :"<<A.cols()<<endl;
    cout<<"s_rows :"<<s.rows()<<" s_cols :"<<s.cols()<<endl;
    cout<<"e_rows :"<<e.rows()<<" e_cols :"<<e.cols()<<endl;
    cout<<"b_rows :"<<b.rows()<<" b_cols :"<<b.cols()<<endl;
    cout<<"b'_rows :"<<bT.rows()<<" b'_cols :"<<bT.cols()<<endl;
    
    int b_rows = b.rows();
    for (int i = 0; i < b_rows; i++)
    {
        b(i) = b(i)%q;
    }
    
    char plain = 's';
    int cipher[8];

    Matrix <int,n,1>u;
    encrypt(&A,&b,plain,cipher,&u);

    cout<<"plaintext letter :"<<plain<<" Recovered letter :"<<decrypt(&A,&u,cipher,&s)<<endl;

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
