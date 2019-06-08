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

const unsigned int inf = 0xffffffff;
#include "Standard.h"
#include "OOPH.h"
#include "SplitMaxMin.h"
#include "SplitOddEven.h"
#include "MinHash.h"
using namespace std;

vector<int> mysets[24000];
int setsize[24000];

int main(int argc, char** argv)
{
    fstream readfile(argv[1], ios::in);
    char datainline[1000000];
    int setindex = 0;
    char *sp;
    memset(setsize, 0, 24000 * sizeof(int));
    while(true) 
    {
        readfile.getline(datainline, 1000000);
		if(readfile.eof()) break;
        sp = strchr(datainline, ' ');
        while(true) 
        {
            ++sp;
            char *tp = strchr(sp, ':');
            *tp = '\0';
            mysets[setindex].push_back(atoi(sp));
            ++setsize[setindex];
            ++tp;
            sp = strchr(tp, ' ');
            if(sp == NULL)
            break;
        }
        ++setindex;
        if(setindex >= 80) break;
    }

    for(int bucketNum=64;bucketNum<5000;bucketNum<<=1)
    {
    double MHmse = 0;
    double TBmse= 0;
    double FBmse = 0;
    double SPmse= 0;
    double SPMMmse = 0;
    double OOPHmse = 0;
    double CPmse =0;
    double CP8mse =0;
    int T = 500;
    for(int times = 0; times < T; times++) 
    {
        int aIndex,bIndex;
        MinHash mh1(bucketNum, times);
        MinHash mh2(bucketNum, times);
        OOPH ooph1(bucketNum, times);
        OOPH ooph2(bucketNum, times);
        //SplitOddEven sp1(bucketNum, times);
        //SplitOddEven sp2(bucketNum, times);
        SplitMaxMin spmm1(bucketNum, times);
        SplitMaxMin spmm2(bucketNum, times);

        StandardCounter S_counter;

        for(int a = 0; a < 5; a++)
        {
            aIndex+= setsize[a];
            for(int i = 0; i < setsize[a]; i++) 
            {
                mh1.input((char *)(&mysets[a][i]));
                ooph1.input((char *)(&mysets[a][i]));
                //sp1.input((char *)(&mysets[a][i]));
                spmm1.input((char *)(&mysets[a][i]));
                S_counter.HandleA((char *)(&mysets[a][i]));
            }
        }
        for(int b = 3; b < 8; b++){
            bIndex+= setsize[b];
            for(int i = 0; i < setsize[b]; i++) 
            {
                mh2.input((char *)(&mysets[b][i]));
                ooph2.input((char *)(&mysets[b][i]));
                //sp2.input((char *)(&mysets[b][i]));
                spmm2.input((char *)(&mysets[b][i]));
                S_counter.HandleB(((char *)(&mysets[b][i])));
            }
        }
        ooph1.modify();
        ooph2.modify();
        //sp1.modify();
        //sp2.modify();
        spmm1.modify();
        spmm2.modify();
        if(!times)
        {
            cout<<endl;
            cout<<bucketNum<<"bins   ";
            cout<<ooph1.noempty<<"full-OOPH   ";
            cout<<spmm1.noempty<<"full-Split     "<<endl;
            cout << "TRUE"<<S_counter.result() <<endl;
        }
        double MHbias = mh1.result(mh2)- S_counter.result();
        //double SPbias = sp1.result(sp2) - S_counter.result();
        double SPMMbias = spmm1.result(spmm2) - S_counter.result();
        double OOPHbias = ooph1.result(ooph2) - S_counter.result();
        MHmse += MHbias*MHbias;
        //SPmse += SPbias * SPbias;
        SPMMmse += SPMMbias * SPMMbias;
        OOPHmse += OOPHbias * OOPHbias;
    }

    cout << "Minhash: "<<MHmse / T << endl;
    cout << "OOPH: "<<OOPHmse / T << endl;
    //cout << "Split-OddEven:"<<SPmse / T <<endl;
    cout << "Split-MaxMin:"<<SPMMmse / T <<endl;
    }
}