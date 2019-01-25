#ifndef __LIST_IMPL_H_
#define __LIST_IMPL_H_

namespace TinyStl {
	//构造，拷贝，赋值，析构
	template<class T,class Alloc>
	list<T,Alloc>::list(size_t n, const value_type& x) {
		initialized_node();
		__insert(end(), n, x);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	list<T,Alloc>::list(InputIterator first, InputIterator last) {
		initialized_node();
		__insert(end(), first, last, std::_Is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	list<T,Alloc>::list(const list& l) { //必须要定begin()和end()的const版本，传入const list使用const this调用成员函数,无法找到合适的const begin().
		initialized_node();
		auto first = l.node->next;
		auto last = l.node;
		for(;first!=last;first=first->next)
		    __insert(begin(),1,first->data);
	}
	template<class T, class Alloc>
	list<T, Alloc>& list<T, Alloc>::operator=(const list& l) {
		if (*this != l) {
			clear();
			auto first = l.node->next;
			auto last = l.node;
			for (; first != last; first = first->next)
				__insert(begin(), 1, first->data);
		}
		return *this;
	}
	template<class T, class Alloc>
	list<T, Alloc>::~list() {
		auto curr = node->next;
		while (curr != node) {
			auto tmp = curr;
			curr = curr->next;
			destroy_node(tmp);
		}
		destroy_node(node);	
	}
	//容器操作
	template<class T, class Alloc>
	typename list<T, Alloc>::size_type list<T,Alloc>::size() {
		auto first = begin();
		size_type n = 0;
		for (; first != end; ++first, ++n);
		return n;
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::const_iterator list<T,Alloc>::begin() const {
		auto tmp = (list* const)this;
		iterator iter(tmp->node->next);
		std::cout << *iter << std::endl;
		return  convert_iterator_to_const(iter); 
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::const_iterator list<T, Alloc>::end() const {
		auto tmp = (list* const)this;
		iterator iter(tmp->node);
		std::cout << *iter << std::endl;
		return  convert_iterator_to_const(iter);
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::push_back(const value_type& x) {
		return __insert(end(), 1, x);
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::push_front(const value_type& x) {
		return __insert(begin(), 1, x);
	}
	template<class T, class Alloc>
	typename list<T,Alloc>::iterator list<T,Alloc>::insert(iterator position, const size_type& n,const value_type& x) {
		return __insert(position, n, x);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	typename list<T,Alloc>::iterator list<T,Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		return __insert(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T, class Alloc>
	typename list<T,Alloc>::iterator list<T, Alloc>::erase(iterator iter) {
		nodePtr next_node = iter.node->next;
		iter.node->pre->next = iter.node->next;
		iter.node->next->pre = iter.node->pre;
		destroy_node(iter.node);
		return next_node;
	}
	template<class T, class Alloc>
	void list<T,Alloc>::pop_front() {
		erase(begin());
	}
	template<class T, class Alloc>
	void list<T,Alloc>::pop_back() {
		auto tmp = end();
		erase(--tmp);
	}
	template<class T, class Alloc>
	void list<T,Alloc>::clear() {
		auto curr = node->next;
		while (curr != node) {
			auto tmp = curr;
			curr = curr->next;
			destroy_node(tmp);
		}
		node->pre = node;
		node->next = node;
	}
	template<class T, class Alloc>
	void list<T,Alloc>::remove(const value_type& x) {
		auto curr=begin();
		auto last = end();
		while (curr != last) {
			if (*curr == x)
				erase(curr++);
			else
				++curr;
		}
	}
	template<class T, class Alloc>
	void list<T,Alloc>::unique() {
		auto first = begin();
		auto next = first;
		++next;
		if (first == end() || next == end()) return;
		while (next != end()) {
			if (*first == *next) {
				erase(next++); //next失效
			}
			else {
				first = next;
				++next;
			}
		}
	}
	template<class T, class Alloc>
	void list<T,Alloc>::splice(iterator position, list& x) {
		transfer(position, x.begin(), x.end());
	}
	template<class T, class Alloc>
	void list<T,Alloc>::splice(iterator position, iterator first, iterator last) {
		if(first!=last)
		  transfer(position, first, last);
	}
	template<class T, class Alloc>
	void list<T,Alloc>::splice(iterator position, list& l, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j)  return;
		transfer(position, i, j);
	}
	template<class T, class Alloc>
	void list<T,Alloc>::reverse() {
		auto first = begin();
		auto curr = first;
		++curr;
		auto end = end();
		if (first == end || ++curr = end) return;
		while (begin() != end) {
			transfer(begin(), curr, ++curr);
		}
	}
	template<class T, class Alloc>
	void list<T, Alloc>::merge(list& l){
		auto first1 = begin();
		auto last1 = end();
		auto first2 = l.begin();
		auto last2 = l.end();
		while (first1 != last1 && first2 != last2) {
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
				//transfer(first1, first2, ++first2); //不能使用，运行时先进行加法运算，之后才对三个参数进行拷贝构造
			}
			else
				++first1;
			}
		if (first2 != last2)
			transfer(last1, first2, last2);
	}
	template<class T, class Alloc>
	void list<T, Alloc>::swap(list& l) {
		TinyStl::swap(node, l.node);
	}
	template<class T, class Alloc>
	void list<T, Alloc>::sort() {
		if (node->next == node || node->next->next == node) return;
		list carry;
		list counter[64];
		int fill = 0;
		while (!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (int i = 1; i < fill; ++i)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}
	//private函数
	template<class T, class Alloc>
	typename list<T, Alloc>::nodePtr list<T, Alloc>::create_node(const value_type& x) {
		nodePtr position= list_allocator::allocate();
		list_allocator::construct(position, list_node<T>(nullptr, nullptr, x));
		return position;
	}
	template<class T, class Alloc>
	void list<T, Alloc>::destroy_node(nodePtr p) {
		list_allocator::destroy(p);
		list_allocator::deallocate(p);
	}
	template<class T, class Alloc>
	void list<T, Alloc>::initialized_node() {
		node = list_allocator::allocate();
		node->next = node;
		node->pre = node;
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::iterator list<T, Alloc>::__insert(iterator position,const size_type& n, const value_type& x) {
		auto i = n;
		for (; i > 0; --i) {
			auto new_node = create_node(x); //iterator是调用node的接口，一般在参数以及返回当中
			new_node->next = position.node;
			new_node->pre = position.node->pre;
			position.node->pre->next = new_node;
			position.node->pre = new_node;
		}
		return position.node->pre;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	typename list<T, Alloc>::iterator list<T, Alloc>::__insert(iterator position, InputIterator first, InputIterator last, std::true_type) {
		return __insert(position, size_type(first), last);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	typename list<T, Alloc>::iterator list<T, Alloc>::__insert(iterator position, InputIterator first, InputIterator last, std::false_type) {
		if (first == last)
			std::cout << "fuck" << std::endl;
		for (; first != last; ++first) {
			__insert(position, 1, *first);
		}
		return position.node->pre;
	}
	template<class T, class Alloc>
	void  list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			position.node->pre->next = first.node;
			last.node->pre->next = position.node;
			first.node->pre->next = last.node;
			nodePtr tmp = first.node->pre;
			first.node->pre = position.node->pre;
			position.node->pre = last.node->pre;
			last.node->pre = tmp;
		}
	}
	template<class T, class Alloc>
	typename list<T, Alloc>::const_iterator  list<T, Alloc>::convert_iterator_to_const(iterator& it) const {
		using nodeP = list_node<const T>*;
		nodePtr ptr = it.node;
		list_node<const T> newnode((nodeP)(ptr->pre), (nodeP)(ptr->next), ptr->data);
		std::cout << newnode.next << std::endl;
		std::cout << newnode.pre << std::endl;
		std::cout << &newnode << std::endl;
		return const_iterator(&newnode); //先前由于list_iterator构造函数的参数缺少const出错
	}
}
#endif