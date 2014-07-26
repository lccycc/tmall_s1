#ifndef OUTPUT_H
#define OUTPUT_H
#include "include/head.h"

class Output{
	ofstream fout;
public:
	Output(string name);
	void printhh(int id, const set <int> &a);
};

#endif
