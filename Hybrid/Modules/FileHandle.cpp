#include "FileHandle.h"

unsigned char* readFile(const char * filename)
{  
	unsigned long data_size;
	unsigned char* data = new unsigned char[2000000];
	
	FILE *fp = fopen(filename ,"rb");
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

int writeFile(unsigned char out[], unsigned long out_size, const char * filename)
{
	FILE *fp = fopen(filename,"wb");

	if(fp == NULL)
	{
		fprintf(stderr,"file output error.\n");
		return -1;	
	}
		
//cout << "write file: "<< out_size << endl;
	int i = 0;
	while(out[i] != '\0')
	{
		fputc(out[i], fp);
		i++;
	}
		
	fclose(fp);

	return 0;
}