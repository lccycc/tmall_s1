#ifndef USER_H
#define USER_H

#include "include/head.h"
#include "include/mth.h"
#include "include/counter.h"

#include "include/constv.h"

class InputUnit{
public:
	int id, x, c, mon, iday, d;
	bool init(ifstream &fin);
};

class DayUnit{
public:
	int buy;
	int click;
	int car;
	int save;
	int d;
	DayUnit();
};
typedef vector<DayUnit> GHistory;

class UProfile{
public:
	int id;
	vector<map<int, DayUnit> > dayhis;
	map<int, GHistory> ghis;
	map<int, int> buydays;// how may times (day) the user buy this good
	map<int, int> buyamnt;// how many total amount user buy this good
	map<int, int> lastbuydate;// how many total amount user buy this good
	map<int, int> lastbuyamnt;
	map<int, int> cardate;

	Avgctr days_click2buy;
	Avgctr amnt_click2buy;
	Avgctr clickdaycnt2buy; //click day and buy it

	map<int, int> clickdays;
	map<int, int> clickamnt;
	map<int, int> lastclickdate;

	set<int> click, buy;
	int total_clickamnt;
	int total_clickday_by_good;
	int total_buydays_by_good;

	static map<int, vector<InputUnit> > split(vector<InputUnit> &a);

	void init(vector<InputUnit> &a);
	int getbuydays(int x, int lastcnt);
	int getbuydays(int x, int left, int right);
	int getclickdays(int x, int lastcnt);
	int getmostclick(int x, int lastcnt);
	int getclickamnt(int x, int lastcnt);
	int getclickamnt(int x, int left, int right);
};

class User{
public:
	map<int, UProfile> up;
	void init(vector<InputUnit> &a);
};

#endif
