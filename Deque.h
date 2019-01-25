#pragma once
#ifndef __DEQUE_H_
#define __DEQUE_H_

#include "allocator.h"
#include "Alloc.h"
#include "iterator.h"
#include "uninitialized_func.h"
namespace TinyStl {
	template<class T>
	class deque;
	template<class T>
	struct deque_iterator{
		template<class T>
		friend class deque;
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef T** map_pointer;
		typedef deque_iterator<T> iterator;
	protected:
		T* curr;
		T* first;
		T* last;
		map_pointer node;
	public:
		deque_iterator(T* c=nullptr, T* f=nullptr, T* l = nullptr, map_pointer n = nullptr):curr(c), first(f), last(l), node(n) {}
		deque_iterator(const iterator& it): curr(it.curr), first(it.first), last(it.last), node(it.node) {}
		reference operator*() const { return *curr; }
		pointer operator->() const { return &(operator*()); }
		iterator& operator++();
		iterator operator++(int);
		iterator& operator--();
		iterator operator--(int);
		iterator& operator+=(difference_type n);
		iterator operator+(difference_type n);
		iterator& operator-=(difference_type n);
		iterator operator-(difference_type n);
		bool operator==(const iterator& it) const;
		bool operator!=(const iterator& it) const;
		bool operator<(const iterator& it) const;
	protected:
		void set_node(map_pointer new_node);
	public:
		template<class T>
		friend typename deque_iterator<T>::difference_type operator-(const deque_iterator<T>& it1, const deque_iterator<T>& it2);
	};
	template<class T>
	class deque {
		template<class T>
		friend struct deque_iterator;
	public:
		typedef T value_type;
		typedef size_t size_type;
		typedef deque_iterator<T> iterator;
		typedef deque_iterator<const T> const_iterator;
		typedef T** map_pointer;
		typedef allocator<T> node_allocator;
		typedef allocator<T*> map_allocator;
		static const int buffer_size = 64;
	protected:
		iterator start;
		iterator finish;
		map_pointer map;
		size_t map_size;
	public:
		deque();
		deque(const size_type& n, const value_type& x);
		template<class InputIterator>
		deque(InputIterator first, InputIterator last);
		deque(const deque& dq);
		deque& operator=(const deque&);
		~deque();

		size_type size() const;
		size_type capacity() const { return map_size * buffer_size; }
		bool empty() const { return start == finish; }
		iterator begin() { return start; }
		const_iterator begin() const;  //const迭代器的转化
		iterator end() { return finish; }
		const_iterator end() const;
		value_type front() { return *start; }
		value_type back() { return *finish; }
		void push_back(const value_type& x);
		void push_front(const value_type& x);
		void pop_back();
		void pop_front();
		iterator insert(iterator position, const value_type& x);
		iterator  insert(iterator position,const size_type& n, const value_type& x); //返回最后一个插入的位置
		template<class InputIterator>
		iterator insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator position);
		void clear();
	protected:
		void create_map(size_t node);
		T* create_node();
		void reallocate_map(bool add_at_front);
		iterator insert_aux(iterator position, const value_type& x);
		iterator insert_aux(iterator position, const size_type& n, const value_type& x);
		template<class InputIterator>
		iterator insert_aux(iterator position, InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		iterator insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		void deque_aux(InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		void deque_aux(InputIterator first, InputIterator last, std::true_type);
		
	};
}
#include "Deque.impl.h"
#endif
