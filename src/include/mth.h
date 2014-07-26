#ifndef MTH_H
#define MTH_H
#include "head.h"

template<class T>
set<T> operator -(const set<T> &a, const set<T> &b);
template<class T>
set<T> operator +(const set<T> &a, const set<T> &b);
template<class T>
set<T> operator &(const set<T> &a, const set<T> &b);

template<class T>
set<T> operator -(const set<T> &a, const set<T> &b){
	set<T> ret;
	for (auto ia = a.begin(), ib = b.begin(); ia!=a.end(); ia++){
		while (ib != b.end() && *ia > *ib){
			ib++;
		}
		if (ib == b.end()){
			ret.insert(ia, a.end());
			break;
		}
		if (*ia != *ib){
			ret.insert(*ia);
		}
	}
	return ret;
}
template<class T>
set<T> operator +(const set<T> &a, const set<T> &b){
	set<T> ret = a;
	ret.insert(b.begin(), b.end());
	return ret;
}
template<class T>
set<T> operator &(const set<T> &a, const set<T> &b){
	set<T> ret;
	for (auto ia = a.begin(), ib = b.begin(); ia!=a.end(); ia++){
		while (ib != b.end() && *ia > *ib){
			ib++;
		}
		if (ib == b.end()){
			break;
		}
		if (*ia == *ib){
			ret.insert(*ia);
		}
	}
	return ret;
}

template<class T, class S>
map<T, S>& operator +=(map<T, S> &a, set<int> &b){
	for (auto ib = b.begin(); ib != b.end(); ib++){
		a[*ib] += 1;
	}
	return a;
}

template<class T, class S>
map<T, S>& operator +=(map<T, S> &a, map<T, S> &b){
	for (auto ib = b.begin(); ib != b.end(); ib++){
		a[ib->first] += ib->second;
	}
	return a;
}

template<class T, class S>
set<T> getbest(map<T, S> &a, int get){
	vector<pair<S, T> > srt;
	for (auto it = a.begin(); it != a.end(); it++){
		srt.push_back(make_pair(it->second, it->first));
	}
	sort(srt.rbegin(), srt.rend());
	get = min((int)srt.size(), get);
	set<T> res;
	for (int i = 0; i<get; i++){
		res.insert(srt[i].second);
	}
	return res;
}

inline int getday(int m, int d){
	if (m == 4) return d - 15;
	if (m == 5) return d + 15;
	if (m == 6) return d + 15 + 31;
	if (m == 7) return d + 15 + 31 + 30;
	return d + 15 + 31 + 30 + 31;
}

#endif
