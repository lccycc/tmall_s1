#ifndef COUNTER_CPP
#define COUNTER_CPP

#include "counter.h"

Avgctr::Avgctr(){
	cnt = 0;
	sum = 0;
}
void Avgctr::add(double x){
	cnt++;
	sum += x;
}
double Avgctr::val(){
	if (cnt == 0) return 0;
	return sum / cnt;
}

#endif
