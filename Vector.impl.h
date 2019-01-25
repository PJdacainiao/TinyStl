#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

namespace TinyStl {
	//构造，拷贝，赋值，析构
	template<class T, class Alloc>
	vector<T, Alloc>::vector(size_type n) {
		allocate_and_fill(n, value_type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(size_type n, const value_type& value) {
		allocate_and_fill(n, value);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last) {
		//补充处理指针和数字之间区别的函数，测试初始化vector(16,1)时会自动使用该构造函数引发错误，将InputIterator自动识别成int类型：（
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& x) {
		allocate_and_copy(x.start, x.finish);
	}
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator=(const vector& v) {
		if (*this != v) {
			destroy_and_deallocate();
			allocate_and_copy(v.start, v.finish);//还要考虑自赋值以及释放旧内存的操作，参考代码似乎没有释放旧的内存?
		}
		return *this;
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector &&v) noexcept : start(v.start), finish(v.finish), end_of_storage(v.end_of_storage) {
		v.start = v.finish = v.end_of_storage = 0;
	}
	template<class T, class Alloc>
	vector<T, Alloc> vector<T, Alloc>:: operator=(vector&& v) noexcept {
		if (*this != v) {
			destroy_and_deallocate();//释放已有元素还是内存呢?
			start = v.start;
			finish = v.finish;
			end_of_storage = v, end_of_storage;
		}
		return *this;
	}
	template<class T, class Alloc>
	vector<T, Alloc>::~vector() {
		destroy_and_deallocate();
	}
	//比较运算
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator==(const vector& v) const {
		if (size() != v.size()) {
			return false;
		}
		else {
			auto ptr1 = start;
			auto ptr2 = v.start;
			for (; ptr1 != finish&& ptr2 != v.finish; ++ptr1, ++ptr2) {
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator!=(const vector<T, Alloc>& v) const {
		return !(*this==v);
	}

	//修改容器，插入删除
	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type &x) {
		if (finish != end_of_storage) {
			*finish = x;
			++finish;
		}
		else 
			insert_aux(end(), 1,x);
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back() {
		--finish;
		Alloc::destroy(finish);
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& x) {
		insert_aux(position, x);
		return position - 1;
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, size_type n, const value_type& x) {
		insert_aux(position, n, x);
		return position - n;
	}
	template<class T, class Alloc>
	template<class InputIterator>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, InputIterator begin, InputIterator end) {
		insert_aux(position, begin, end,std::is_integral<InputIterator>::type());
		return position-(end-begin);
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
		copy(position + 1, finish, position);
		--finish;
		Alloc::destroy(finish);
		return position;
	}
	template<class T, class Alloc>
	typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last) {
		auto i = copy(last, finish, first);
		Alloc::destroy(i, finish);
		finish = finish - (last-first);
		return first;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::clear() {
		destroy_and_deallocate();
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::resize(size_type n, const value_type& x) {
		if (n < size()) {
			Alloc::destroy(start + n, finish);
			finish = finish - n;
		}
		else if (n > size()) {
			if (n < size_t(end_of_storage - finish))
				uninitialized_fill_n(finish, n - (finish - start), x);
			else {
				iterator new_start = Alloc::allocate(2 * n);
				iterator i = copy(start, finish, new_start);
				iterator new_finish = uninitialized_fill_n(i, n, x);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + 2 * n;
			}
		}
		return;
	}
	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector v) {
		TinyStl::swap(start, v.start);
		TinyStl::swap(finish, v.finish);
		TinyStl::swap(end_of_storage, v.end_of_storage);
	}
	
	//分配并初始化空间
	template<class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill(size_type n, const value_type& value) {
		start = Alloc::allocate(n);
		finish = TinyStl::uninitialized_fill_n(start, n, value);
		end_of_storage = finish;
	}
	//分配一段空间并将另一迭代器区间内的数据复制到该区间
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last) {
		start = Alloc::allocate(size_type(last - first));
		finish = TinyStl::uninitialized_copy(first, last, start);
		end_of_storage = finish;
	}
	//销毁并释放内存
	template<class T, class Alloc>
	void vector<T, Alloc>::destroy_and_deallocate() {
		if (capacity() != 0) {
			Alloc::destroy(start, finish);
			Alloc::deallocate(start, capacity());
		}
	}
	template<class T, class Alloc>
	inline void vector<T, Alloc>::insert_aux(iterator position, size_type n, const value_type& x) {
		if (n < size_type(end_of_storage - finish)) {
			difference_type len1 = finish - position;
			if (size_type(len1)<n) {
				iterator new_finish=finish+n;
				copy_backward(position,finish,new_finish-1);
				for (; n > 0; --n, ++position)
					*position = x;
				finish =new_finish;
			}
			else {
				auto new_finish=uninitialized_copy(finish-n, finish, finish);
				auto iter1= copy_backward(position, finish-n, finish-1);
				std::cout << *position << std::endl;
				for (; n > 0; --n, ++position)
					*position = x;
				finish = new_finish;
			}
		}
		else {
			const size_type new_size = size() + n * 2;
			iterator new_start = Alloc::allocate(new_size);
			auto position1 = uninitialized_copy(start, position, new_start);
			auto position2 = uninitialized_fill_n(position1, n, x);
			auto new_finish = uninitialized_copy(position, finish, position2);
			destroy_and_deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
	template<class T, class Alloc>
	template<class InputIterator>
	inline void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::true_type) {
		insert_aux(position, first, last);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	inline void vector<T, Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
		typedef typename iterator_traits<InputIterator>::difference_type Distance; 
		Distance len = last - first;
		if (len < (end_of_storage - finish)) {
			if (len < (finish - position)) {
				auto new_finish=uninitialized_copy(finish-len, finish, finish);
				copy_backward(position, finish-len, finish - 1);
				copy(first, last, position);
				finish = new_finish;
			}
			else {
				auto index = uninitialized_copy(first + len, last, finish);
				auto new_finish = uninitialized_copy(position, finish, index);
				copy_backward(first, first + len, finish-1);
				finish = new_finish;
			}
		}
		else {
			size_type new_size = size() + 2 * len;
			iterator new_start = Alloc::allocate(new_size);
			auto position1 = uninitialized_copy(start, position, new_start);
			auto position2 = uninitialized_copy(first, last, position1);
			auto new_finish = uninitialized_copy(position, finish, position2);
			destroy_and_deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
	template<class T,class Alloc>
	template<class InputIterator>
	inline void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::true_type) {
		allocate_and_fill(first, last);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	inline void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
		allocate_and_copy(first,last);
	}

}
#endif
