#ifndef __DEQUE_IMPL_H_
#define __DEQUE_IMPL_H_

namespace TinyStl {
	template<class T>
	typename deque_iterator<T>::iterator& deque_iterator<T>::operator++() {
		++curr;
		if (curr == last) {
			set_node(node + 1);
			curr = first;
		}
		return *this;
	}
	template<class T>
	typename deque_iterator<T>::iterator deque_iterator<T>::operator++(int) {
		iterator tmp = *this;
		++*this;
		return tmp;
	}
	template<class T>
	typename deque_iterator<T>::iterator& deque_iterator<T>::operator--()  {
		if (curr == first) {
			set_node(node - 1);
			curr = last;
		}
		--curr;
		return *this;
	}
	template<class T>
	typename deque_iterator<T>::iterator deque_iterator<T>::operator--(int)  {
		iterator tmp = *this;
		--*this;
		return tmp;
	}
	template<class T>
	typename deque_iterator<T>::iterator& deque_iterator<T>::operator+=(difference_type n) {
		difference_type offset = n + (curr-first);
		if (offset >= 0 && offset < difference_type(deque<T>::buffer_size))
			curr+= n;
		else {
			difference_type node_offset = (offset > 0) ? offset / difference_type(deque<T>::buffer_size) : -difference_type((-offset - 1) / deque<T>::buffer_size)-1;
			set_node(node + node_offset);                                                           
			curr = first + (offset - node_offset * difference_type(deque<T>::buffer_size));
		}
		return *this;
	}
	template<class T>
	typename deque_iterator<T>::iterator deque_iterator<T>::operator+(difference_type n) {
		iterator tmp = *this;
		return tmp += n;
	}
	template<class T>
	typename deque_iterator<T>::iterator& deque_iterator<T>::operator-=(difference_type n) {
		return *this += (-n);
	}
	template<class T>
	typename deque_iterator<T>::iterator deque_iterator<T>::operator-(difference_type n) {
		iterator tmp = *this;
		return tmp -= n;
	}
	template<class T>
	bool deque_iterator<T>::operator==(const iterator& it) const {
		return (node == it.node)&&(curr == it.curr);
	}
	template<class T>
	bool deque_iterator<T>::operator!=(const iterator& it) const {
		return !(*this == it);
	}
	template<class T>
	bool deque_iterator<T>::operator<(const iterator& it) const {
		return node < it.node || (node == it.node&&curr < it.curr);		
	}
	template<class T>
	void deque_iterator<T>::set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(deque<T>::buffer_size);
	}
	template<class T>
    typename deque_iterator<T>::difference_type operator-(const deque_iterator<T>& it1, const deque_iterator<T>& it2) {
		return (it1.node - it2.node - 1)*deque<T>::buffer_size + (it1.curr - it1.first) + (it2.last - it2.curr);
	}
	template<class T>
	deque<T>::deque() {
		create_map(0);	
	}
	template<class T>
    deque<T>::deque(const size_type& n, const value_type& x) {
		deque_aux(first, last, std::true_type);
	}
	template<class T>
	template<class InputIterator>
	deque<T>::deque(InputIterator first, InputIterator last) {
		deque_aux(first, last, std::is_integral<InputIterator>::type());
	}
	template<class T>
	deque<T>::deque(const deque& dq) {
	    create_map(dq.size());
		deque_aux(dq.start, dq.finish, std::is_integral<iterator>::type());
	}
	template<class T>
	typename deque<T>::deque& deque<T>::operator=(const deque& dq) {
		if (*this != dq) {
			map_pointer ptr = start.node;
			while (ptr != finish.node) {
				node_allocator::destroy(*ptr, *ptr + buffer_size);
				node_allocator::deallocate(*ptr, buffer_size);
				++ptr;
			}
			node_allocator::destroy(finish.first, finish.curr);
			node_allocator::deallocate(finish.first, buffer_size);
			map_allocator::deallocate(map, map_size);
			create_map(dq.size());
			deque_aux(dq.start, dq.finish, std::is_integral<iterator>::type());
		}
		return *this;
	}
	template<class T>
	deque<T>::~deque() {
		map_pointer ptr=start.node;
		while(ptr!=finish.node){
			node_allocator::destroy(*ptr,*ptr+buffer_size);
			node_allocator::deallocate(*ptr, buffer_size);
			++ptr;
		}
		node_allocator::destroy(finish.first, finish.curr);
		node_allocator::deallocate(finish.first, buffer_size);
		map_allocator::deallocate(map, map_size);
	}
	template<class T>
	typename deque<T>::size_type deque<T>::size() const {
		return (finish.node - start.node - 1)*buffer_size + (finish.curr - finish.first) + (start.last - start.curr);
	}
	template<class T>
	void deque<T>::push_back(const value_type& x) {
		if (finish.curr != finish.last - 1) {
			construct(finish.curr, x);
			++finish.curr;
		}
		else {
			if (map_size <= size_type(finish.node - map + 1))
				reallocate_map(false);
			*(finish.node+1) = create_node();
			construct(finish.curr, x); //注意，是前闭后开区间，先对curr构造，在++curr
			++finish;
		}
	}
	template<class T>
	void deque<T>::push_front(const value_type& x) {
		if (start.curr!= start.first ) {
			construct(start.curr-1, x);
			--start.curr;
		}
		else {
			if (start.node - map == 0)
				reallocate_map(true);
			*(start.node - 1) = create_node();
			--start;
			construct(start.curr, x);
		}
	}
	template<class T>
	void deque<T>::pop_front() {
		if (start.curr - 1 != start.last) {
			node_allocator::destroy(start.curr);
			++start.curr;
		}
		else {
			destroy(start.curr);
			node_allocator::deallocate(start.first, buffer_size);
			start.set_node(start.node+1);
			start.curr = start.first;
		}
	}
	template<class T>
	void deque<T>::pop_back() {
		if (finish.curr != finish.first) {
			--finish.curr;
			destroy(finish.curr);
		}
		else {
			node_allocator::deallocate(finish.first, buffer_size);//前闭后开区间，pop_back的其实是--finish.curr的值
			finish.set_node(finish.node - 1);
			finish.curr = finish.last - 1;
			destroy(finish.curr);
		}
	}
	template<class T>
	typename deque<T>::iterator deque<T>::erase(iterator position) {
		size_t n = position- start+1;
		if (n< (size() >> 1)) {
			copy_backward(start, position, position);
			pop_front();
		}
		else {
			copy(position + 1, finish, position);
			pop_back();
		}
		return start + n; //统一返回position的后一个位置
	}
	template<class T>
	typename deque<T>::iterator deque<T>::insert(iterator position, const value_type& x) {
		if (position == start) {
			push_front(x);
			return first;
		}
		if (position = finish) {
			push_back(x);
			return finish - 1;
		}
		else {
			return insert_aux(position, x);
		}
	}
	template<class T>
	typename deque<T>::iterator deque<T>::insert(iterator position,const  size_type& n, const value_type& x) {
		if (position == start) {
			for (int i=n; i > 0; --i)
				push_front(x);
			return start+n-1;
		}
		if (position == finish) {
			for (int i = n; i > 0; --i)
				push_back(x);
			return finish - 1;
		}
		else
			return insert_aux(position, n, x);
	}
	template<class T>
	template<class InputIterator>
	typename deque<T>::iterator deque<T>::insert(iterator position, InputIterator first, InputIterator last){
		return insert_aux(position, first, last, std::is_integral<InputIterator>::type());
	}

	template<class T>
	typename deque<T>::iterator deque<T>::insert_aux(iterator position, const value_type& x) {
		auto n = position - start;
		if (n < (size() >> 1)) {
			push_front(*start);
			copy(start + 2, start + n + 1, start + 1);
			iterator tmp = start + n ;
			*tmp = x;
			return tmp;
		}
		else {
			push_back(*(finish - 1));
			copy_backward(start + n, finish - 2, finish - 1);
			iterator tmp = start + n;
			*tmp = x;
			return tmp;
		}
	}
	template<class T>
	typename deque<T>::iterator deque<T>::insert_aux(iterator position, const size_type& n, const value_type& x) {
		auto index = position - start;
		if (index < (size() >> 1)) {
			for (int i = n; i > 0; --i)
				push_front(x);
			copy(start + n, start + n + index, start);
			for (iterator it = start + index; it != start + n + index; ++it)
				*it = x;
		}
		else {
			for (int i = n; i > 0; --i)
				push_back(x);
			copy_backward(start + index, finish - n, finish - 1);
			for (iterator it = start + index; it != finish - index; ++it)
				*it = x;
		}
		return start + index + n - 1;
	}
	template<class T>
	template<class InputIterator>
	typename deque<T>::iterator deque<T>::insert_aux(iterator position, InputIterator first, InputIterator last,std::true_type) {
		return insert_aux(position, first, last);
	}
	template<class T>
	template<class InputIterator>
	typename deque<T>::iterator deque<T>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		typedef typename iterator_traits<InputIterator>::difference_type Distance;
		Distance len = last - first;
		auto index = position - start;
		iterator iter = position;
		for (; first != last; ++first) {
			iter = insert_aux(iter, 1, *first);
			++iter;
		}
		return start + index - 1;
	}
	template<class T>
	void deque<T>::clear() {
		for (map_pointer current = start.node + 1; current < finish.node; ++current) {
			node_allocator::destroy(*current, *current + buffer_size);
			node_allocator::deallocate(*current, buffer_size);
		}
		if (start.node != finish.node) {
			node_allocator::destroy(start.curr, start.last);
			node_allocator::destroy(finish.first, finish.curr);
			node_allocator::deallocate(finish.first, buffer_size);
		}
		else {
			node_allocator::destroy(start.first, finish.curr);
		}
		finish = start;
	}
	template<class T>
	void deque<T>::create_map(size_t n) {
		size_type node_num =n/buffer_size + 1;
		map_size = node_num + 2; //首尾各加一个保证头插和尾插
		map = map_allocator::allocate(map_size);
		map_pointer mfirst = map+(map_size-node_num)/2;
		map_pointer mlast = mfirst +node_num-1;
		for (map_pointer current = mfirst; current<=mlast; ++current)
			*current = create_node();
		start.set_node(mfirst);
		start.curr = start.first;
		finish.set_node(mlast);
		finish.curr = finish.first + n % buffer_size;
	}
	template<class T>
	T *  deque<T>::create_node() {
		return node_allocator::allocate(buffer_size);
	}
	template<class T>
	void deque<T>::reallocate_map(bool add_at_front) {
		size_type old_node_num = finish.node - start.node + 1;
		size_type new_node_num = (map_size > 2 * old_node_num + 2) ? map_size : 2 * old_node_num + 2;
		if (new_node_num != map_size) {
			map_pointer new_map = map_allocator::allocate(new_node_num);
			map_pointer new_start = new_map + (new_node_num - old_node_num) / 2 + ((add_at_front) ? 1 : 0);//保持头尾未分配空间的严格大小
			copy(start.node, finish.node+1, new_start);
			map_allocator::deallocate(map,map_size);
			map = new_map;
			map_size = new_node_num;
			start.set_node(new_start);
			finish.set_node(new_start+old_node_num-1);
		}
		else {
			map_pointer new_start = map + (finish.node - start.node) / 2 + ((add_at_front) ? 1 : 0);
			if (size_type(new_start - start.node) < old_node_num / 2)
				copy(start.node, finish.node + 1, new_start);
			else
				copy_backward(start.node, finish.node + 1, (new_start + old_node_num - 1));
		}
	}
	template<class T>
	template<class InputIterator>
	void deque<T>::deque_aux(InputIterator first, InputIterator last,std::false_type) {
		typedef typename iterator_traits<InputIterator>::difference_type Distance;
		Distance len = last - first;
		create_map(size_type(len));
		for (iterator current = start; current != finish; ++current, ++first)
			construct(current.curr, *first);
	}
	template<class T>
	template<class InputIterator>
	void deque<T>::deque_aux(InputIterator first, InputIterator last, std::true_type) {
		create_map(first);
		for (map_pointer curr = start.node; curr != finish.node; ++curr)
			uninitialized_fill_n(*curr, buffer_size, last);
		uninitialized_fill(finish.first, finish.curr, last);
	}
}
#endif
