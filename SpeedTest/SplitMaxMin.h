#include <iostream>
#include <cmath>
#include <cstdlib>
#include "BOBHash32.h"
using namespace std;

#define BUCKETNUMMAX 20000

class SplitMaxMin{    
public:
    int bucketNum;
    int noempty = 0;
    unsigned int Bucket[BUCKETNUMMAX];
    unsigned int Bucket2[BUCKETNUMMAX];
   
    class BOBHash32 HashFunc_a;
    class BOBHash32 HashFunc_b;
    class BOBHash32 HashFunc_c;

    SplitMaxMin(int _bucket, int seed);
    void input(const char *str);
    unsigned int hash(const char *str, int feq);
    double result(const SplitMaxMin & another);
    void modify();
    void share();
};

SplitMaxMin::SplitMaxMin(int _bucket, int seed)
{
    bucketNum = _bucket;
    for(int i = 0; i < bucketNum; i++) 
    {
        Bucket[i]  = UINT32_MAX;
        Bucket2[i] = 0;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

unsigned int SplitMaxMin::hash(const char *str, int feq)
{
    return ((unsigned int) (HashFunc_b.run(str, 4)));
}

void SplitMaxMin::input(const char *str)
{

    int num = 1;
    unsigned int hashresult = hash(str, num);
    unsigned int groupLen =  (UINT32_MAX / bucketNum);
    unsigned int bucketid = hashresult /groupLen;

    if(hashresult < Bucket[bucketid]) 
    {
        Bucket[bucketid] = hashresult;
    }
    if(hashresult > Bucket2[bucketid]) 
    {
        Bucket2[bucketid] = hashresult;
    }
}

inline void SplitMaxMin::share()
{
    for(int i = 0; i < bucketNum; i++) 
    {
        if(Bucket[i]!=UINT32_MAX)
        {
            continue;
        }
        else
        {   
            unsigned int friendID = (i+bucketNum/2)%bucketNum;
            if(Bucket2[friendID]!=0)
                Bucket[i] =Bucket2[friendID];
        }
    }
}

void SplitMaxMin::modify()
{
    share();
    unsigned int FullBucket[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) {
        if(Bucket[i] == UINT32_MAX) 
        {
            int attempt = 0;
            unsigned int next = (HashFunc_c.run((char *)(&attempt), 4)^ HashFunc_a.run((char *)(&i), 4))% bucketNum;
            while(Bucket[next] == UINT32_MAX) 
            {
                ++attempt;
                next = (HashFunc_c.run((char *)(&attempt), 4) ^ HashFunc_a.run((char *)(&i), 4))% bucketNum;
            }
            FullBucket[i] = Bucket[next];
        }
        else
        {
            FullBucket[i] = Bucket[i];
            noempty++;
        }
    }
    for(int i = 0; i < bucketNum; i++) 
        Bucket[i] = FullBucket[i]; 
}

double SplitMaxMin::result(const SplitMaxMin & another)
{
    double same = 0;
    for(int i = 0; i < bucketNum; i++) 
    {
        if(Bucket[i] == another.Bucket[i])
                ++same;
    }
    return same / bucketNum ;
}

