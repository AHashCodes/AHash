#include <iostream>
#include <cmath>
#include <cstdlib>
#include "BOBHash32.h"
using namespace std;
#define inf 0xffffffff
#define BUCKETNUMMAX 20000

class SplitOddEven{    
    public:

    int bucketNum;
    int noempty = 0;
    unsigned int* Bucket;
    unsigned int* Bucket2;
   
    class BOBHash32 HashFunc_a;
    class BOBHash32 HashFunc_b;
    class BOBHash32 HashFunc_c;

    SplitOddEven(){};
    ~SplitOddEven();
    SplitOddEven(int _bucket, int seed);
    void init(int _hashnum,int seed);
    unsigned int hash(const char *str, int feq);
    void input(const char *str);
    void share();
    void modify();
    double result(const SplitOddEven & another);
    vector<int> outputvec(int b);
    char* outputsign(int b);
    int* outputintsign(int k,int eachgroupnum,int b);
    void clear();

};

SplitOddEven::SplitOddEven(int _bucket, int seed)
{
    bucketNum = _bucket;
    Bucket=new unsigned int[BUCKETNUMMAX];
    Bucket2=new unsigned int[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) 
    {
        Bucket[i]  = Bucket2[i] = inf;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

SplitOddEven::~SplitOddEven()
{
    delete []Bucket;
    delete []Bucket2;
}

void SplitOddEven::init(int _bucket, int seed)
{
    bucketNum = _bucket;
    Bucket=new unsigned int[BUCKETNUMMAX];
    Bucket2=new unsigned int[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) 
    {
        Bucket[i]  = Bucket2[i] = inf;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

void SplitOddEven::clear()
{
    for(int i=0;i<bucketNum;i++)
    {
        Bucket[i]  = Bucket2[i] = inf;
    }
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
    for(int i = 0; i < bucketNum; i++) 
    {
		if(Bucket[i]!=inf)
        {
            if(!(i%2) && Bucket2[i] != inf)
				Bucket[i] = Bucket2[i]; 
        }
        else
        {   
            if(Bucket2[i]!=inf)
               Bucket[i] = Bucket2[i];   
            else
            {
               unsigned int friendID = (i+bucketNum/2)%bucketNum;
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
    int flag=0;
    for(int i=0;i<bucketNum;i++)
    {
        if(Bucket[i]!=inf)
            {flag=1;}
    }
    if(!flag)
        {return;}
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

vector<int> SplitOddEven::outputvec(int b)
{
    modify();
    vector<int> output_vec;
    int spreadlen=pow(2,b)-1;
    int lowbiter=0;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<bucketNum;i++)
    {
        int lowbit=lowbiter&Bucket[i];
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

char* SplitOddEven::outputsign(int b)
{
    modify();
    if(b*bucketNum%8!=0)
    {
        printf("8 need to be divisible by b*k\n");
        return NULL;
    }
    if(8%b!=0)
    {
        printf("b need to be divisible by 8\n");
        return NULL;
    }
    char* signature=new char[b*bucketNum/8];
    int spreadlen=pow(2,b)-1;
    int lowbiter=0;
    int partnum=8/b;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<b*bucketNum/8;i++)
    {
        signature[i]=0;
    }
    for(int i=0;i<bucketNum;i++)
    {
        int lowbit=lowbiter&Bucket[i];
        int signindex=i/partnum;
        int partindex=i%partnum;
        signature[signindex]|=(lowbit<<(b*(partnum-1-partindex)));
    }
    return signature;
}

int* SplitOddEven::outputintsign(int k,int eachgroupnum,int b)
{
    modify();
    int* signature=new int[eachgroupnum];
    int startindex=k*eachgroupnum;
    int lowbiter=0;
    for(int i=0;i<b;i++)
    {
        lowbiter+=1<<i;
    }
    for(int i=0;i<eachgroupnum;i++)
    {
        signature[i]=0;
    }
    for(int i=0;i<eachgroupnum;i++)
    {
        int lowbit=lowbiter&Bucket[startindex+i];
        signature[i]=lowbit;
    }
    return signature;
}
