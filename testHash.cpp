#include <string>
#include "Hash/hash.h"
#include <iostream>
using namespace std;
using std::string;
unsigned long data_size;


unsigned char* readFile(const char * filename)
{  
	unsigned char* data = new unsigned char[2000000];
	
	FILE *fp = fopen("1.txt","rb");
	int c;

	if(fp == NULL)
	{
		fprintf(stderr,"file input error.\n");
		return NULL;	
	} 
		

	for(data_size=0; data_size<2000000 && (c=fgetc(fp))!=EOF; data_size++) 
	{
		data[data_size] = c;
	}
	fclose(fp);
//cout << "read file: "<< data_size << endl;
	return data;
}

 int main(int argc, char const *argv[])
{
	unsigned char* data1 = readFile("1.txt");
	string hash1 = sha256(data1);

	unsigned char* data2 = readFile("11.txt");
	string hash2 = sha256(data2);

	cout<< compareHash(hash1, hash2) << endl;
	cout<< hash1 << endl;
	cout<< hash2 << endl;

	return 0;
}