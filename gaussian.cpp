#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include "sodium.h"

#define PI 3.14
#define q 1000
#define n 30
#define m 270
using namespace std;


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

int main(int argc, char const *argv[])
{
    
    double alpha = sqrt(double(n))/q;
    double sigma = alpha/sqrt(2*PI);
    std::mt19937 gen(randombytes_random());    
    std::normal_distribution<double> gauss_dis{0,sigma};

    int count = 0;
    while (count <100000)
    {
        cout<<gaussian(sigma)<<endl;
        count++;
    }
       
    return 0;
}
