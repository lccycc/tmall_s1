#ifndef COUNTER_H
#define COUNTER_H
#include "include/head.h"

class Avgctr{
	int cnt;
	double sum;
public:
	Avgctr();
	void add(double x);
	double val();
};

#endif

