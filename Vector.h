#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "Allocator.h"
#include "Algorithm.h"
#include "Construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "ReverseIterator.h"
#include "uninitialized_func.h"

namespace TinyStl {
	template<class T, class Alloc=allocator<T>>
	class vector {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef pointer iterator;
		typedef const T* const_iterator;
		typedef reverse_iterator<iterator> r_iterator;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		typedef Alloc data_allocator;
	protected:
		iterator start;
		iterator finish;
		iterator end_of_storage;
	
	//构造，拷贝，赋值，析构函数
	public:
		vector() : start(0), finish(0), end_of_storage(0) {}
		explicit vector(size_type n);
		vector(size_type n, const value_type& value);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector& );
		vector& operator=(const vector&);
		vector(vector &&) noexcept;
		vector operator=(vector&&) noexcept;
		~vector();

	    //比较运算符重载
		bool operator==(const vector &v1) const;
		bool operator!=(const vector &v1) const;
	
	    //迭代器函数
		iterator begin() { return start; }
		iterator end() { return finish; }
		iterator rbegin() { return r_iterator(start); }
		iterator rend() { return r_iterator(start); }
		const_iterator cbegin() const { return start; }
		const_iterator cend() const { return finish; }
	

		//访问元素
		reference front() { return *start; }
		reference back() { return *--finish; }
		reference operator[](const size_type i) { return *(start + i); }
		const reference operator[](const size_type i) const { return *(start + i); }

		//容量
		size_type size() const { return size_type(finish-start); }
		size_type capacity() const { return end_of_storage - start; }
		bool empty() const { return start == finish; }
		
		//修改容器操作
		void push_back(const value_type &x);
		void pop_back();
		iterator insert(iterator position, const value_type &);
		iterator insert(iterator position,size_type n, const value_type &);
		template<class InputIterator>
		iterator insert(iterator position, InputIterator begin, InputIterator end);
		iterator erase(iterator i);
		iterator erase(iterator first, iterator last);
		void resize(size_type n, const value_type &x=value_type());
		void clear();
		void swap(vector v);
      //分配空间及填充元素
	protected:
		inline void allocate_and_fill(size_type n, const value_type& value);
		template<class InputIterator>
		inline void allocate_and_copy(InputIterator first, InputIterator last);
		inline void destroy_and_deallocate();
		inline void insert_aux(iterator position, size_type n=1, const value_type& x=value_type());
		template<class InputIterator>
		inline void insert_aux(iterator position, InputIterator first, InputIterator last,std::true_type);
		template<class InputIterator>
		inline void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		inline void vector_aux(InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		inline void vector_aux(InputIterator first, InputIterator last, std::false_type);
	};
}
#include "Vector.impl.h"
#endif
