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
  dtype q = 1<<30;
  // unsigned long tmp1 = 1;
  // tmp1 = tmp1 << 31;
  // q = tmp1 - 19;
  // q = LLONG_MAX;
  // q = q>>5;
  long s = LONG_MAX;
  cout<<s<<endl;
  cout<<LLONG_MAX<<endl;
  cout<<INT32_MAX<<endl;
  cout<<sizeof(long long)<<endl;
  cout<<sizeof(long)<<endl;
  cout<<sizeof(int)<<endl;
  cout<<q<<endl;  
  cout<<hex<<q<<endl;
  // for (int i = 0; i < 1000000; i++)
  // {
  //   // printf(genUniformRandomlong(0,q-1));
  // }
  return 0;
}
