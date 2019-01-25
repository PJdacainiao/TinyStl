#ifndef __UTILITY_H_
#define __UTILITY_H_

namespace TinyStl {
	template<class T1, class T2>
	struct pair {
		typedef T1 type_1;
		typedef T2 type_2;
		T1 first;
		T2 second;
		pair() {}
		pair(const T1& v1, const T2& v2) : first(v1), second(v2) {}
		template<class U1, class U2>
		pair(const pair<U1, U2>& p): first(p.first), second(p.second) {}
		pair& operator=(const pair& p);
		friend bool operator==(const pair& p1, const pair& p2) { return p1.first == p2.first&&p1.second == p2.second; }
		friend bool operator!=(const pair& p1, const pair& p2) { return !(p1==p2); }
	};
	template<class T1, class T2>
	pair<T1, T2>& pair<T1, T2>::operator=(const pair& p) {
		if (*this != p) {
			first = p.first;
			first = p.first;
		}
		return *this;
	}
}

#endif
