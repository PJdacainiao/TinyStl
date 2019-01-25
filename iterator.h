#ifndef __ITERATOR_
#define __ITERATOR_

namespace TinyStl {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	//泛型迭代器生成,一般容器都会定义自己专有的迭代器
	template<class category, class T, class Distance = ptrdiff_t, class Pointer = T * , class Reference = T & >
	struct iterator {
		typedef category iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};
	template<class T>
	struct  input_iterator {
		typedef input_iterator_tag iterator_category;
	};

	template< class T>
	struct output_iterator : public iterator<output_iterator_tag, T> {};
	template<class T>
	struct forward_iterator : public iterator<forward_iterator_tag, T> {};
	template<class T>
	struct bidirectonal_iterator : public iterator<bidirectional_iterator_tag, T> {};
	template< class T>
	struct random_access_iterator : public iterator<random_access_iterator_tag, T> {};
	//迭代器性质萃取
	template<class iterator>
	struct iterator_traits {
		typedef typename iterator::iterator_category iterator_category;
		typedef typename iterator::value_type value_type;
		typedef typename iterator::difference_type difference_type;
		typedef typename iterator::pointer pointer;
		typedef typename iterator::reference reference;
	};

	template<class T>//针对原生指针的偏特化版本，原生指针不是类类型
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};
	template<class T>//针对const原生指针的偏特化版本，原生指针不是类类型
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};

	template<class iterator>
	inline typename iterator_traits<iterator>::iterator_category 
		iterator_category(const iterator&) {
		return typename iterator_traits<iterator>::iterator_category();
	}
	template<class iterator>
	inline typename iterator_traits<iterator>::value_type*
		value_type(const iterator&) {
		return static_cast<typename iterator_traits<iterator>::value_type*>(0);
	}
	template<class iterator>
	inline typename iterator_traits<iterator>::distance_type*
		distance_type(const iterator&) {
		return static_cast<typename iterator_traits<iterator>::distance_type*>(0);
	}

}
#endif
