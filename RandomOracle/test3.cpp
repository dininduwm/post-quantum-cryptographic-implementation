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

using namespace std;
using namespace std::chrono;
using std::string;
using CryptoPP::byte;
using CryptoPP::SHA256;

// initial random arry size
#define initialRandomByteArraySize 10

// byte array to hols the hash values
byte *sigma;
// hash value
byte *hashBytes = new byte[32];

// do the hashing to a byte array
void byteHash(byte *message, int size, byte *output)
{
    /*
    inputs:
        message : a byte array for message to hash
        size    : size of the message
        output  : a byte array for message to output
    */
    SHA256 hash;
    hash.Update(message, size);
    hash.Final(output);
}

// do the hashing for a string
void stringHash(string message, byte *output)
{
    // This will take string as the input and generate a hash value.
    std::cout << message << std::endl;
    SHA256 hash;
    hash.Update((const byte *)message.data(), message.size());
    std::cout << hash.DigestSize() << std::endl;
    hash.Final(output);
}

// concatinating string and byte array to a String
void appendBytesToString(string &str, byte *array, size_t num_bytes)
{
    // This will concatanate the sigma(random key) and a given message in string format
    str.append((char *)array, num_bytes);
}

// initialize random number genaration with hash functions
byte* initHash(byte *initalByteArray, byte *hashBytes, string message) {
    // initializing new byte array
    initalByteArray = new byte[initialRandomByteArraySize];
    randombytes_buf(initalByteArray, initialRandomByteArraySize);

    // concatinate the byte array with message
    string initialString;
    appendBytesToString(initialString, initalByteArray, initialRandomByteArraySize);
    cout << initialString << endl;
    // initial hash
    stringHash(initialString, hashBytes);
    return hashBytes;
}

//full with random binary numbers
void fillWithRandomBinary(dtype **mat, short row, short col) {
    int numberOfPositions = row*col;
    int positionCounter = 0;

    int rowPos;
    int colPos;
    int byteArrayPos;
    int byteArraySegment;
    int byteArraySegmentPos;

    // looping through the positions
    while (positionCounter < numberOfPositions) {
        if (positionCounter%256 == 0) {
            byteHash(hashBytes, sizeof(hashBytes), hashBytes);
        }

        // calculating the positions
        rowPos = positionCounter / col;
        colPos = positionCounter % col;
        byteArrayPos = positionCounter % 256;
        byteArraySegment = byteArrayPos / 8;
        byteArraySegmentPos = byteArrayPos % 8;

        cout << rowPos << ", " << colPos << ", " << byteArraySegment << ", " << byteArraySegmentPos << endl;
        // filing the matrix
        mat[rowPos][colPos] = hashBytes[byteArraySegment] % (1 << byteArraySegmentPos);

        positionCounter++;
    }
}

int main () {

    // genarate random numbers array with sodium
    string message = "Hello";
    hashBytes = initHash(sigma, hashBytes, message);

    // matrix
    dtype **mat;
    mat = initMatrix(mat, 5, 10); 
    fillWithRandomBinary(mat, 5, 10);

    cout << 7/4 << endl;
    return 0;
}