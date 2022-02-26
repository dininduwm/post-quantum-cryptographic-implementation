#include "Matrix.h"

using namespace std;
// typedef long long dtype;

// modulus function
dtype mod(dtype value, dtype mod_value)
{
    return ((value % mod_value) + mod_value) % mod_value;
}

// Matrix oparations =================================

// initializing the 2D matrix
dtype **initMatrix(dtype **A, int row, int col)
{
    A = new dtype *[row];
    for (int i = 0; i < row; i++)
        A[i] = new dtype[col];
    return A;
}

// matrix multiplication for threaded application
void matMulSegment(dtype **mat1, dtype **mat2, dtype **result, int r1_start, int r1_stop, int c, int r2, dtype q)
{

    for (int i = r1_start; i < r1_stop; i++)
    {
        for (int j = 0; j < r2; j++)
        {
            result[i][j] = 0;

            for (int k = 0; k < c; k++)
            {
                dtype mul = 0;
                if (mat1[i][k] && mat2[k][j])
                    mul = mat1[i][k] * mat2[k][j];
                    mul = mod(mul, q);
                    result[i][j] += mul;
                    result[i][j] = mod(result[i][j], q);
            }
        }
    }
}

// matrix multiplication and addition for threaded application
void matMulAddSegment(dtype **mat1, dtype **mat2, dtype **mat3, dtype **result, int r1_start, int r1_stop, int c, int r2, dtype q)
{

    for (int i = r1_start; i < r1_stop; i++)
    {
        for (int j = 0; j < r2; j++)
        {
            result[i][j] = mat3[i][j];

            for (int k = 0; k < c; k++)
            {
                dtype mul = mat1[i][k] * mat2[k][j];
                mul = mod(mul, q);
                result[i][j] += mul;
                result[i][j] = mod(result[i][j], q);
            }
        }
    }
}

// multiply matricies
void matMul(dtype **mat1, dtype **mat2, dtype **result, int r1, int c, int r2, dtype q)
{
    // note - resultent matrix should be initialized before calling this function
    //  initializing the thread matrix
    thread th1(matMulSegment, mat1, mat2, result, 0, r1 / 4, c, r2, q);
    thread th2(matMulSegment, mat1, mat2, result, r1 / 4, r1 / 2, c, r2, q);
    thread th3(matMulSegment, mat1, mat2, result, r1 / 2, 3 * r1 / 4, c, r2, q);
    thread th4(matMulSegment, mat1, mat2, result, 3 * r1 / 4, r1, c, r2, q);

    // joining the threads
    th1.join();
    th2.join();
    th3.join();
    th4.join();
}

// multiply matricies
void matMulAdd(dtype **mat1, dtype **mat2, dtype **mat3, dtype **result, int r1, int c, int r2, dtype q)
{
    // note - resultent matrix should be initialized before calling this function
    //  initializing the thread matrix
    thread th1(matMulAddSegment, mat1, mat2, mat3, result, 0, r1 / 4, c, r2, q);
    thread th2(matMulAddSegment, mat1, mat2, mat3, result, r1 / 4, r1 / 2, c, r2, q);
    thread th3(matMulAddSegment, mat1, mat2, mat3, result, r1 / 2, 3 * r1 / 4, c, r2, q);
    thread th4(matMulAddSegment, mat1, mat2, mat3, result, 3 * r1 / 4, r1, c, r2, q);

    // joining the threads
    th1.join();
    th2.join();
    th3.join();
    th4.join();
}