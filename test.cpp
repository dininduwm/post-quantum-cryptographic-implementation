#include <iostream>
#include "Eigen/Dense"
#include <random>
#include <ctime>

using namespace std;
using Eigen::Matrix;
using Eigen::MatrixXd;

int main(int argc, char const *argv[]) {
    for (int i = 0; i < 10000; i++) {
        cout << rand()%2 << endl;
    }
}