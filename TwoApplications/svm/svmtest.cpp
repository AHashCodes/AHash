#include<iostream>
#include<fstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../BOBHash32.h"
#include"../MinHash.h"
#include"../OOPH.h"
#include"../SplitOddEven.h"
using namespace std;
#define inf 0xffffffff
int main(int arg,char** argc)
{
	string datasource(argc[1]);
	fstream readfile((string("../../data/")+datasource+string(".binary")).c_str(),ios::in);
	FILE* mh_wf=fopen((string("../../data/")+datasource+string("_minhash.binary")).c_str(),"w");
	FILE* oh_wf=fopen((string("../../data/")+datasource+string("_oophhash.binary")).c_str(),"w");
	FILE* soe_wf=fopen((string("../../data/")+datasource+string("_splitoehash.binary")).c_str(),"w");
	int b=atoi(argc[2]),hashnum=atoi(argc[3]),datalen=4;
	char onedata[8000001];
	char* onedatap=onedata;
	char* spacep;
	char* devidep;
	int datasize=0;
	MinHash minhash(hashnum,rand());
	OOPH oophhash(hashnum,rand());
	SplitOddEven splitoehash(hashnum,rand());
	for(;;)
	{
		readfile.getline(onedata,8000001);
		if(readfile.eof())
			{break;}
		datasize++;
		onedatap=onedata;
		minhash.clear();
		oophhash.clear();
		splitoehash.clear();
		spacep=strchr(onedatap,' ');
		while(onedatap!=spacep)
		{
			fprintf(mh_wf,"%c",*onedatap);
			fprintf(oh_wf,"%c",*onedatap);
			fprintf(soe_wf,"%c",*onedatap);
			onedatap++;
		}
		for(;;)
		{	spacep++;
			devidep=strchr(spacep,':');
			if(devidep==NULL)
				{break;}
			*devidep='\0';
			int index=atoi(spacep);
			minhash.input((char*)(&index));
			oophhash.input((char*)(&index));
			splitoehash.input((char*)(&index));
			devidep++;
			spacep=strchr(devidep,' ');
			if(spacep==NULL)
				{break;}
		}
		vector<int>mh_output=minhash.outputvec(b);
		vector<int>oh_output=oophhash.outputvec(b);
		vector<int>soe_output=splitoehash.outputvec(b);
		for(int i=0;i<mh_output.size();i++)
		{
			if(mh_output[i]!=0)
			{
			 fprintf(mh_wf," %d:%d",i+1,mh_output[i]);	
			}			
			if(oh_output[i]!=0)
			{
			 fprintf(oh_wf," %d:%d",i+1,oh_output[i]);
			}
			if(soe_output[i]!=0)
			{
			 fprintf(soe_wf," %d:%d",i+1,soe_output[i]);
			}
		}
		fprintf(mh_wf,"\n");
		fprintf(oh_wf,"\n");
		fprintf(soe_wf,"\n");
	}
	return 0;
}