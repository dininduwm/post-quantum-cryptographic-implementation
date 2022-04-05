
#include "Regev.h"


int main(int argc, char const *argv[])
{
	struct privateKeyRegev private_key;
	struct publicKeyRegev public_key;
	genarateRegevKeys(&private_key, &public_key);
	short message_bit[numberBits];

	for (long j = 0; j < numberBits; j++)
		{
			message_bit[j] = (short)1;
		}

	cipherTextRegev test = RegevEncrypt( public_key,  message_bit);	
	unsigned char* ans = RegevDecrypt( private_key, test);

	for (long j = 0; j < numberBits; j++)
		{
			cout<<	(short)ans[j];
		}



	return 0;
}