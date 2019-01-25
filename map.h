#ifndef __MAP_H
#define __MAP_H
#include "Allocator.h"
#include "RBtree.h"
#include "Function.h"
namespace TinyStl {
	template<class Key, class T, class Compare = less<Key>, class Alloc = allocator<rb_tree_node<pair<Key, T>>>>
	class map {
	public:
		typedef Key key_type;
		typedef T data_type;
		typedef T map_type;
		typedef pair<Key, T> value_type;
	private:
		template<class value_type>
		struct key_is_first :public unary_function<Key, value_type> {
			const Key& operator()(const value_type& x) { return x.first; }
		};
		typedef rb_tree<key_type, value_type, key_is_first<value_type>, Compare, Alloc> rb;
		rb t;
	public:
		typedef typename rb::iterator iterator;
		typedef typename rb::const_iterator  const_iterator;
		typedef typename rb::pointer pointer;
		typedef typename rb::reference reference;

		map() : t() {}
		map(const map& m) : t(m.t) {}
		template<class InputIterator>
		map(InputIterator first, InputIterator last) { t.insert(first, last); }
		iterator begin() { return t.begin(); }
		iterator end() { return t.end(); }
		pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
		void erase(iterator& x) { t.erase(x); }
		T& operator[](const key_type& k) { return (*(insert(value_type(k, T())).first)).second; }
	};
}

#endif
