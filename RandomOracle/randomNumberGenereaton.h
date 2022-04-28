#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <random>
#include <ctime>
#include "sodium.h"
#include "../Matrix/Matrix.h"
#include "../cryptopp/aes.h"
#include "../cryptopp/hex.h"
#include "../cryptopp/files.h"
#include "../cryptopp/sha.h"

// do the hashing to a byte array
void byteHash(byte *message, int size, byte *output);

// do the hashing for a string
void stringHash(string message, byte *output);

// concatinating string and byte array to a String
void appendBytesToString(string &str, byte *array, size_t num_bytes);

// initialize random number genaration with hash functions
byte *initHash(byte *initalByteArray, byte *hashBytes, string message);

// fill with random binary numbers
void fillWithRandomBinary(dtype **mat, short row, short col, byte *hashBytes);

// fill with random dtype numbers
void fillWithRandomDtype(dtype **mat, short row, short col, byte *hashBytes);

void printMatrix(dtype **mat, int row, int col);