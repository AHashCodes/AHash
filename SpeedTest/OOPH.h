#include <iostream>
#include <cmath>
#include <cstdlib>
#include "BOBHash32.h"
using namespace std;


#define BUCKETNUMMAX 20000

class OOPH{
    
    public:
    int noempty = 0;
    int emptycounter[24];
    int Bucketcounter[BUCKETNUMMAX];
    OOPH(int _bucket, int seed);
    void input(const char *str);
    unsigned int hash(const char *str, int feq);
    double result(const OOPH & another);
    void modify();
    void printcounter();

    private:
    int bucketNum;
    unsigned int Bucket[BUCKETNUMMAX];
    class BOBHash32 HashFunc_a;
    class BOBHash32 HashFunc_b;
    class BOBHash32 HashFunc_c;
};

OOPH::OOPH(int _bucket, int seed)
{
    bucketNum = _bucket;
    for(int i = 0; i < bucketNum; i++) {
        Bucket[i] = inf;
        Bucketcounter[i] = 0;
    }
    for(int i = 0; i < 24; i++) {
        emptycounter[i] = 0;
    }
    HashFunc_a.initialize(3*seed);
    HashFunc_b.initialize(5*seed);
    HashFunc_c.initialize(7*seed);
}

unsigned int OOPH::hash(const char *str, int feq)
{
 
    return ((unsigned int) HashFunc_b.run(str, 4));
}

void OOPH::input(const char *str)
{
    int num = 1;
    unsigned int hashresult = hash(str, num);
    unsigned int id = hashresult / (UINT32_MAX / bucketNum);
    //int id = bucketid % bucketNum;
    //++Bucketcounter[id]; 
    if(hashresult < Bucket[id]) {
        Bucket[id] = hashresult;
    }
}


void OOPH::modify()
{
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

void OOPH::printcounter()
{
    for(int i = 0; i < bucketNum; i++) {
        ++emptycounter[Bucketcounter[i]];
    }
    int i = 0;
    while(emptycounter[i]) {
        cout << i << " " << emptycounter[i] << endl;
        ++i;
    }
    cout << endl;
}