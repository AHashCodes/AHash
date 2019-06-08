#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include "BOBHash32.h"
using namespace std;
#define inf 0xffffffff
#define MAXHASHNUM 10000


class MinHash{
    
    public:
    MinHash(){};
    ~MinHash();
    MinHash(int _hashnum, int seed);
    void init(int _hashnum,int seed);
    void input(const char *str);
    double result(const MinHash& another);
    double Standard_result();
    vector<int> outputvec(int b);
    char* outputsign(int b);
    int* outputintsign(int b);
    void clear();
    private:

    int Hashnum;
    class BOBHash32* Hashfunclist;
    unsigned int* minValue;

};

MinHash::MinHash(int _hashnum, int seed)
{
    Hashnum = _hashnum;
    Hashfunclist=new class BOBHash32[MAXHASHNUM];
    minValue=new unsigned int[MAXHASHNUM];
    for(int i = 0; i < Hashnum; i++) {
        Hashfunclist[i].initialize(seed*3+i);
        minValue[i] = inf;
    }
}

MinHash::~MinHash()
{
    delete []Hashfunclist;
    delete []minValue;
}

void MinHash::init(int _hashnum, int seed)
{
    Hashnum = _hashnum;
    Hashfunclist=new class BOBHash32[MAXHASHNUM];
    minValue=new unsigned int[MAXHASHNUM];
    for(int i = 0; i < Hashnum; i++) {
        Hashfunclist[i].initialize(seed*3+i);
        minValue[i] = inf;
    }
}

void MinHash::clear()
{
    for(int i=0;i<Hashnum;i++)
    { minValue[i]=inf; }
}

void MinHash::input(const char *str)
{
    for(int i = 0 ; i < Hashnum; i++) {
        unsigned int tmp = Hashfunclist[i].run(str, 4);
        if (tmp < minValue[i]) minValue[i] = tmp;
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
vector<int> MinHash::outputvec(int b)
{
    vector<int> output_vec;
    int spreadlen=pow(2,b)-1;
    int lowbiter=0;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<Hashnum;i++)
    {
        int lowbit=lowbiter&minValue[i];
        for(int j=spreadlen;j>=0;j--)
        {
            if(lowbit==j)
            {output_vec.push_back(1);}
            else
            {output_vec.push_back(0);}
        }
    }
    return output_vec;
}

char* MinHash::outputsign(int b)
{
    if(b*Hashnum%8!=0)
    {
        printf("8 need to be divisible by b*k\n");
        return NULL;
    }
    if(8%b!=0)
    {
        printf("b need to be divisible by 8\n");
        return NULL;
    }
    char* signature=new char[b*Hashnum/8];
    int spreadlen=pow(2,b)-1;
    int lowbiter=0;
    int partnum=8/b;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<b*Hashnum/8;i++)
    {
        signature[i]=0;
    }
    for(int i=0;i<Hashnum;i++)
    {
        int lowbit=lowbiter&minValue[i];
        int signindex=i/partnum;
        int partindex=i%partnum;
        signature[signindex]|=(lowbit<<(b*(partnum-1-partindex)));
    }
    return signature;
}

int* MinHash::outputintsign(int b)
{
    int* signature=new int[Hashnum];
    int lowbiter=0;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<Hashnum;i++)
    {
        signature[i]=0;
    }
    for(int i=0;i<Hashnum;i++)
    {
        int lowbit=lowbiter&minValue[i];
        signature[i]=lowbit;
    }
    return signature;
}
