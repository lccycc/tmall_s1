#ifndef MAIN_CPP
#define MAIN_CPP
#include "profile/good.h"
#include "output/output.h"
#include "lr/lr.h"

int CNS::MaxDay;
vector<InputUnit> vinput, vtest;

User user, utest;
Good good;
LR lr;

Output output("esti.out");

int main(){
	//ifstream fin("data/input/train_input", ios::in);
	//ifstream fin("data/input/train_input_615", ios::in);
	ifstream fin("data/input/aliout", ios::in);
	InputUnit inputd;
	while (inputd.init(fin)){
		/*
		if (inputd.d <= getday(6, 15)){
			cerr<<inputd.id<<'\t'<<inputd.x<<'\t'<<inputd.c<<'\t'<<inputd.mon<<'\t'<<inputd.iday<<endl;
		}
		*/
		vinput.push_back(inputd);
		CNS::MaxDay = max(CNS::MaxDay, inputd.d);
	}
	user.init(vinput);
	/*
	ifstream ftest("data/input/test_input");
	while (inputd.init(ftest)){
		vtest.push_back(inputd);
	}
	utest.init(vtest);
	*/

	good.init(user);

	lr.init(good, user);

	for (auto it = user.up.begin(); it != user.up.end(); it++){
		UProfile &upf = it->second;

		//output.printhh(upf.id, good.test(upf, user));
		//output.printhh(upf.id, lr.pred(good, upf));
		//output.printhh(upf.id, good.willbuyagain(upf));
		//output.printhh(upf.id, good.willbuy(upf));
		//output.printhh(upf.id, good.willbuy(upf) + good.willbuyagain(upf));
		output.printhh(upf.id, good.willbuy(upf) + good.willbuyagain(upf) + good.test(upf, user));
		//output.printhh(upf.id, (good.willbuy(upf) + good.willbuyagain(upf) + good.test(upf, user)) - (good.willbuy(upf) + good.willbuyagain(upf)));
		//output.printhh(upf.id, good.willbuy(upf) + good.willbuyagain(upf) + lr.pred(good, upf));
		//output.printhh(upf.id, good.willbuytoo(upf));
		//output.printhh(upf.id, good.willbuy(upf) + good.willbuyagain(upf) + good.willbuytoo(upf));
		//output.printhh(upf.id, good.test(upf, user));
	}
	return 0;
}
#endif
