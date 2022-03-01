#include <iostream>
#include <random>
#include <ctime>
#include <cmath>
#include "sodium.h"
#include <chrono>
#include "./Helper/Helper.h"

#define PI 3.14
#define q 12289
#define n 1024
#define m 270
using namespace std;

int main(int argc, char const *argv[])
{
    
    double alpha = sqrt(double(n))/q;
    double sigma = alpha/sqrt(2*PI);
    // double sigma = sqrt(1.75);
    // std::mt19937 gen(randombytes_random());    
    // normal_distribution<double> gauss_dis{0,sigma};

    int count = 0;
    chrono::steady_clock sc;
    auto start = sc.now();
    while (count <100000)
    {
        gaussian(sigma,q);
        count++;
    }
    auto end = sc.now();
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    cout<<"Operation took: "<<time_span.count()<<" seconds !!!"<<endl;
    return 0;
}
