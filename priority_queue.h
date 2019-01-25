#ifndef __PRIORITY_QUEUE_H_
#define __PRIORITY_QUEUE_H_

#include "Algorithm.h"
#include "Vector.h"
#include "Function.h"

namespace TinyStl {
	template<class T, class Compare=less<T>>
	class priority_queue {
	private:
		typedef vector<T> vec;
		vec v;
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef size_t size_type;
	
	public:
		priority_queue(): v() {}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last): v(first, last) { make_heap(v.begin(), v.end()); }
		priority_queue(const priority_queue& q) : v(q.v) {}
		priority_queue& operator=(const priority_queue& q) { v = q.v; return *this; }

		size_type size() { return v.size(); }
		bool empty() { return v.empty(); }
		void push(const value_type& x) { v.push_back(x); push_heap(v.begin(), v.end()); }
		void pop() { pop_heap(v.begin(), v.end()); v.pop_back(); }
		reference top() { return v.front(); }
	};
}

#endif
