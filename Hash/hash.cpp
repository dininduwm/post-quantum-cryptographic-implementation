#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <openssl/sha.h>
unsigned long data_size;

unsigned char* readFile(const char * filename)
{  
    unsigned char* data = new unsigned char[2000000];
    
    FILE *fp = fopen(filename, "rb");
    int c;
    
    if(fp == NULL)
    {
        fprintf(stderr,"file input error.\n");
        return NULL;    
    } 
  

    for(data_size=0; data_size < 2000000 && (c=fgetc(fp))!=EOF; data_size++) 
    {
        data[data_size] = c;
    }
        
    fclose(fp);

    return data;
}


string sha256(unsigned char array[])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, array, strlen((char*)array));
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

int main() 
{
    unsigned char* data = readFile("1.txt");
    
    cout << sha256(data) << endl;

    unsigned char* data2 = readFile("11.txt");
    
    cout << sha256(data2) << endl;
    return 0;
}