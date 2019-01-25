#ifndef _CONSTRUCT_H_
#define _CONSTRUCT_H_
/*构造和析构单个对象及对象序列*/
#include<new>
#include "type_traits.h"
namespace TinyStl {
	template<class p, class T>
	inline void construct(p* ptr, T& value) {
		new(ptr) T(value);
	}
	
	template<class p>
	inline void destroy(p* ptr) {
		ptr->~p();
	}
	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		__destroy(first,last, value_type(first));
	}

	template<class ForwardIterator,class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last,T*) {
		typedef typename  type_traits<T>::has_trivial_destructor trivial_distructor;
		__destroy_aux(first, last, trivial_distructor());
	}
    
	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, true_type) {}

	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator first, ForwardIterator last, false_type) {
		for (; first != last; ++first)
			destroy(&*first);
	}
}

#endif

