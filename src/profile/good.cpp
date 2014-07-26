#ifndef GOOD_CPP
#define GOOD_CPP

#include "good.h"

GProfile::GProfile(){
	buydaycnt = 0;
	buyusercnt = 0;
	sellcnt = 0;

	clickamnt = 0;
	clickdaycnt = 0;
	clickusercnt = 0;

	for (int i = 0; i<=CNS::MaxDay; i++){
		sellbd.push_back(0);
		clickbd.push_back(0);
	}
}

void Good::co_buy_init(User &u){
	for (auto ut = u.up.begin(); ut != u.up.end(); ut++){
		UProfile &upf = ut->second;
		for (auto it = upf.buy.begin(); it != upf.buy.end(); it++){
			for (auto jt = upf.buy.begin(); jt != upf.buy.end(); jt++){
				if (it == jt) continue;

				co_buy[*it][*jt]++;
			}
		}
	}
}
void Good::co_click_init(User &u){

	for (auto ut = u.up.begin(); ut != u.up.end(); ut++){
		UProfile &upf = ut->second;
		for (int i = 0; i<upf.dayhis.size(); i++){
			auto &dhis = upf.dayhis[i];
			for (auto it = dhis.begin(); it != dhis.end(); it++){
				for (auto jt = dhis.begin(); jt != dhis.end(); jt++){
					if (it->first == jt->first) continue;
					co_click[it->first][jt->first]++;
				}
			}
		}
	}
}

void Good::init(User &u){
	for (auto it = u.up.begin(); it != u.up.end(); it++){
		UProfile &upf = it->second;
		goodset.insert(upf.click.begin(), upf.click.end());
		for (auto jt = upf.buydays.begin(); jt != upf.buydays.end(); jt++){
			gp[jt->first].buydaycnt += jt->second;
			gp[jt->first].buyusercnt += (jt->second>0);
		}
		for (auto jt = upf.buyamnt.begin(); jt != upf.buyamnt.end(); jt++){
			gp[jt->first].sellcnt += jt->second;
		}
		for (auto jt = upf.ghis.begin(); jt != upf.ghis.end(); jt++){
			int x = jt->first;
			GHistory &gv = jt->second;
			for (int i = 0; i<gv.size(); i++) if (gv[i].buy>0){
				for (int j = i+1; j<gv.size(); j++) if (gv[j].buy>0){
					gp[x].days_buyagain.add(gv[j].d - gv[i].d);
					gp[x].days_buyagain_byamnt.add((double)(gv[j].d - gv[i].d)/(double)gv[i].buy);
					i = j-1;
					break;
				}
			}
		}
		for (auto it = upf.click.begin(); it!=upf.click.end(); it++){
			int x = *it;
			gp[x].clickdaycnt += upf.clickdays[x];
			gp[x].clickusercnt += 1;
			gp[x].clickamnt += upf.clickamnt[x];
		}
	}
	for (auto it = gp.begin(); it != gp.end(); it++){
		it->second.x = it->first;
	}
	for (auto it = gp.begin(); it != gp.end(); it++){
		if (it->second.buydaycnt > 4){
			buyagainset.insert(it->first);
		}
	}
	co_buy_init(u);
	for (auto it = u.up.begin(); it != u.up.end(); it++){
		for (auto jt = it->second.ghis.begin();
				jt != it->second.ghis.end(); jt++){
			int x = jt->first;
			GHistory &his = jt->second;
			for (int i = 0; i<his.size(); i++){
				if (his[i].buy>0){
					gp[x].sellbd[his[i].d]++;
				}
				if (his[i].click>0){
					gp[x].clickbd[his[i].d]++;
				}
			}
		}
	}
	/* best click up */
	bestclickup = -1;
	double rate = -1;
	for (auto it = gp.begin(); it != gp.end(); it++){
		int i7 = it->second.getclickbd(7);
		int i30 = it->second.getclickbd(30)+1 - i7;
		/*
		if ((double)i7/(double)i30 > rate){
			rate = (double)i7/(double)i30;
		*/
		if (i7 > rate){
			rate = i7;
			bestclickup = it->first;
		}
	}
	vector<pair<double, int> > csrt;
	for (auto it = gp.begin(); it != gp.end(); it++){
		auto &gpf = it->second;
		double rate = -(double)gpf.buyusercnt/(double)gpf.clickusercnt;

		csrt.push_back(make_pair(rate,
				gpf.x));
	}
	sort(csrt.begin(), csrt.end());
	for (int i = 0; i<csrt.size();i++){
		gp[csrt[i].second].changerank = i;
	}
}

