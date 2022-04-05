#include "AES.h"

int main(int argc, char const *argv[])
{
	   	unsigned char iv[16];
  	unsigned char key[16];
   	unsigned char receiverIV[16];
 		unsigned char receiverKey[16];
   	RAND_bytes(iv, 16);
  	RAND_bytes(key, 16);
	return 0;
}