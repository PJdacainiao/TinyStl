#ifndef __UNINITIALIZED_FUNC_
#define __UNINITIALIZED_FUNC_
/*construct和destroy的补充，用于构造*/
#include "Allocator.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "Algorithm.h"

namespace TinyStl {
	template<class InputIterator,class ForwardIterator>
	inline ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, true_type) {
		//memcpy(result, first, (last - first) * sizeof(*first));
		copy(first, last, result);
		return result+(last-first);
	}
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, false_type) {
		for (; first != last; ++first)
			construct(result++, *first);
		return result;         
	}
	template<class InputIterator,class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		typedef type_traits<iterator_traits<ForwardIterator>::value_type>::is_POD_type is_POD;
		return uninitialized_copy_aux(first, last, result, is_POD());
	}
    

	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, true_type) {
		fill(first, last, value); //后面algorithm定义
		/*for (; first != last; ++first)
			construct(first, value);*/
		return first;
	}
	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_aux(ForwardIterator first, const T& value, false_type) {
		for(;first!=last;++first)
			construct(first, value);
		return first;
	}
	template<class ForwardIterator,class T>
	ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &value) {
		typedef type_traits<iterator_traits<ForwardIterator>::value_type>::is_POD_type is_POD;
		return uninitialized_fill_aux(first, last, value, is_POD());
	}
	
	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T &value,true_type) {
		return fill_n(first, n, value);
	}
	template<class ForwardIterator, class T>
	inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator first, size_t n, const T &value, false_type) {
		for (int i = 0; i < n; ++i)
			construct(first + i, value);
		return first + i;
	}
	template<class ForwardIterator,class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, size_t n, const T &value) {
		typedef type_traits<iterator_traits<ForwardIterator>::value_type>::is_POD_type is_POD;
		return  uninitialized_fill_n_aux(first,n, value, is_POD());
	}
}

#endif
