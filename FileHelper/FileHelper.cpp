#include"FileHelper.h"

using namespace std;

// dump matrix to a file
// need to provide an opend ostream
void dumpMatrix(ofstream *fout, dtype **A, int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        (*fout).write((char *)&A[row][0], sizeof(dtype) * cols);
    }
}

// dump the hash 
void dumpHash(ofstream *fout, byte *array) {
    (*fout).write((char *)&array[0], 32);
}

// dump array to a file
// need to provide an opend ostream
void dumpKey(ofstream *fout, union un key)
{
    (*fout).write((char *)&key, sizeof(key));
}

// load matrix from a file
// need to provide an opend istream
dtype **loadMatrix(ifstream *fin, dtype **A, int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        (*fin).read((char *)&A[row][0], sizeof(dtype) * cols);
    }
    return A;
}
// load hash from the file 
byte* loadHash(ifstream *fin, byte* array) {
    (*fin).read((char *)&array[0], 32);
    return array;
}
// load array from a file
// need to provide an opend istream
union un loadKey(ifstream *fin, union un key)
{
    (*fin).read((char *)&key, sizeof(key));
    return key;
}


