#ifndef __LIST_H_
#define __LIST_H_
/*������const��������ת��-->Ӱ�쵽��������Ϳ�����ֵ*/
#include "allocator.h"
#include "Algorithm.h"
#include "iterator.h"
#include "uninitialized_func.h"
namespace TinyStl {
	template<class T>
	struct list_node {
		list_node<T>* pre;
		list_node<T>* next;
		T data;
		list_node(list_node<T>* p,list_node<T>* n,const T& d) : pre(p), next(n), data(d) {}
	};
	template<class T>
	struct list_iterator {
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef bidirectional_iterator_tag iterator_category;
		typedef list_node<T>* nodePtr;

		nodePtr node;

		list_iterator() {}
	    list_iterator(const list_iterator& x) : node(x.node) {}
	    list_iterator(const nodePtr& n) : node(n) {} 
		bool operator==(const list_iterator& x) const { return node == x.node; }
		bool operator!=(const list_iterator& x) const { return !(*this == x); }
		reference operator*() const { return (*node).data; }
		pointer operator->() const { return &(operator*()); }
		list_iterator& operator++() { node = (*node).next; return *this; }
		list_iterator operator++(int) { nodePtr tmp = node; ++*this; return tmp; }
		list_iterator& operator--() { node = (*node).pre; return *this; }
		list_iterator operator--(int) { nodePtr tmp = node; --*this; return tmp; }
	};
	
	template<class T,class Alloc= allocator<list_node<T>>>
	class list {
		template<class T>
		friend struct list_Iterator;
	public:
		typedef list_node<T>* nodePtr;
		typedef  Alloc list_allocator;

		typedef  T value_type;
		typedef  size_t size_type;
		typedef list_iterator<T> iterator;
		typedef list_iterator<const T> const_iterator;
	protected:
		nodePtr node;
	public:
		//���죬��������ֵ������
		list() { initialized_node(); }
		list(size_t n, const value_type& x=value_type());
		template<class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator=(const list& l);
		~list();
		
		iterator begin() { return node->next; } //node->next��list_node<T>*,�����øķ���ֵ��ʼ��iterator
		iterator end() { return node; }
		const_iterator begin() const; //const this ʹ��
		const_iterator end() const;
		bool  empty() { return node->next == node; }
		size_type size();

		iterator push_back(const value_type& x);
		iterator push_front(const value_type& x);
		iterator insert(iterator position, const size_type& n,const value_type& x);
		template<class InputIterator>
		iterator insert(iterator position, InputIterator first, InputIterator last);
		iterator erase(iterator iter);
		void pop_front();
		void pop_back();
		void clear();
		void remove(const value_type& x);
		void unique(); //ֻ��ɾ���������ظ�Ԫ��
		void splice(iterator position, list& x); //��������list����,position������first��last֮�䣬��ͬ
		void splice(iterator position, iterator first, iterator last);
		void splice(iterator position, list& l, iterator i);
		void reverse();
		void merge(list&); //����list�����ȵ�������
		void swap(list& l);
		void sort();
		
	private:
		//����ռ䲢��ʼ��
		nodePtr create_node(const value_type& x);
		void destroy_node(nodePtr p);
		void initialized_node();
		iterator __insert(iterator position, const size_type& n, const value_type& x);
		template<class InputIterator>
		iterator __insert(iterator position, InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		iterator __insert(iterator position, InputIterator first, InputIterator last, std::false_type);
		void transfer(iterator position, iterator first, iterator last);
		const_iterator convert_iterator_to_const(iterator& iter) const;
	};
}
#include "list.impl.h"
#endif
