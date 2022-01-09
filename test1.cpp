#include <iostream>
#include "Eigen/Dense"
#include <random>

using namespace std;
using Eigen::MatrixXd;
using Eigen::Matrix;

#define q 64
#define n 30
#define m 180
int* encrypt(MatrixXd A,MatrixXd b,char plain){
    int* cipher;
    cipher = (int*)malloc(sizeof(int)*8);
    //generate binary vector X.
    //bitwise encription to create new char u'.
    return cipher;
}

char decrypt(MatrixXd A,MatrixXd u, int cipher[]){
    char plain = 0;  
    return plain;
}



int main(int argc, char const *argv[])
{    
    default_random_engine generator;
    normal_distribution<double> distribution(32.0,15.0);
    //generate A

    int number = 0;
    Matrix <int,n,m> A;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            A(i,j) = (int(distribution(generator)))%q;
        }
        
    }
    
    // cout<<"done"<<A<<endl;

    //generate s
    Matrix <int,1,n>s;
    //populate s

    for (int i = 0; i < n; i++)
    {
        s(i) = (int(distribution(generator)))%q;
    }

    
    //generate e
    Matrix <int,1,m> e;
    for (int i = 0; i < m; i++)
    {
        e(i) = rand()%3;
    }

    // cout<<s<<'\n'<<e<<endl;
    //calculate b


    Matrix <int,m,1> b;
    Matrix <int,1,m> bT;
    //b = s`* A + e`
    bT = s*A + e ;
    b = bT.transpose();
    //+ e.transpose();
    cout<<"A_rows :"<<A.rows()<<" A_cols :"<<A.cols()<<endl;
    cout<<"s_rows :"<<s.rows()<<" s_cols :"<<s.cols()<<endl;
    cout<<"e_rows :"<<e.rows()<<" e_cols :"<<e.cols()<<endl;
    cout<<"b_rows :"<<b.rows()<<" b_cols :"<<b.cols()<<endl;
    cout<<"b'_rows :"<<bT.rows()<<" b'_cols :"<<bT.cols()<<endl;
    // cout<<b<<endl;
    int b_rows = b.rows();
    for (int i = 0; i < b_rows; i++)
    {
        b(i) = b(i)%q;
    }
    //cout<<b<<endl;



    //We have A,s,e,b

    // encryption:



    //generate X
    Matrix <int,m,1> X;
    for (int i = 0; i < m; i++)
    {
        X(i) = rand()%2;
    }

    Matrix <int,n,1>u = A*X;
    
    cout<<"X_rows :"<<X.rows()<<" X_cols :"<<X.cols()<<endl;
    cout<<"u_rows :"<<u.rows()<<" u_cols :"<<u.cols()<<endl;

    char plain = 's';
    int cipher[8];
    unsigned char shift = 1;
    int btx = (bT*X)%q;
    cout<<"char"<<sizeof(plain)<<" shift: "<<sizeof(shift)<<endl;
    cout<<"bTx : "<<btx<<endl;

    for (int i = 0; i < 8; i++)
    {
        cout<<(plain&shift)<<endl;
        cipher[i] = (btx + ((plain&shift)>>i)*q/2)%q;
        shift = shift<<1;

    }
    for (int i = 0; i < 8; i++)
    {
        cout<<cipher[i]<<" ";
    }
    cout<<endl;

    cout<<"eTx"<<(e*X)%q<<endl;
    //decryptor has u and u'(cipher)

    int sTu = (s*u)%q;
    cout<<sTu<<endl;
    char recovered = 0;
    shift = 1;
    for (size_t i = 0; i < 8; i++)
    {
        // cout<<(cipher[i]-sTu +q)%q<<" ";
        if (((cipher[i] - sTu)+q)%q > q/4){ //bit is 1
            recovered  = recovered | shift; 
            cout<<1;
        }else{
            cout<<0;
        }
        shift = shift<<1;
    }
    cout<<'\n'<<"recovered text"<<recovered<<endl;
    

    cout<<"plaintext : "<<plain<<endl;
    

    




    return 0;
}
