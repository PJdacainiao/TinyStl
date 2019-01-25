#ifndef __HASH_TABLE_IMPL_H
#define __HASH_TABLE_IMPL_H

namespace TinyStl {
	template<class Key, class Value, class HashFunc, class ExtractKey>
	const unsigned int hash_table<Key, Value, HashFunc, ExtractKey>::bucket_size_list[list_size] = { 53, 97, 193, 389, 769, 1543, 3079, 6151 };


	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_iterator<Key, Value, HashFunc, ExtractKey>::iterator& hash_iterator<Key, Value, HashFunc, ExtractKey>::operator++() {
		node_ptr old = curr;
		value_type x = old->element;
		curr = curr->next;
		size_type n = ht->get_bucket_num(x);
		while (!curr && ++n < (ht->buckets.size())) {
			curr = ht->buckets[n];
		}
		return *this;
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_iterator<Key, Value, HashFunc, ExtractKey>::iterator hash_iterator<Key, Value, HashFunc, ExtractKey>::operator++(int) {
		iterator tmp = *this;
		++*this;
		return tmp;
	} 
	template<class Key, class Value, class HashFunc, class ExtractKey >
	hash_table<Key, Value, HashFunc, ExtractKey>::hash_table(const hash_table& h) {
		hash = h.hash;
		key_of_value = h.key_of_value;
		elem_count = h.elem_count;
		initialized(h.elem_count);
		copy(h);
    }

	template<class Key, class Value, class HashFunc, class ExtractKey >
	void hash_table<Key, Value, HashFunc, ExtractKey>::copy(const hash_table& h) {
		for (unsigned int i = 0; i < h.buckets.size(); ++i) {
			nodePtr first = h.buckets[i];
			if (first) {
				nodePtr new_node = create_node(first->element);
				buckets[i] = new_node;
				first = first->next;
			}
			else
				continue;
			nodePtr cur = buckets[i];
			while (first) {
				nodePtr new_node = create_node(first->element);
				cur->next = new_node;
				cur = cur->next;
				first = first->next;
			}
		}
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	hash_table<Key, Value, HashFunc, ExtractKey>& 
		hash_table<Key, Value, HashFunc, ExtractKey>::operator=(const hash_table& h) {
		if (this != &h) {     //比较地址更方便，不用定义vector,hash_table的==
			clear();
			hash = h.hash;
			key_of_value = h.key_of_value;
			elem_count = h.elem_count;
			copy(h);
		}
		return *this;
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	hash_table<Key, Value, HashFunc, ExtractKey>::~hash_table() {
		clear();//调用vector的析构函数，不用再定义
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	void hash_table<Key, Value, HashFunc, ExtractKey>::clear() {
		for (unsigned int  i = 0; i < buckets.size(); ++i) {
			nodePtr first = buckets[i];
			if (!first) continue;
			else {
			    first = first->next;
				while (first) {
					nodePtr follow = first->next;
					destroy_node(first);
					first = follow;
				}
				buckets[i] = nullptr;
			}
		}
		elem_count = 0;
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	pair<typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator, bool>
		hash_table<Key, Value, HashFunc, ExtractKey>::insert_unique(const value_type& x) {
		resize(1+elem_count);
		return insert_unique_no_resize(x);
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	template<class InputIterator>
	void hash_table<Key, Value, HashFunc, ExtractKey>::insert_unique(InputIterator first, InputIterator last) {
		for (; first != last; ++first)
			insert_unique(*first);
	}
	
	template<class Key, class Value, class HashFunc, class ExtractKey >
	typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator
		hash_table<Key, Value, HashFunc, ExtractKey>::insert_equal(const value_type& x) {
		resize(elem_num + 1);
		size_type index = hash(key_of_value(x), buckets, buckets.size());
		nodePtr first = buckets[index];
		for (nodePtr curr=first; curr != nullptr; ++curr) {
			if (curr->element == x) {
				nodePtr tmp = create_node(x);
				tmp->next = curr->next;
				curr->next = tmp;
				++elem_count;
				return iterator(curr, this);
			}
		}
			nodePtr tmp = create_node(x);
			tmp->next = first->next;
			first=tmp;
			++elem_count;
			return iteartor(tmp, this);
		
	}
	template<class Key, class Value, class HashFunc, class ExtractKey >
	void hash_table<Key, Value, HashFunc, ExtractKey>::resize(size_type n) {
		if (n > buckets.size()) {
			const size_type new_size = next_size(n);
			vector<nodePtr> tmp(new_size, nodePtr());
			for (size_type i = 0; i < buckets.size(); ++i) {
				nodePtr first = buckets[i];
				while (first) {
					size_type index = hash(key_of_value(first->element), n);
					buckets[index] = first->next;
					first->next = tmp[index];
					tmp[index] = first;
					first = buckets[index];
				}
			}
			buckets.swap(tmp);
		}
	}

	template<class Key, class Value, class HashFunc, class ExtractKey >
	pair<typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator, bool>
		hash_table<Key, Value, HashFunc, ExtractKey>::insert_unique_no_resize(const value_type& x) {
		size_type index = get_bucket_num(x);
		nodePtr first = buckets[index];
		nodePtr curr = first;
		while (curr) {
			if (curr->element == x)
				return pair<iterator, bool>(iterator(curr, this), false);
			else
				curr = curr->next;
		}
	    nodePtr node = create_node(x);
		node->next = first;
		buckets[index] = node;
		elem_count += 1;
		return pair<iterator, bool>(iterator(node, this), true);
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator hash_table<Key, Value, HashFunc, ExtractKey>::begin() {
		for (unsigned int i = 0; i < buckets.size(); ++i) {
			nodePtr first = buckets[i];
			if (first)
				return iterator(first, this);
		}
		return iterator(buckets[0], this);
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator hash_table<Key, Value, HashFunc, ExtractKey>::end() {
		for (int i = buckets.size() - 1; i >= 0; --i) {
			nodePtr curr = buckets[i];
			if (curr) {
				while (curr)
					curr = curr->next;
				return iterator(curr,this);
			}	
		}
		return  iterator(buckets[0],this);
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator hash_table<Key, Value, HashFunc, ExtractKey>::erase(iterator p) {
		value_type value = p.curr->element;
		size_type n = get_bucket_num(value);
		nodePtr pre= buckets[n];
		nodePtr cur_node= pre->next;
		if (!(pre->next)) {
			buckets[n] = pre->next;
			--elem_count;
			return iterator(cur_node,this);
		}
		while (!cur_node) {
			if (cur_node->element == value) {
				pre->next = cur_node->next;
				destroy_node(cur_node);
				--elem_count;
			}
			else {
				pre = cur_node;
				cur_node = cur_node->next;
			}	
		}
		return iterator(pre->next,this);
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::iterator hash_table<Key, Value, HashFunc, ExtractKey>::find(const value_type& x) {
		size_type n = get_bucket_num(x);
		nodePtr first = buckets[n];
		while (!first) {
			if (first->element != x)
				first = first->next;
		}
		return iterator(first,this);
	}

	template<class Key, class Value, class HashFunc, class ExtractKey>
	void hash_table<Key, Value, HashFunc, ExtractKey>::initialized(size_type n) {
		auto new_size = next_size(n);
		buckets.resize(new_size);
		buckets.insert(buckets.end(), new_size, nodePtr());
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::size_type  hash_table<Key, Value, HashFunc, ExtractKey>::next_size(const size_type n) {
		auto first = bucket_size_list;
		auto last = bucket_size_list + list_size;
		auto new_index = lower_bound(first, last, n);
		return new_index ? *(new_index) : bucket_size_list[list_size-1];
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	typename hash_table<Key, Value, HashFunc, ExtractKey>::nodePtr 
		hash_table<Key, Value, HashFunc, ExtractKey>::create_node(const value_type& x) {
		nodePtr tmp = node_allocator::allocate();
		tmp->next = nullptr;
		construct(&tmp->element, x);
		return tmp;
	}
	template<class Key, class Value, class HashFunc, class ExtractKey>
	void hash_table<Key, Value, HashFunc, ExtractKey>::destroy_node(nodePtr n) {
		node_allocator::destroy(n);
		node_allocator::deallocate(n);
	}
}
#endif

