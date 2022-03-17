#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
    int p =10;
    float *q ;
    q = (float*)&p;

    cout<<sizeof(p)<<" "<<sizeof(q)<<endl;
    cout<<p<<" "<<*q<<endl;
    
    return 0;
}
