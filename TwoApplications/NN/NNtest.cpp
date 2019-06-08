#include<iostream>
#include<fstream>
#include<sys/fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<map>
#include<set>
#include<vector>
#include<utility>
#include<algorithm>
#include<stdlib.h>
#include<string.h>
#include"../BOBHash32.h"
#include"../MinHash.h"
#include"../OOPH.h"
#include"../SplitOddEven.h"
using namespace std;
#define inf 0xffffffff
class cmpsim
{
	public:
	int label;
	double sim;
	cmpsim(int _label=0,double _sim=0)
	{
		label=_label;
		sim=_sim;
	}
	int operator <(const cmpsim& other)const
	{
		return sim>other.sim;
	}
};

class signature
{
	public:
	int siglen;
	char* sig;
	signature(char* _sig,int _siglen)
	{sig=_sig;
	 siglen=_siglen;
	}
	int operator <(const signature& other)const
	{
		for(int i=0;i<siglen;i++)
		{
			if(sig[i]!=other.sig[i])
				{return sig[i]<other.sig[i];}
		}
		return 0;
	}
};

class intsignature
{
	public:
	int siglen;
	int* sig;
	intsignature(int* _sig,int _siglen)
	{sig=_sig;
	 siglen=_siglen;
	}
	int operator <(const intsignature& other)const
	{
		for(int i=0;i<siglen;i++)
		{
			if(sig[i]!=other.sig[i])
				{return sig[i]<other.sig[i];}
		}
		return 0;
	}
};

