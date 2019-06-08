#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <map>
#include <unistd.h>
#include <cstring>
#include <memory.h>
#include <algorithm>
using namespace std;

class StandardCounter{
    public:
    int QueryA(const char *str);
    int QueryB(const char *str);
    void HandleA(const char *str);
    void HandleB(const char *str);
    double result();
    private:
    map<string, int> A, B;
};

int StandardCounter::QueryA(const char *str)
{
    map<string, int>::iterator it;
    string tmp = string(str, 4);
    it = A.find(tmp);
    if(it == A.end()) {
        return 0;
    }
    else {
        return it->second;
    }
}

int StandardCounter::QueryB(const char *str)
{
    map<string, int>::iterator it;
    string tmp = string(str, 4);
    it = B.find(tmp);
    if(it == B.end()) {
        return 0;
    }
    else {
        return it->second;
    }
}

void StandardCounter::HandleA(const char *str)
{
    string tmp(str);
    A[tmp]++;
}

void StandardCounter::HandleB(const char *str)
{
    string tmp(str);
    B[tmp]++;
}

double StandardCounter::result()
{
    map<string, int>::iterator it, st;
    double cmax = 0;
    double cmin = 0;
    for(it = A.begin(); it != A.end(); it++) {
        st = B.find(it->first);
        if(st == B.end()) {
            cmax += it->second;
        }
        else if(st->second > it->second) {
            cmax += st->second;
            cmin += it->second;
        }
        else {
            cmax += it->second;
            cmin += st->second;
        }
    }
    for(it = B.begin(); it != B.end(); it++) {
        st = A.find(it->first);
        if(st == A.end()) {
            cmax += it->second;
        }
    }
    return cmin / cmax ;
} 