int GProfile::getsellbd(int lastcnt){
	int tot = 0;
	for (int i = CNS::MaxDay; i>CNS::MaxDay-lastcnt; i--){
		tot += sellbd[i];
	}
	return tot;
}

int GProfile::getclickbd(int lastcnt){
	return getclickbd(CNS::MaxDay-lastcnt+1, CNS::MaxDay);
}
int GProfile::getclickbd(int left, int right){
	int tot = 0;
	for (int i = right; i>=left; i--){
		tot += clickbd[i];
	}
	return tot;
}

int Good::getbestsell(int lastcnt){
	int l = -1, bs = -1;
	for (auto it = gp.begin(); it != gp.end(); it++){
		int x = it->first;
		int sl = it->second.getsellbd(lastcnt);
		if (sl > bs){
			bs = sl;
			l = x;
		}
	}
	return l;
}

int Good::getbestclick(int lastcnt){
	int l = -1, bs = -1;
	for (auto it = gp.begin(); it != gp.end(); it++){
		int x = it->first;
		int sl = it->second.getclickbd(lastcnt);
		if (sl > bs){
			bs = sl;
			l = x;
		}
	}
	return l;
}

set<int> Good::getbestsell(set<int> &gset, int lastcnt){
	if (gset.size() <= lastcnt) return gset;

	vector<pair<int, int> > srt;
	for (auto x = gset.begin(); x != gset.end(); x++){
		srt.push_back(make_pair(gp[*x].sellcnt, *x ));
	}
	sort(srt.rbegin(), srt.rend());
	set<int> res;
	for (int i = 0; i<lastcnt; i++){
		res.insert(srt[i].second);
	}
	return res;
}

