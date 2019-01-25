#ifndef __HASH_MAP_H_
#define __HASH_MAP_H_

#include "hash_table.h"
namespace TinyStl {
	template<class T, class HashFunc=hash<T, size_t>>
	class hash_set {
	public:
		typedef T value_type;
		typedef size_t size_type;
	private:
		typedef hash_table<value_type, value_type, HashFunc> ht;
		ht t;
	public:
		typedef typename ht::iterator iterator;
		//typedef typename ht::const_iterator const_iterator;
	public:
		hash_set(): t() {}
		hash_set(const size_type n): t(n) {}
		template<class InputIterator>
		hash_set(InputIterator first, InputIterator last) : t() { t.insert_unique(first, last); }
		hash_set(const hash_set& hs): t(hs.t) {}
		hash_set& operator=(const hash_set& hs) { t = hs.t; return *this; }

		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		value_type front() { return *(t.begin()); }
		value_type back() { return *(--t.end()); }
		pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) { return t.insert_unique(first, last); }
		void clear() { t.clear(); }
		iterator find(const value_type& x) { return t.find(x); }
		iterator erase(iterator it) { return t.erase(it); }
	};
}

#endif
