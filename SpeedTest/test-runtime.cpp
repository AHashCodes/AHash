#include <iostream>
#include <fstream>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <string.h>
#include "memory.h"
#include <iomanip>
#include "time.h"

const unsigned int inf = 0xffffffff;
#include "Standard.h"
#include "OOPH.h"
#include "SplitMaxMin.h"
#include "SplitOddEven.h"
#include "MinHash.h"
using namespace std;

vector<int> mysets[100000];
int setsize[100000];

int main(int argc, char **argv)
{
    fstream readfile(argv[1], ios::in);
    char datainline[100000001];
    int setindex = 0;
    char *sp;
    memset(setsize, 0, 100000 * sizeof(int));
    while(true) 
    {
        readfile.getline(datainline, 100000000);
		if(readfile.eof()) break;
        sp = strchr(datainline, ' ');
        while(true) 
        {
            ++sp;
            char *tp = strchr(sp, ':');
            if(tp == NULL) break;
            *tp = '\0';
            mysets[setindex].push_back(atoi(sp));
            ++setsize[setindex];
            ++tp;
            sp = strchr(tp, ' ');
            if(sp == NULL)
            break;
        }
        ++setindex;
        if(setindex >= 100000) break;
    }

    for(int bucketNum=128;bucketNum<513;bucketNum<<=1)
    {
    double MHmse = 0;
    double TBmse= 0;
    double FBmse = 0;
    double SPmse= 0;
    double SPMMmse = 0;
    double OOPHmse = 0;
    double CPmse =0;
    double CP8mse =0;
    int T = 10;
    for(int times = 0; times < T; times++) 
    {
        int aIndex = 0;
        int bIndex = 0;
        MinHash mh1(bucketNum, times);
        MinHash mh2(bucketNum, times);
        OOPH ooph1(bucketNum, times);
        OOPH ooph2(bucketNum, times);
        //SplitOddEven sp1(bucketNum, times);
        //SplitOddEven sp2(bucketNum, times);
        SplitMaxMin spmm1(bucketNum, times);
        SplitMaxMin spmm2(bucketNum, times);

        double mhtime, oophtime, spilttime;
        clock_t start, finish;

        StandardCounter S_counter;
        
        start = clock();
        for(int a = 0; a < setindex; a++)
        {
            aIndex+= setsize[a];
            for(int i = 0; i < setsize[a]; i++) 
            {
                mh1.input((char *)(&mysets[a][i]));
                
            }
        }
        finish = clock();
        mhtime += double (finish - start) / CLOCKS_PER_SEC;
        
        start = clock();
        for(int a = 0; a < setindex; a++)
        {
            aIndex+= setsize[a];
            for(int i = 0; i < setsize[a]; i++) 
            {
                ooph1.input((char *)(&mysets[a][i]));
            }
        }
        ooph1.modify();
        finish = clock();
        oophtime += double (finish - start) / CLOCKS_PER_SEC;
        
        start = clock();
        for(int a = 0; a < setindex; a++)
        {
            aIndex+= setsize[a];
            for(int i = 0; i < setsize[a]; i++) 
            {
                spmm1.input((char *)(&mysets[a][i]));
            }
        }
        spmm1.modify();
        finish = clock();
        spilttime += double (finish - start) / CLOCKS_PER_SEC;


        if(times == T-1)
        {
            cout << mhtime/T << " " << oophtime/T << " " << spilttime/T << endl;  
        }
    }
    }
}