// #include "sodium.h"
#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

#define q 1000
#define n 30
#define PI 3.14

long discrete_gaussian_generator(double sigma){
    
    normal_distribution<double> distribution(0,sigma);
    srand(time(0));
    default_random_engine generator;
    generator.seed(time(0));
    double x = distribution(generator)%1;
    cout<<x;
    long y = int(ceil(q*x + 0.5)%q);
    cout<<y<<endl;
    return y;

}

int main(int argc, char const *argv[])
{
    double alpha = sqrt(n)/q;
    double sigma = alpha/(2*PI);
    long x = discrete_gaussian_generator(sigma);
    return 0;
}


