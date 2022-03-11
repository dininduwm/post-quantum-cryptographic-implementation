#include <iostream>
#include "sodium.h"
#include <random>
using namespace std;
int main(int argc, char const *argv[])
{
    // long lseed =0L;
    // lseed = randombytes_random();
    // lseed = (lseed<<32)|randombytes_random();
    // cout<<lseed<<" "<<sizeof(lseed)<<endl;
    int seed = 519202011;
    // lseed = 2169037968223587073;
    mt19937 gen(seed);
    for (size_t i = 0; i < 10; i++)
    {
        cout<<gen()<<" ";
    }
    cout<<endl;
    cout<<"what it should like :"<<endl;
    cout<<"810441992 769165511 159291061 274989660 3361614168 4095262316 1216709751 3039484016 441964121 3799439008"<<endl;
    return 0;
}
