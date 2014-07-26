#ifndef LR_H
#define LR_H
#include "profile/good.h"
class LR{
public:
	vector<double> w;
	LR();
	void init(Good &g, User &u);
	set<int> pred(Good &g, UProfile &upf);
};
#endif
