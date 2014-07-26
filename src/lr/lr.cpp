#ifndef LR_CPP
#define LR_CPP
#include "lr.h"
LR::LR(){
	/* empty */
}
void LR::init(Good &g, User &u){
	/*
	w.resize(2);
	w[0] = -251.449;
	w[1] = 134.246;
	*/
	w.resize(7);
	w[0] = -2484.12;
	w[1] = w[2] = w[3] = w[4] = w[5] = 291.936;
	w[6] =  -392.83;
	return ;
	/*
	 * 1 2 4 8 16 user click amnt
	 * last 15 good click
	 */
	vector<vector<double> > x;
	vector<bool> y;
	for (auto it = u.up.begin(); it != u.up.end(); it++){
			UProfile &upf = it->second;
		for (auto jt = upf.ghis.begin(); jt != upf.ghis.end(); jt++){
			/* for each user, for each good it click */
			int uid = it->first, xid = jt->first;
			for (int d = 16; d<=CNS::MaxDay-15; d++){
				int v1 = upf.getclickamnt(xid, d, d);
				int v2 = upf.getclickamnt(xid, d-1, d);
				int v4 = upf.getclickamnt(xid, d-3, d);
				int v8 = upf.getclickamnt(xid, d-7, d);
				int v16 = upf.getclickamnt(xid, d-15, d);
				int g15 = g.gp[xid].getclickbd(d-14, d);
				x.push_back(vector<double>());
				x.back().push_back(v1);
				x.back().push_back(v2);
				x.back().push_back(v4);
				x.back().push_back(v8);
				x.back().push_back(v16);
				/*
				*/
				x.back().push_back(g15);
				y.push_back(upf.getbuydays(xid, d+1, d+30) > 0);
				if (y.back()){
					for (int i = 0; i<0; i++){
						x.push_back(x.back());
						y.push_back(y.back());
					}
				}
			}
		}
	}
	ofstream fout("data/output/lrtrain.tmp");
	for (int i = 0; i<y.size(); i++){
		fout<<(y[i]?"+1":"-1");

		for (int j = 0; j<x[i].size(); j++){
			fout<<" "<<j+1<<":"<<log(x[i][j]+1);
		}
		fout<<endl;
	}
	fout.close();
	char arg[100];
	sprintf(arg, "./script/lrtrain.sh %d %d", y.size(), x[0].size());
	cout<<arg<<endl;
	FILE *pipe = popen(arg, "r");
	assert(pipe);
	w.clear();
	for (int i = 0; i<x[0].size()+1; i++){
		double wx;
		fscanf(pipe, "%lf", &wx);
		w.push_back(wx);
	}
	pclose(pipe);
	for (int i = 0; i<w.size(); i++) cout<<w[i]<<' ';cout<<endl;
}
set<int> LR::pred(Good &g, UProfile &upf){
	set<int> res;
	for (auto it = g.gp.begin(); it != g.gp.end(); it++){
		int xid = it->first;
		int v1 = upf.getclickamnt(xid, 0);
		int v2 = upf.getclickamnt(xid, 2);
		int v4 = upf.getclickamnt(xid, 4);
		int v8 = upf.getclickamnt(xid, 8);
		int v16 = upf.getclickamnt(xid, 16);
		int g15 = g.gp[xid].getclickbd(15);
		double r = w[0] + v1*w[1] + v2*w[2] + v4 * w[3] + v8*w[4] + v16*w[5] + g15*w[6];
		if (r > 0) res.insert(xid);
	}
	return res;
}

#endif
