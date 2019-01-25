#ifndef __HASH_MAP_H_
#define __HASH_MAP_H_

#include "hash_table.h"
#include "Function.h"
#include "utility.h"

namespace TinyStl {
	template<class Key, class T, class HashFunc= hash<Key, size_t>>
	class hash_map {
	public:
		typedef Key key_value;
		typedef pair<Key, T> value_type;
	private:
		template<class value_type>
		struct select_key :public unary_function<Key, value_type> {
			const Key& operator() (const value_type& v) {return v.first; }
		};
		typedef hash_table<key_value, value_type, HashFunc, select_key<value_type>> ht;
		ht t;
	public:
		typedef typename ht::iterator iterator;

		hash_map() : t() {}
		hash_map(size_t n): t(n) {}
		template<class InputIterator>
		hash_map(InputIterator first, InputIterator last) : t() { t.insert(first, last); }
		hash_map(const hash_map& h) : t(h.t) {}
		hash_map& operator=(const hash_map& h) { t = h.t; return *this; }
		
		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) { t.insert_unique(first, last); }
		iterator find(const value_type& x) { return t.find(x); }
		iterator erase(iterator it) { return t.erase(it); }
		void clear() { return t.clear(); }
	};
}



#endif
