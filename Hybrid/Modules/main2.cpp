#include "AES.h"

int main(int argc, char const *argv[])
{   
	

	double rounds = 1;
	double success = 0;
	struct AESPayload AES;	
	AES_KEY aesKey, aesKey2;
	unsigned char iv[16];
  	unsigned char key[16];
   	unsigned char receiverIV[16];
 	unsigned char receiverKey[16];

 	cout << 1 << endl;

   	RAND_bytes(iv, 16);
  	RAND_bytes(key, 16);
  	short *binKey = binConvert(key);
	short *binIV = binConvert(iv);



	struct privateKeyRegev private_key;
	struct publicKeyRegev public_key;
	genarateRegevKeys(&private_key, &public_key);

	AES.AESKey = RegevEncrypt(public_key, binKey);
	AES.AESIv = RegevEncrypt(public_key, binIV);

	unsigned char *recovered_AESKey = RegevDecrypt(private_key, AES.AESKey);
	unsigned char *recovered_AESIv = RegevDecrypt(private_key, AES.AESIv);

	cout << 2 << endl;

	AES.AESCipherText = encryptAES(aesKey, key, iv, argv[1]);


	unsigned char* decrypted = decryptAES(aesKey2, recovered_AESKey, recovered_AESIv, "out.txt");
	
	struct InputFile data = readFile(argv[0]);
	if (checkAnswer(data.data, decrypted))
        {
            success++;
        }

    delete[] recovered_AESKey; delete[] recovered_AESIv; delete[] binKey; delete[] binIV; 

    cout << "Encryption and Decryption works " << (success / rounds) * 100 << "% of time." << endl;

		cout << 1 << endl;
	return 0;
}