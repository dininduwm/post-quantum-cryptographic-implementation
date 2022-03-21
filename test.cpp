
#include <iostream>
#include <cstring>
typedef long long dtype;
using namespace std;

int main(int argc, char const *argv[])
{
    string str;
    int arr[5] = {2,4,6,8,10};
    memcpy(&str,&arr,sizeof(arr)+1);
    str[sizeof(arr)] = 0;
    vector<byte> plain;
    cout<<str;
    return 0;
}
