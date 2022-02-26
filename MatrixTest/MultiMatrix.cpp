// CPP program to implement Strassen’s Matrix
// Multiplication Algorithm
#include <iostream>
#include "sodium.h"
#include <thread>
#include <chrono>

using namespace std::chrono;
using namespace std;
typedef long long lld;

lld upperBound = 50;

#define nval 1000
#define mval 800
#define lval 1000

// maximum number of threads
#define MAX_THREAD 4

// normal matrix multiplication
void mulMatNormal(lld** mat1, lld** mat2, lld** mat3, int n, int m, int l) {
 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < l; j++) {
            mat3[i][j] = 0;
 
            for (int k = 0; k < m; k++) {
                mat3[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// matrix multiplication for threaded application
void mulMatNormalMulti(lld** mat1, lld** mat2, lld** mat3, int n_start, int n_stop, int m, int l) {
 
    for (int i = n_start; i < n_stop; i++) {
        for (int j = 0; j < l; j++) {
            mat3[i][j] = 0;
 
            for (int k = 0; k < m; k++) {
                mat3[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

lld genUniformRandomlong(lld lowerBound, lld upperBound)
{
    lld range = (upperBound - lowerBound) + 1;
    lld randomNumber;
    randomNumber = randombytes_uniform(range);
    lld randomNumberModified = ((lld)randomNumber) + lowerBound;
    return randomNumberModified;
}

// initializing the 2D matrix
lld** initMatrix(lld** A, int n, int m) {
    A = new lld*[n];
	for (int i = 0; i < n; i++)
		A[i] = new lld[m];
    return A;
}

void fillWithRandom(lld** A, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            A[i][j] = genUniformRandomlong(0, upperBound);
        }
    }
}

void printMatrix(lld** A, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool checkAnswer(lld** matC,lld** matD) {
    for (int i = 0; i < nval; i++) {
        for (int j = 0; j < lval; j++) {
            if (matC[i][j] != matD[i][j]) {
                return false;
            }
        }
    }
    return true;
}



int main()
{
    int n = nval;
    int m = mval;
    int l = lval;


	lld** matA;
    matA = initMatrix(matA, n, m);
    fillWithRandom(matA, n, m);
    // printMatrix(matA, n, m);

	lld** matB;
    matB = initMatrix(matB, m, l);
    fillWithRandom(matB, m, l);
    // printMatrix(matB, m, l);

    // matrix multiplication without threads
    lld** matD;
    matD = initMatrix(matD, n, l);

    auto start = high_resolution_clock::now();
    mulMatNormal(matA, matB, matD, n, m, l);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Without Thread function = " << duration.count()<< endl;

    // starting threaded applicaition
    lld** matC;
    matC = initMatrix(matC, n, l);
    

    start = high_resolution_clock::now();
    // initializing the thread matrix
    thread th1(mulMatNormalMulti, matA, matB, matC, 0, n/4, m, l);
    thread th2(mulMatNormalMulti, matA, matB, matC, n/4, n/2, m, l);
    thread th3(mulMatNormalMulti, matA, matB, matC, n/2, 3*n/4, m, l);
    thread th4(mulMatNormalMulti, matA, matB, matC, 3*n/4, n, m, l);

    // joining the threads
    th1.join();
    th2.join();
    th3.join();
    th4.join();

    stop = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>(stop - start);
    cout << "with Thread function = " << duration2.count() << endl;
    double speed = (duration.count()-duration2.count())*100/duration.count();
    cout << speed << "% faster" << endl;

    if (checkAnswer(matC, matD)) {
        cout << "MultiThread Correct" << endl;
    } else {
        cout << "MultiThread Wrong" << endl;
    }


    // printMatrix(matD, n, l);
	return 0;
}