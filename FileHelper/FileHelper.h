#include <iostream>
#include <fstream>
#include"../AESHelper/AESHelper.h"

using namespace std;

// typedef long long dtype;
typedef int dtype;

// dump matrix to a file
// need to provide an opend ostream
void dumpMatrix(ofstream *fout, dtype **A, int rows, int cols);
// dump the hash to a file
// need to provide an opend ostream
void dumpHash(ofstream *fout, byte *array);
// dump array to a file
// need to provide an opend ostream
void dumpKey(ofstream *fout, union un key);

// load matrix from a file
// need to provide an opend istream
// load array from a file
dtype **loadMatrix(ifstream *fin, dtype **A, int rows, int cols);
// load hash from the file 
// need to provide an opend istream
byte* loadHash(ifstream *fin, byte* array);
// need to provide an opend istream
union un loadKey(ifstream *fin, union un key);