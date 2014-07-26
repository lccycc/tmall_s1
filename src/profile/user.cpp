#ifndef USER_CPP
#define USER_CPP

#include "user.h"
bool InputUnit::init(ifstream &fin){
	bool r = false;
	if (fin>>id){
		r = true;
		fin>>x>>c>>mon>>iday;
		d = getday(mon, iday);
	}
	return r;
}
bool cmp_id_x_d(InputUnit a, InputUnit b){
	if (a.id < b.id) return true;
	if (a.id == b.id && a.x < b.x) return true;
	if (a.id == b.id && a.x == b.x && a.d<b.d) return true;
	return false;
}

DayUnit::DayUnit(){
	car = buy = click = save = 0;
	d = -1;
}

map<int, vector<InputUnit> >
UProfile::split(vector<InputUnit> &a){
	sort(a.begin(), a.end(), cmp_id_x_d);
	map<int, vector<InputUnit> > ret;

	for (int i = 0, j; i<a.size(); i = j){
		j = i;
		while (j<a.size() && a[i].id == a[j].id){
			j++;
		}
		ret[a[i].id].insert(ret[a[i].id].end(), a.begin()+i, a.begin()+j);
	}
	return ret;
}

void UProfile::init(vector<InputUnit> &a){
	assert(a.size());
	id = a[0].id;

	dayhis.resize(CNS::MaxDay+1);

	for (int i = 0, j; i<a.size(); i = j){
		j = i;
		DayUnit du;
		du.d = a[i].d;
		while (j < a.size() && a[j].x == a[i].x && a[j].d == a[i].d){
			if (a[j].c == 0){
				du.click++;
			}
			click.insert(a[j].x);
			if (a[j].c == 1){
				du.buy++;
				buy.insert(a[j].x);
			}
			if (a[j].c == 2){
				du.save++;
			}
			if (a[j].c == 3){
				du.car++;
				cardate[a[i].x] = a[i].d;
			}
			j++;
		}
		if (du.buy>0){
			cardate.erase(a[i].x);
		}
		ghis[a[i].x].push_back(du);
		dayhis[du.d][a[i].x] = du;
	}

	for (auto it = ghis.begin(); it != ghis.end(); it++){
		int x = it->first;
		if (buy.find(x) == buy.end()) continue;

		for (auto jt = it->second.rbegin(); jt != it->second.rend(); jt++){
			if (jt->buy > 0){
				lastbuydate[x] = jt->d;
				lastbuyamnt[x] = jt->buy;
				break;
			}
		}
	}
	for (auto it = ghis.begin(); it != ghis.end(); it++){
		int x = it->first;
		for (int i = 0; i<it->second.size(); i++){
			if (it->second[i].buy > 0){
				buydays[x]++;
				buyamnt[x] += it->second[i].buy;
			}
		}
	}

	for (auto it = ghis.begin(); it != ghis.end(); it++){
		int x = it->first;
		clickdays[x] = it->second.size();
		for (int i = 0; i<it->second.size(); i++){
			clickamnt[x] += it->second[i].click;
		}
		if (clickdays[x]>0){
			lastclickdate[x] = it->second.back().d;
		}
	}
	total_clickday_by_good = 0;
	for (auto it = clickdays.begin(); it != clickdays.end(); it++){
		total_clickday_by_good += it->second;
	}
	total_clickamnt= 0;
	for (auto it = clickamnt.begin(); it != clickamnt.end(); it++){
		total_clickamnt += it->second;
	}
	total_buydays_by_good = 0;
	for (auto it = buydays.begin(); it != buydays.end(); it++){
		total_buydays_by_good+= it->second;
	}

	for (auto it = ghis.begin(); it != ghis.end(); it++){
		int x = it->first;
		if (buydays[x] == 0) continue;
		GHistory &his = it->second;
		int clickamnt = 0;
		for (int j = 0; j<his.size(); j++){
			clickamnt += his[j].click;
			if (his[j].buy > 0){
				int deta = his[j].d - his[0].d;
				if (deta >= 1){
					days_click2buy.add(deta);
				}
				amnt_click2buy.add(clickamnt);
				clickdaycnt2buy.add(j+1);
				break;
			}
		}
	}
}

int UProfile::getbuydays(int x, int lastcnt){
	return getbuydays(CNS::MaxDay - lastcnt +1, CNS::MaxDay);
}
int UProfile::getbuydays(int x, int left, int right){
	GHistory &his = ghis[x];
	int days = 0;
	for (int i = his.size()-1; i>=0; i--){
		if (his[i].d < left){
			break;
		}
		if (his[i].d >= right){
			days += (his[i].buy>0);
		}
	}
	return days;
}

int UProfile::getmostclick(int x, int lastcnt){
	GHistory &his = ghis[x];
	int maxc = 0;
	for (int i = his.size()-1; i>=0; i--){
		if (his[i].d <= CNS::MaxDay - lastcnt){
			break;
		}
		maxc = max(maxc, his[i].click);
	}
	return maxc;
}

int UProfile::getclickdays(int x, int lastcnt){
	GHistory &his = ghis[x];
	int days = 0;
	for (int i = his.size()-1; i>=0; i--){
		if (his[i].d <= CNS::MaxDay - lastcnt){
			break;
		}
		days += (his[i].click>0);
	}
	return days;
}

int UProfile::getclickamnt(int x, int lastcnt){
	return getclickamnt(x, CNS::MaxDay - lastcnt+1, CNS::MaxDay);
}
int UProfile::getclickamnt(int x, int left, int right){
	GHistory &his = ghis[x];
	int res = 0;
	for (int i = his.size()-1; i>=0; i--){
		if (his[i].d < left){
			break;
		}
		if (his[i].d >= right){
			res += his[i].click;
		}
	}
	return res ;
}

void User::init(vector<InputUnit> &a){
	auto splitres = UProfile::split(a);
	for (auto it = splitres.begin(); it != splitres.end(); it++){
		up[it->first].init(it->second);
	}
}

#endif
