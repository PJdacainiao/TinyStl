#ifndef __HASH_TABLE_H_
#define __HASH_TABLE_H_

#include "Allocator.h"
#include "Construct.h"
#include "Algorithm.h"
#include "iterator.h"
#include "Vector.h"
#include "hash_function.h"
#include "utility.h"

namespace TinyStl {
	template<class T>
	struct hash_node {
		hash_node* next;
		T element;
	};
	template<class Key, class Value, class HashFunc, class ExtractKey>
	class hash_table;

	template<class Key, class Value, class HashFunc, class ExtractKey>
	class hash_iterator {
		template<class Key, class Value, class HashFunc, class ExtractKey>
		friend class hash_table;
	public:
		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef Value* pointer;
		typedef Value& reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef hash_node<Value>* node_ptr;
		typedef hash_iterator<Key, Value, HashFunc, ExtractKey> iterator;
		typedef hash_table<Key, Value, HashFunc, ExtractKey> hashTable;
	protected:
		node_ptr curr;
		hashTable* ht;
	public:
		hash_iterator() {}
		hash_iterator(node_ptr n, hashTable* h) : curr(n), ht(h) {}
		reference operator*() { return curr->element; }
		pointer operator->() { return &(operator*()); }
		iterator& operator++();
		iterator operator++(int);
		template<class Key, class Value, class HashFunc , class ExtractKey >
		friend bool operator==(const hash_iterator<Key,Value,HashFunc,ExtractKey >& i1, const hash_iterator<Key, Value, HashFunc, ExtractKey >& i2) { return i1.curr == i2.curr && i1.ht == i2.ht; }
		template<class Key, class Value, class HashFunc , class ExtractKey >
		friend bool operator!=(const hash_iterator<Key, Value, HashFunc, ExtractKey >& i1, const hash_iterator<Key, Value, HashFunc, ExtractKey >& i2) { return !(i1 == i2); }
	};
	template<class Key, class Value, class HashFunc = hash<Key, size_t>, class ExtractKey = key_of_value<Value>>
	class hash_table {
		template<class Key, class Value, class HashFunc, class ExtractKey>
		friend class hash_iterator;
	public:
		typedef size_t size_type;
		typedef Value value_type;
		typedef hash_node<Value>* nodePtr;
		typedef hash_iterator<Key, Value, HashFunc, ExtractKey> iterator;
		typedef allocator<hash_node<Value>> node_allocator;
	protected:
		HashFunc hash;
		ExtractKey key_of_value;
		vector<nodePtr> buckets;
		size_type elem_count;
	public:
		hash_table() { initialized(1); }
		hash_table(const HashFunc h, const ExtractKey ex) : hash(h), key_of_value(ex), elem_count(0) { initialized(1); }
		hash_table(const size_type n)  { initialized(n); }
		hash_table(const hash_table& h);
		hash_table& operator=(const hash_table& h);
		~hash_table();
		iterator begin();
		iterator end();
		pair<iterator, bool> insert_unique(const value_type& x);
		template<class InputIterator>
		void insert_unique(InputIterator first, InputIterator last);
		iterator insert_equal(const value_type& x);
		iterator erase(iterator p);
		iterator find(const value_type& x);
		void clear();
		//bool operator==(const hash_table& h) { return buckets == h.buckets; }
	protected:
		static const int list_size = 8;
		static const unsigned int bucket_size_list[list_size];

		void initialized(size_type n);
		size_type get_bucket_num(const Value& x) { return hash(key_of_value(x), buckets.size()); }
		size_type next_size(const size_type n);
		nodePtr create_node(const value_type& x);
		void destroy_node(nodePtr n);
		void resize(size_type n);
		pair<iterator, bool> insert_unique_no_resize(const value_type& x);
		void copy(const hash_table& h);
		
	};
}
#include "hash_table.impl.h"
#endif
