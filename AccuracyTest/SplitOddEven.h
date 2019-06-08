#include <iostream>
#include <cmath>
#include <cstdlib>
#include "BOBHash32.h"
using namespace std;

#define BUCKETNUMMAX 20000

class SplitOddEven{    
    public:

    int bucketNum;
    int noempty = 0;
    unsigned int Bucket[BUCKETNUMMAX];
    unsigned int Bucket2[BUCKETNUMMAX];
   
    class BOBHash32 HashFunc_a;
    class BOBHash32 HashFunc_b;
    class BOBHash32 HashFunc_c;

    SplitOddEven(int _bucket, int seed);
    unsigned int hash(const char *str, int feq);
    void input(const char *str);
    void share();
    void modify();
    double result(const SplitOddEven & another);
    
};

SplitOddEven::SplitOddEven(int _bucket, int seed)
{
    bucketNum = _bucket;
    for(int i = 0; i < bucketNum; i++) 
    {
        Bucket[i]  = Bucket2[i] = inf;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

unsigned int SplitOddEven::hash(const char *str, int feq)
{
    return ((unsigned int) (HashFunc_b.run(str, 4)));
}

void SplitOddEven::input(const char *str)
{

    int num = 1;
    unsigned int hashresult = hash(str, num);
    unsigned int groupLen =  (UINT32_MAX / bucketNum);
    unsigned int bucketid = hashresult /groupLen;

    if(hashresult & 16)
    {
        if(hashresult < Bucket[bucketid]) 
        {
            Bucket[bucketid] = hashresult;
        }
    }
    else
    {
        if(hashresult < Bucket2[bucketid]) 
        {
            Bucket2[bucketid] = hashresult;
        }
    }
}

void SplitOddEven::share()
{
	for (int i = 0; i < bucketNum; i++)
	{
		if (Bucket[i] != inf)
		{
			if (!(i % 2) && Bucket2[i] != inf)
				Bucket[i] = Bucket2[i];
		}
		else
		{
			if (Bucket2[i] != inf)
				Bucket[i] = Bucket2[i];
			else
			{
				unsigned int friendID = (i + bucketNum / 2) % bucketNum;
				if (!(i % 2))
					Bucket[i] = Bucket2[friendID];
				else
					Bucket[i] = Bucket[friendID];
			}
		}
	}
}

void SplitOddEven::modify()
{
    share();
    unsigned int FullBucket[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) {
        if(Bucket[i] == inf) 
        {
            int attempt = 0;
            unsigned int next = (HashFunc_c.run((char *)(&attempt), 4)^ HashFunc_a.run((char *)(&i), 4))% bucketNum;
            while(Bucket[next] == inf) 
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

double SplitOddEven::result(const SplitOddEven & another)
{
    double same = 0;
    for(int i = 0; i < bucketNum; i++) 
    {
        if(Bucket[i] == another.Bucket[i])
                ++same;
    }
    return same / bucketNum ;
}

