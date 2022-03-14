#include "crypto++/aes.h"
#include "crypto++/modes.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <string>
#include <cstdlib>
#include "crypto++/cryptlib.h"
#include "crypto++/hex.h"
#include "crypto++/filters.h"
#include "crypto++/ccm.h"
#include "crypto++/files.h"
#include "assert.h"
#include "crypto++/osrng.h"
#include <iostream>
#include <random>
#include <ctime>
// #include "sodium.h"
#include <cmath>

using namespace std;
using std::string;
using std::exit;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using std::cout;
using std::cerr;
using std::endl;
using CryptoPP::CTR_Mode;
using CryptoPP::AES;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Exception;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Redirector;
using CryptoPP::byte;
using CryptoPP::MeterFilter;
using CryptoPP::word64;
using CryptoPP::FileSource;
using CryptoPP::FileSink;

inline bool EndOfFile(const FileSource& file)
{
  std::istream* stream = const_cast<FileSource&>(file).GetStream();
  return stream->eof();
}

int main(int argc, char* argv[])
{
  try
  {
      byte key[AES::DEFAULT_KEYLENGTH]={}, iv[AES::BLOCKSIZE]={};
      CTR_Mode<AES>::Encryption encryptor;
      encryptor.SetKeyWithIV(key, sizeof(key), iv);

      MeterFilter meter;
      StreamTransformationFilter filter(encryptor);
     
      FileSource source("plain.bin", false);
      FileSink sink("cipher.bin");
 
      source.Attach(new Redirector(filter));
      filter.Attach(new Redirector(meter));
      meter.Attach(new Redirector(sink));

      const word64 BLOCK_SIZE = 4096;
      word64 processed = 0;
	  
      while(!EndOfFile(source) && !source.SourceExhausted())
      {
        source.Pump(BLOCK_SIZE);
        filter.Flush(false);

        processed += BLOCK_SIZE;

        if (processed % (1024*1024*10) == 0)
          cout << "Processed: " << meter.GetTotalBytes() << endl;
      }
  
      // Signal there is no more data to process.
      // The dtor's will do this automatically.
      filter.MessageEnd();
  }
  catch(const Exception& ex)
  {
    cerr << ex.what() << endl;
  }

  return 0;
}