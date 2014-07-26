#ifndef GOOD_H
#define GOOD_H

#include "include/head.h"
#include "include/mth.h"
#include "include/counter.h"
#include "user.h"

class GProfile{
public:
	int x;
	int buydaycnt; //howmanydays this good is bought
	int buyusercnt; //howmany user buy this good
	int sellcnt; //howmany goods has been selled
	vector<int> sellbd;
	vector<int> clickbd;
	int changerank;

	int clickamnt;
	int clickdaycnt;
	int clickusercnt;

	Avgctr days_buyagain; //how many days one will buy again
	Avgctr days_buyagain_byamnt; //how many days one will buy again

	GProfile();
	bool willbuyagain(UProfile &upf);
	bool willbuy(UProfile &upf);

	int getsellbd(int lastcnt);
	int getclickbd(int lastcnt);
	int getclickbd(int left, int right);
};

class Good{
public:
	set<int> goodset;
	map<int, GProfile> gp;
	set<int> buyagainset;
	map<int, map<int, int> > co_buy;
	map<int, map<int, int> > co_click;

	vector<pair<set<int>, set<int> > > xp;
	int bestclickup;

	void init(User &u);
	void co_buy_init(User &u);
	void co_click_init(User &u);
	int getbestsell(int lastcnt);
	int getbestclick(int lastcnt);
	set<int> getbestsell(set<int> &gset, int get);

	set<int> willbuyagain(UProfile &upf);
	set<int> willbuy(UProfile &upf);
	set<int> willbuytoo(UProfile &upf);
	set<int> test(UProfile &upf, User &u);

};

#endif
