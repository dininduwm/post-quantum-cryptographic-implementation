#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include "sodium.h"
#include <chrono>

#define PI 3.14
#define q 12289
#define n 1024
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
    // double sigma = sqrt(1.75);
    // std::mt19937 gen(randombytes_random());    
    normal_distribution<double> gauss_dis{0,sigma};

    int count = 0;
    chrono::steady_clock sc;
    auto start = sc.now();
    while (count <100000)
    {
        gaussian(sigma);
        count++;
    }
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    cout<<"Operation took: "<<time_span.count()<<" seconds !!!";
    return 0;
}
