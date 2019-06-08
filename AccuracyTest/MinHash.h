#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include "BOBHash32.h"
using namespace std;

#define MAXHASHNUM 10000


class MinHash{
    
    public:

    MinHash(int _hashnum, int seed);
    void input(const char *str);
    double result(const MinHash& another);
    double Standard_result();

    private:

    int Hashnum;
    class BOBHash32 Hashfunclist[MAXHASHNUM];
   
    unsigned int minValue[MAXHASHNUM];

};

MinHash::MinHash(int _hashnum, int seed)
{
    Hashnum = _hashnum;
    for(int i = 0; i < Hashnum; i++) {
        Hashfunclist[i].initialize(seed*3+i);
        minValue[i] = inf;
    }
}

void MinHash::input(const char *str)
{
    for(int i = 0 ; i < Hashnum; i++) {
        unsigned int tmp = Hashfunclist[i].run(str, 4);
        //if (tmp < minValue[i]) minValue[i] = tmp;
        minValue[i] = (tmp < minValue[i]) ? tmp : minValue[i];
    }
}
double MinHash::result(const MinHash& another)
{
    double eq = 0;
    for(int i = 0; i < Hashnum ; i++) {
        if(minValue[i] == another.minValue[i])
        ++eq;
    }
    return eq / Hashnum;
}
