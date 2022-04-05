#include "AES.h"


unsigned char* encryptAES(AES_KEY enc_key, unsigned char key[], unsigned char iv[], const char * filename)
{	
	unsigned char* data = readFile(filename);

	unsigned long data_size;
	unsigned long out_size = ((data_size/AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
	unsigned char* encrypted = new unsigned char[out_size];

	for(int i=data_size; i<out_size; i++) 
		data[i] = (out_size-data_size);
    AES_set_encrypt_key(key, 128, &enc_key); // 128bit = 16byte
    AES_cbc_encrypt(data, encrypted,out_size, &enc_key, iv, AES_ENCRYPT);

    return encrypted;
}


unsigned char* decryptAES(AES_KEY enc_key, unsigned char encrypted[], unsigned char key[], unsigned char iv[], unsigned long data_size, const char * filename)
{	
  unsigned char* decrypted = new unsigned char[data_size];
  AES_set_decrypt_key(key, 128, &enc_key);
  //memset(iv, 0x01, AES_BLOCK_SIZE);
  AES_cbc_encrypt(encrypted, decrypted, data_size, &enc_key, iv, AES_DECRYPT);
  
  unsigned char tail = decrypted[data_size-1];

   
   int valid = (tail > 0 && tail<=AES_BLOCK_SIZE) ;

  	
  	for(int i=1;i<tail && valid;i++)
		valid = (decrypted[data_size-1-i] == tail);
	  writeFile(decrypted, data_size, filename);

  return decrypted;
}