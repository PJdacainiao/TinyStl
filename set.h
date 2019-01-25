/*源码剖析里基类和self的作用TT...
暂且将set中的迭代器设置为非常量迭代器*/
#ifndef __SET_H_
#define __SET_H_
#include "Allocator.h"
#include "Function.h"
#include "RBtree.h"
namespace TinyStl {

	template<class Key,class Compare=less<Key>, class Alloc=allocator<rb_tree_node<Key>>>
	class set {
	public:
		typedef Key Key_type;
		typedef Key value_type;
	private:
		template<class Key>
		struct key_is_value :public unary_function<Key, Key> {
			const Key& operator() (const Key& k) const { return k; }
		};
		typedef rb_tree<Key, Key,key_is_value<Key>, Compare, Alloc> RBtree;
		RBtree t;
	public:
		//实际上只有const_iterator,const_pointer和const_reference
		typedef typename RBtree::iterator iterator;
		typedef typename RBtree::const_iterator const_iterator;
		typedef typename RBtree::pointer pointer;
		typedef typename RBtree::const_pointer const_pointer;
		typedef typename RBtree::reference reference;
		typedef typename RBtree::const_reference const_reference;
		typedef typename RBtree::size_type size_type;
		typedef typename RBtree::difference_type difference_type;
	public:
		set() : t() {}
		template<class InputIterator>
		set(InputIterator first, InputIterator last) : t() {  t.insert_unique(first, last); }
		set(const RBtree& t1) : t(t1) {}
		set(const set& s) : t(s.t) {}
		set& operator=(const set& s) {
			t = s.t;
			return *this;
		}
		iterator begin() { return t.begin(); }
		const_iterator begin() const { return RBtree::change_iterator_to_const(t.begin()); }
		iterator end() { return  t.end(); }
		const_iterator end() const { returnRBtree::change_iterator_to_const(t.end()); }
		bool empty() { return t.empty(); }
		size_type size() { return t.size(); }
		pair<iterator, bool> insert(const value_type& x) {
			return  t.insert_unique(x);
		}
		template<class InputIterator>
		void insert(InputIterator first, InputIterator last) {
			t.insert_unique(first, last);
		}
		void erase(iterator& it) { return t.erase(it); }
		iterator find(const value_type& x) { return t.find(x); }
	};
}
#endif