bool GProfile::willbuyagain(UProfile &upf){
	if (upf.buydays[x] == 0) return false;

	int ubuydays = upf.buydays[x];
	int ubuyamnt = upf.buyamnt[x];
	int ulastbuydate = upf.lastbuydate[x];
	int ulastbuyamnt = upf.lastbuyamnt[x];
	int ulastclickdate = upf.lastclickdate[x];

	double gdays_buyaga = days_buyagain.val();
	double gdbabamnt = days_buyagain_byamnt.val();

	if (ulastbuydate > CNS::MaxDay - 7) return true;
	if (ulastclickdate > ulastbuydate + 7 &&
			ulastclickdate >CNS::MaxDay - 7) return true;
	return buydaycnt > buyusercnt;
}
bool GProfile::willbuy(UProfile &upf){
//	if (upf.buydays[x]>0) return false;

	int uclickamnt = upf.clickamnt[x];
	int uclickday = upf.clickdays[x];
	int ulastclickdate = upf.lastclickdate[x];
	double uclick2buy = upf.days_click2buy.val();

	bool rs1 = false, rs2 = false, rs3 = false, rs4 = false;
	/*
	if (upf.getclickamnt(x, 1) == 1) return true;
	if (upf.getclickamnt(x, 2) == 2) return true;
	if (upf.getclickamnt(x, 6) == 3) return true;
	if (upf.getclickamnt(x, 15) > 3) return true;
	*/

	/*
	if (upf.getmostclick(x, 1) == 2) return true;
	if (upf.getmostclick(x, 6) == 3) return true;
	if (upf.getmostclick(x, 19) > 3) return true;

	if (upf.getmostclick(x, 1) == 2) return true;
	if (upf.getmostclick(x, 4) == 3) return true;
	if (upf.getmostclick(x, 6) == 4) return true;
	if (upf.getmostclick(x, 13)== 5) return true;
	if (upf.getmostclick(x, 25) > 5) return true;
	*/
	if (upf.getmostclick(x, 1) == 2) rs1 = true;
	if (upf.getmostclick(x, 6) == 3) rs1 = true;
	if (upf.getmostclick(x, 19) > 3) rs1 = true;

	if (upf.getmostclick(x, 1) == 2) rs2 = true;
	if (upf.getmostclick(x, 4) == 3) rs2 = true;
	if (upf.getmostclick(x, 6) == 4) rs2 = true;
	if (upf.getmostclick(x, 13)== 5) rs2 = true;
	//if (upf.getmostclick(x, 25) > 5) rs2 = true;
	if (upf.getmostclick(x, 1) == 2) rs3 = true;
	if (upf.getmostclick(x, 3) == 3) rs3 = true;
	if (upf.getmostclick(x, 7) > 3) rs3 = true;

	for (int i = upf.ghis[x].size()-1; i>=0; i--){
		if (upf.ghis[x][i].d>CNS::MaxDay-3 &&
				upf.ghis[x][i].car > 0) rs3 = true;
		if (upf.ghis[x][i].d>CNS::MaxDay-5 &&
				upf.ghis[x][i].save > 0) rs3 = true;
	}

	return rs3;

	/*
	if (getclickbd(7)*2 >= getclickbd(14)*3 && getclickbd(7)>=5) return true;
	*/

	return false;
}
set<int> Good::willbuyagain(UProfile &upf){
	set<int> ret;
	for (auto it = upf.buydays.begin(); it!=upf.buydays.end(); it++){
		if (it->second ==0) continue;

		if (gp[it->first].willbuyagain(upf)){
			ret.insert(it->first);
		}
	}
	return ret;
}
set<int> Good::willbuy(UProfile &upf){
	set<int> ret;
	for (auto it = upf.ghis.begin(); it!=upf.ghis.end(); it++){

		assert(gp[it->first].x == it->first);
		if (gp[it->first].willbuy(upf)){
			ret.insert(it->first);
		}
	}
	return ret;
}
set<int> Good::willbuytoo(UProfile &upf){
	set<int> res;

	int maxjt = -1, maxx;
	for (auto it = upf.buy.begin(); it != upf.buy.end(); it++){
		int x = *it;
		for (auto jt = co_buy[x].begin(); jt != co_buy[x].end(); jt++){
			int y = jt->first;
			if (jt->second >= maxjt){
				if (upf.buy.find(y) == upf.buy.end()){
					maxjt = jt->second;
					maxx = y;
				}
			}
			if (jt->second >= 2){
				if (upf.click.find(y) ==  upf.click.end()){
					//res.insert(y);
				}
			}
		}
	}
	/*
	if (maxjt>=0){
		res.insert(maxx);
	}
	*/
	return res;
}

set<int> Good::test(UProfile &upf, User &u){
	set<int> res;
	for (auto xt = upf.ghis.begin(); xt != upf.ghis.end(); xt++){
		int x = xt->first;
		auto &hs = xt->second;
		double score = 0;
		for (int i = 0; i<hs.size(); i++){
			score += (hs[i].d/7+1)*(hs[i].click*0.1 + hs[i].car + hs[i].save + hs[i].buy*2);
		}
		//score /= CNS::MaxDay;
		if (score>=32.12) {
		//cerr<<upf.id<<"_"<<x<<' '<<score<<endl;
			res.insert(x);
		}
	}
	return res;
}

#endif