int main(int arg,char** argc)		
{
	string datasource(argc[1]);
	fstream readfile((string("../../data/")+datasource+string(".binary")).c_str(),ios::in);
	int b=atoi(argc[2]),hashnum=atoi(argc[3]),datalen=4,L=500;
	vector<double>dif_rfscore[5];
	vector<int>Larray;
	map<int,vector<int>> datamap;
	vector<int>labels;
	char onedata[8000001];
	char* onedatap=onedata;
	char* spacep;
	char* devidep;
	int datasize=0;	
	srand(time(NULL));
	for(int i=22;i<=22;i++)
	{
		Larray.push_back(i);
	}
	for(;;)
	{
		readfile.getline(onedata,8000000);
		if(readfile.eof())
			{break;}
		datasize++;
		onedatap=onedata;
		int label;
		vector<int> labeldata;
		spacep=strchr(onedatap,' ');
		*spacep='\0';
		label=datasize;
		labels.push_back(label);
		for(;;)
		{	spacep++;
			devidep=strchr(spacep,':');
			if(devidep==NULL)
				{break;}
			*devidep='\0';
			int index=atoi(spacep);
			labeldata.push_back(index);
			devidep++;
			spacep=strchr(devidep,' ');
			if(spacep==NULL)
				{break;}
		}
		datamap.insert(make_pair(label,labeldata));
		if(datasize>=100000)
			{break;}
	}
 	for(int w=0;w<Larray.size();w++)
 	{
 		L=Larray[w];
 		int siglen=hashnum;
 		MinHash minhash[500];
		OOPH oophhash;
		SplitOddEven splitoehash;
		map<intsignature,set<int>> minhash_table[500];
		map<intsignature,set<int>> oophhash_table[500];
		map<intsignature,set<int>> splitoehash_table[500];
		map<int,vector<int>> oridatamap;
 		for(int i=0;i<L;i++)
		{
			minhash[i].init(hashnum,i);
		}
		oophhash.init(hashnum*L,rand());
		splitoehash.init(hashnum*L,rand());
		int p=1;
		for(map<int,vector<int>>::iterator it=datamap.begin();it!=datamap.end();it++,p++)
		{
			int label=it->first;
			for(int i=0;i<L;i++)
			{
				minhash[i].clear();
			}
			oophhash.clear();
			splitoehash.clear();
			for(int i=0;i<it->second.size();i++)
			{
				int index=it->second[i];
				oophhash.input((char*)(&index));
				splitoehash.input((char*)(&index));
				for(int j=0;j<L;j++)
				{
					minhash[j].input((char*)(&index));
				}
			}
			for(int i=0;i<L;i++)
			{
				intsignature mh_signature(minhash[i].outputintsign(b),siglen);
				minhash_table[i][mh_signature].insert(label);
				intsignature oh_signature(oophhash.outputintsign(i,hashnum,b),siglen);
				oophhash_table[i][oh_signature].insert(label);
				intsignature soe_signature(splitoehash.outputintsign(i,hashnum,b),siglen);
				splitoehash_table[i][soe_signature].insert(label);
			}
		}
		int testnum=500;
		double rfscore[5];
		for(int i=0;i<=0;i++)
		{
			rfscore[i]=0;
		}
		for(int p=0;p<testnum;p++)
		{
			multiset<cmpsim> topsimset;
			map<int,double> candisim;
			int query=labels[rand()%(labels.size())];
			for(int i=0;i<labels.size();i++)
			{
				int templabel=labels[i];
				if(templabel==query)
					{continue;}
				vector<int>unionset;
				vector<int>intersectset;
				set_union(datamap[query].begin(),datamap[query].end(),datamap[templabel].begin(),datamap[templabel].end(),back_inserter(unionset));
				set_intersection(datamap[query].begin(),datamap[query].end(),datamap[templabel].begin(),datamap[templabel].end(),back_inserter(intersectset));
				double sim=double(intersectset.size())/double(unionset.size());
				topsimset.insert(cmpsim(templabel,sim));
				candisim.insert(make_pair(templabel,sim));
			}
			int timek=0;
			
			set<int> mh_candidate;
			set<int> oh_candidate;
			set<int> soe_candidate;
			for(int i=0;i<L;i++)
			{
				minhash[i].clear();
			}
			oophhash.clear();
			splitoehash.clear();
			for(vector<int>::iterator it=datamap[query].begin();it!=datamap[query].end();it++)
			{
				int index=*it;
				oophhash.input((char*)(&index));
				splitoehash.input((char*)(&index));
				for(int i=0;i<L;i++)
				{
					minhash[i].input((char*)(&index));
				}
			}
			for(int i=0;i<L;i++)
			{
				intsignature mh_signature(minhash[i].outputintsign(b),siglen);
				for(set<int>::iterator it=minhash_table[i][mh_signature].begin();it!=minhash_table[i][mh_signature].end();it++)
				{
					if(*it!=query)
					{
						mh_candidate.insert(*it);
					}
				}
				intsignature oh_signature(oophhash.outputintsign(i,hashnum,b),siglen);
				for(set<int>::iterator it=oophhash_table[i][oh_signature].begin();it!=oophhash_table[i][oh_signature].end();it++)
				{
					if(*it!=query)
					{
						oh_candidate.insert(*it);
					}
				}
				intsignature soe_signature(splitoehash.outputintsign(i,hashnum,b),siglen);
				for(set<int>::iterator it=splitoehash_table[i][soe_signature].begin();it!=splitoehash_table[i][soe_signature].end();it++)
				{
					if(*it!=query)
					{
						soe_candidate.insert(*it);
					}
				}
			}
			int mh_posinum=0,oh_posinum=0,soe_posinum=0;
			int i;
			float countT=0;
			set<cmpsim>::iterator it;
			countT=0;
			for(set<cmpsim>::iterator it=topsimset.begin();it!=topsimset.end()&&it->sim>=0.5;countT++,it++)
			{
				if(mh_candidate.find(it->label)!=mh_candidate.end())
				{
					mh_posinum++;
				}
				if(oh_candidate.find(it->label)!=oh_candidate.end())
				{
					oh_posinum++;
				}
				if(soe_candidate.find(it->label)!=soe_candidate.end())
				{
					soe_posinum++;
				}
			}
			if(countT!=0)
			{
				double mh_precision;
				double mh_recall=double(mh_posinum)/countT;
				if(mh_candidate.size()==0||mh_recall==0)
				{mh_precision=0;
				 rfscore[0]+=0;
				}
				else
				{mh_precision=double(mh_posinum)/double(mh_candidate.size());
				 rfscore[0]+=2*mh_recall*mh_precision/(mh_recall+mh_precision);
				}

				double oh_precision;
				double oh_recall=double(oh_posinum)/countT;
				if(oh_candidate.size()==0||oh_recall==0)
				{
					oh_precision=0;
					rfscore[1]+=0;
				}
				else
				{
					oh_precision=double(oh_posinum)/double(oh_candidate.size());
					rfscore[1]+=2*oh_recall*oh_precision/(oh_recall+oh_precision);
				}

				double soe_precision;
				double soe_recall=double(soe_posinum)/countT;
				if(soe_candidate.size()==0||soe_recall==0)
				{
					soe_precision=0;
					rfscore[2]+=0;
				}
				else
				{
					soe_precision=double(soe_posinum)/double(soe_candidate.size());
					rfscore[2]+=2*soe_recall*soe_precision/(soe_recall+soe_precision);
				}

			}
		}
 	}
	printf("Larray:");
	for(int i=0;i<Larray.size();i++)
	{
		printf(" %d",Larray[i]);
	}
	printf("\nmh_rfscore:");
	for(int i=0;i<Larray.size();i++)
	{
		printf(" %lf",dif_rfscore[0][i]);
	}
	printf("\noh_rfscore:");
	for(int i=0;i<Larray.size();i++)
	{
		printf(" %lf",dif_rfscore[1][i]);
	}
	printf("\nsoe_rfscore:");
	for(int i=0;i<Larray.size();i++)
	{
		printf(" %lf",dif_rfscore[2][i]);
	}
	return 0;
}