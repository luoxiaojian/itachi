#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <utility>

using namespace std;

char *ldname, *ofname;
vector<int> coreNum;
vector<int> utVal;

class result {
public:
	explicit result(double p, double m) : preemption(p), migration(m) { }

	double preemption, migration;
};

void initVecs() {
	for(int i=2; i<=32; i+=2)
		coreNum.push_back(i);
	for(int i=11; i<=20; i++)
		utVal.push_back(i*5);
}

result scan(int tnum, int pnum, int ut) {
	result res(0, 0);
	int count=0;
	for(int ind=1; ind<=20; ind++) {
		char filename[20];
		char entileName[50];
		sprintf(filename, "/%d_%d_%d_%d.log", tnum, pnum, ut, ind);
		strcpy(entileName, ldname);
		strcat(entileName, filename);
		ifstream fin(entileName);
		string tmp;
		fin>>tmp;
		if(tmp.empty()) {
			fin.clear();
			fin.close();
			continue;
		}
		count+=1;
		double sum=0.0;
		for(int i=0; i<tnum; i++) {
			double cur;
			fin>>cur;
			if(i==(tnum-1))
				continue;
			sum+=cur;
		}
		sum=sum/(tnum-1);
//		sum=sum/tnum;
		res.preemption+=sum;
		fin>>tmp;
		sum=0.0;
		for(int i=0; i<tnum; i++) {
			double cur;
			fin>>cur;
			if(i==(tnum-1))
				continue;
			sum+=cur;
		}
		sum=sum/(tnum-1);
//		sum=sum/tnum;
		res.migration+=sum;
		fin.clear();
		fin.close();
	}
	if(count==0) {
		res.preemption=-1;
		res.migration=-1;
	}
	else {
		res.preemption=res.preemption/count;
		res.migration=res.migration/count;
	}
	return res;
}

int main(int argc, char **argv)
{
	if(argc<3)
	{
		cout<<"usage: ./statis <logfile_dir> <output_filename>"<<endl;
		return 0;
	}

	ldname=argv[1];
	ofname=argv[2];

	initVecs();


	vector<pair<int, result> > coreX, utX;

	for(vector<int>::iterator it=coreNum.begin(); it!=coreNum.end(); it++) {
		result res=scan((*it)*2, *it, 100);
		coreX.push_back(make_pair(*it, res));
	}

	for(vector<int>::iterator it=utVal.begin(); it!=utVal.end(); it++) {
		result res=scan(24, 16, *it);
		utX.push_back(make_pair(*it, res));
	}

	ofstream fout(ofname);
	fout<<"coreNumX:"<<endl;
	fout<<"preemption:"<<endl;
	for(vector<pair<int, result> >::iterator it=coreX.begin(); it!=coreX.end(); it++)
		fout<<it->first<<"\t";
	fout<<endl;
	for(vector<pair<int, result> >::iterator it=coreX.begin(); it!=coreX.end(); it++)
		fout<<it->second.preemption<<"\t";
	fout<<endl;
	fout<<"migration:"<<endl;
	for(vector<pair<int, result> >::iterator it=coreX.begin(); it!=coreX.end(); it++)
		fout<<it->first<<"\t";
	fout<<endl;
	for(vector<pair<int, result> >::iterator it=coreX.begin(); it!=coreX.end(); it++)
		fout<<it->second.migration<<"\t";
	fout<<endl;

	fout<<"utilizationX:"<<endl;
	fout<<"preemption:"<<endl;
	for(vector<pair<int, result> >::iterator it=utX.begin(); it!=utX.end(); it++)
		fout<<it->first<<"\t";
	fout<<endl;
	for(vector<pair<int, result> >::iterator it=utX.begin(); it!=utX.end(); it++)
		fout<<it->second.preemption<<"\t";
	fout<<endl;
	fout<<"migration:"<<endl;
	for(vector<pair<int, result> >::iterator it=utX.begin(); it!=utX.end(); it++)
		fout<<it->first<<"\t";
	fout<<endl;
	for(vector<pair<int, result> >::iterator it=utX.begin(); it!=utX.end(); it++)
		fout<<it->second.migration<<"\t";
	fout<<endl;

	fout.clear();
	fout.close();

	return 0;
}
