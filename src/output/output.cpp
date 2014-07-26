#ifndef OUTPUT_CPP
#define OUTPUT_CPP

#include "output.h"

Output::Output(string name){
	fout.open((string("data/output/")+name).c_str());
}
void Output::printhh(int id, const set<int> &a){
	if (a.size() == 0) return ;
	fout<<id<<"\t";
	auto f1 = a.begin();
	fout<<*f1;
	for (f1++; f1 != a.end(); f1++){
		fout<<","<<*f1;
	}
	fout<<endl;
	for (auto it = a.begin(); it!= a.end(); it++){
		cerr<<id<<' '<<*it<<endl;
	}
}

#endif
