#include <climits>
#include <iostream>
#include "sodium.h"

typedef long long dtype;
using namespace std;
dtype genUniformRandomlong(dtype lowerBound, dtype upperBound)
{
    dtype range = (upperBound - lowerBound) + 1;
    dtype randomNumber;
    randomNumber = randombytes_uniform(range);
    dtype randomNumberModified = ((dtype)randomNumber) + lowerBound;
    return randomNumberModified;
}

int main(int argc, char const *argv[])
{
  // dtupe temp = 1
  size_t x = 1;
  void * buf;
  const unsigned char seed[1] = {'a'};
  // void randombytes_buf_deterministic(void * const buf, const size_t size,
  //                                  const unsigned char seed[randombytes_SEEDBYTES]);
  cout<<randombytes_SEEDBYTES<<endl;
  randombytes_buf_deterministic();
  return 0;
}
