#include <iostream>
#include <cmath>
#include <cstdlib>
#include "BOBHash32.h"
using namespace std;
#define inf 0xffffffff

#define BUCKETNUMMAX 20000

class OOPH{
    
    public:
    int noempty = 0;
    OOPH(){};
    ~OOPH();
    OOPH(int _bucket, int seed);
    void input(const char *str);
    void init(int _hashnum,int seed);
    unsigned int hash(const char *str, int feq);
    double result(const OOPH & another);
    void modify();
    vector<int> outputvec(int  b);
    char* outputsign(int b);
    int* outputintsign(int k,int eachgroupnum,int b);
    void clear();

    private:
    int bucketNum;
    unsigned int* Bucket;
    class BOBHash32 HashFunc_a;
    class BOBHash32 HashFunc_b;
    class BOBHash32 HashFunc_c;
};

OOPH::OOPH(int _bucket, int seed)
{
    bucketNum = _bucket;
    Bucket=new unsigned int[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) {
        Bucket[i] = inf;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

OOPH::~OOPH()
{
    delete []Bucket;
}

void OOPH::init(int _bucket, int seed)
{
    bucketNum = _bucket;
    Bucket=new unsigned int[BUCKETNUMMAX];
    for(int i = 0; i < bucketNum; i++) {
        Bucket[i] = inf;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

void OOPH::clear()
{
    for(int i=0;i<bucketNum;i++)
    {
        Bucket[i]=inf;
    }
}

unsigned int OOPH::hash(const char *str, int feq)
{
 
    return ((unsigned int) HashFunc_b.run(str, 4));
}

void OOPH::input(const char *str)
{
    int num = 1;
    unsigned int hashresult = hash(str, num);
    unsigned int bucketid = hashresult / (UINT32_MAX / bucketNum);
    int id = bucketid % bucketNum;
    if(hashresult < Bucket[id]) {
        Bucket[id] = hashresult;
    }
}


void OOPH::modify()
{
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
        if(Bucket[i] == inf) {
            int attempt = 0;
            unsigned int next = (HashFunc_c.run((char *)(&attempt), 4)^ HashFunc_a.run((char *)(&i), 4))% bucketNum;
            while(Bucket[next] == inf) {
                ++attempt; 
                next = (HashFunc_c.run((char *)(&attempt), 4)^ HashFunc_a.run((char *)(&i), 4))% bucketNum;
            }
            FullBucket[i] = Bucket[next];
        }
        else
        {
            noempty++;
            FullBucket[i] = Bucket[i];
        }

    }
    for(int i = 0; i < bucketNum; i++) 
        Bucket[i] = FullBucket[i]; 
}

double OOPH::result(const OOPH & another)
{
    double same = 0;
    for(int i = 0; i < bucketNum; i++) {
        if(Bucket[i] == another.Bucket[i]) {
                ++same;
        }
    }
    return same / bucketNum ;
}

vector<int> OOPH::outputvec(int b)
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

char* OOPH::outputsign(int b)
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

int* OOPH::outputintsign(int k,int eachgroupnum,int b)
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
