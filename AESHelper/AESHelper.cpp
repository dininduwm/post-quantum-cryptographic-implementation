#include"AESHelper.h"
using namespace std;

short * binConvert(byte* input,int AESKeyLength) 
{

  short*  bitstream = new short[AESKeyLength * 8];

  
  for (int i = 0; i < AESKeyLength; ++i)
   {
	for (int j = 0; j < 8; ++j) 
		{
		  bitstream[ (i*8 - j + 7)] = (input[i] >> j) & 1;    
		}
   } 
   
   return bitstream;
}