#include <iostream>
#include "../Matrix/Matrix.h"
struct cipherTextRegev
{
    long *C1;
    long **c0;
    long *c1;
    long *c2T;
    long *c3T;
};
struct token
{
    long **D;
};

void de

int test(cipherTextRegev ctx1, cipherTextRegev ctx2, token d1, token d2)
{
    bool canParse = true;
    if (!canParse)
    {
        return -1;
    }
    long *v1T = ctx1.c3T - ctx1.c2T*d1.D;
    long *v2T = ctx2.c3T - ctx2.c2T * d2.D;

}

int main(int argc, char const *argv[])
{

    return 0;
}
