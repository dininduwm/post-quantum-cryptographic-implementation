#include <iostream>
#include <cstddef>
// #include <cstdlib>
// #include <cstdlib>


// typedef char byte;
namespace std
{
  // define std::byte
  enum class byte : unsigned char {};

};

short * binConvert(byte* input,int AESKeyLength); 