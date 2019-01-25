#ifndef __Algorithm_
#define __Algorithm_

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include "iterator.h"
#include "type_traits.h"
#include "ReverseIterator.h"
#include "Function.h"
namespace TinyStl {
	//-------------------数值算法----------------------
	template<class InputIterator,class T>
	T accumulate(InputIterator first, InputIterator last, T init) {
		for (; first != last; ++first)
			init += *first;
		return init;
	}
	template<class InputIterator, class T, class BinaryOperation>
	T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
		for (; first != last; ++first)
			init = binary_op(init, *first);
		return init;
	}
	template<class InputIterator, class OutputIterator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
		typedef typename iterator_traits<InputIterator>::value_type  value_type;
		if (first == last) return result;
		value_type value = *first;
		*result = value;
		while (++first != last) {
			value_type tmp = *first;
			*++result = tmp-value;
			value = tmp;	
		}
		return ++result;
	}
	template<class InputIterator, class OutputIterator>
	OutputIterator patial_sum(InputIterator first, InputIterator last, OutputIterator result) {
		typedef typename iterator_traits<InputIterator>::value_type  value_type;
		if (first == last) return result;
		*result = *first;
		value_type init = *first;
		while (++first != last) {
			++*result = init + *first;
			init = *result;
		}
	}
		template<class Type,class Interger>
		Type power(Type x, Interger n) {
			if (n == 0)  
				return x;
			Type result=1;
			while (n > 0) {
				result *= x;
				--n;
			}
			return result;
	}
	template<class ForwardIterator>
	typename iterator_traits<ForwardIterator>::difference_type distance(ForwardIterator first, ForwardIterator last) {
		typedef typename iterator_traits<ForwardIterator>::difference_type difference_value;
		difference_value n = 0;
		for (; first != last; ++first, ++n);
		return n;
	}

   //-------------基本算法-------------
	template<class InputIterator1, class InputIterator2>//完全相等
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
		typename iterator_traits<InputIterator1>::difference_type len1 = distance(first1,last1);
		typename iterator_traits<InputIterator2>::difference_type len2 = distance(first2,last2);
		if (len1 != len2) return false;
		if (len1 = len2) {
			while (len1 > 0) {
				if (*first1 == *first2)
				{
					++first1; ++first2;
					--len1;
				}
				else
					return false;
			}
		}
		return true;
	}

	template<class ForwardIterator,class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& value) {
		for (; first != last; ++first) 
			 *first = value;
	}
	template<class ForwardIterator, class size,class T> //使用assign，n可能超出范围
	ForwardIterator fill_n(ForwardIterator first, size n, const T& value) {
		for (; n > 0; --n, ++first) 
			*first = value;
		return first;
	}
	template<class ForwardIterator>
	inline void iter_swap(ForwardIterator iter1, ForwardIterator iter2) {
		typename iterator_traits<ForwardIterator>::value_type tmp = *iter1;
		*iter1 = *iter2;
		*iter2 = tmp;
	}
	template<class T>
	inline void swap(T& iter1, T& iter2) {
		T tmp = iter1;
		iter1 = iter2;
		iter2 = tmp;
	}
	template<class InputIterator1,class InputIterator2> //按照字典序比较两个范围元素
	void lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
		while (first1 != last1 && first2 != last2) {
			if(*first1==*first2)
			{
				++first1; ++first2;
			}
			else if (*first1 < *first2) 
				return true;
			else 
				return false;
		}
		return first1 == last && first2 != last2; //比较长度，长度小返回true,如果二者相等返回false
	}
	//-------------------------lexicographical-------------------------------
	template<class T>
	void lexicographical_compare(T* first1, T* last1, T* first2, T* last2) {
		size_t len1 = last1 - first1;
		size_t len2 = last2 - first2;
		const int result =memcmp(first1, first2, min(len1, len2));  //memcmp在cstring中
		return result != 0 ? result < 0 : len1 < len2;
	}
	//--------------------------------copy()---------------------------------
	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		if (first == last) return result;
		return _copy(first, last, result, iterator_category(first));
	}
	inline char* copy(const char* first, const char*last, char* result) {
		memmove(result,first, last-first); //memcpy()存在result介于first-last之间copy覆盖的风险
		return result + (last - first);
	}
	inline wchar_t* copy(const wchar_t* first, const wchar_t*last, wchar_t* result) {
		memmove(result, first, last - first); 
		return result + (last - first);
	}
	template<class InputIterator, class OutputIterator>
	inline OutputIterator copy_backward(InputIterator first, InputIterator last, OutputIterator result) {
		if (first == last)  return result;
		for (--last; last != first; --last, --result)
			*result = *last;
		*result++ = *first;
		return result;
	}
	template<class InputIterator, class OutputIterator>
	inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
		for (; first != last; ++first, ++result) {
			*result = *first;
		}
		return result;
	}
	template<class InputIterator, class OutputIterator>
	inline OutputIterator _copy(InputIterator first, InputIterator last, OutputIterator result, random_access_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n; 
		n = last-first;
		for (; n > 0; --n) { //random_iterator使用n循环速度快
   			*result++ = *first++;
		}
		return result;
	}
	template<class T>
	inline T* _copy(T* first, T* last, T* result) {
		return __copy(first, last, result, type_traits<T>::isPOD());
	}
	template<class T>
	inline T* __copy(T* first, T* last, T* result, true_type) {
		memmove(result, first, (last - first));
		return result + (last - first);
	}
	template<class T>
	inline T* __copy(T* first, T* last, T* result, false_type) {
		typename iterator_traits<InputIterator>::difference_type n; 
		n = first - last;
		for (; n > 0; ++result, ++first) { //random_iterator使用n循环速度快
			*result = *first;
		}
		return result;
	}
	//-------------------------set_Union---------------------------
	template<class InputIterator1,class InputIterator2,class OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {//最多含有max(m,n)个相同的元素
			if (*first1 > *first2) {
				*result = *first2;
				++first2;
			}
			else if (*first1 == *first2) { 
				*result = *first1;
				++first1;
				++first2;
			}
			else {
				*result = *first1;
				++first1;
			}
			++result;
		}
		return copy(first1, last1, copy(first2, last2, result)); //两者必有一个是空白的
	}
    //-----------------------------set_intersection----------------------------------
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				++first1;
			}
			else if (*first1 > *first2) {
				++first2;
			}
			else {
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
		}
		return result;
	}
	//---------------------------------------set_difference--------------------------------------------
	//S1-S2
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first1 == *first2) {
				++first1;
				++first2;
			}
			else {
				++first2;
			}
		}
		return copy(first1, last1, result);
	}
	//---------------------------------------set_symmetric_difference--------------------------------------------
	//出现在S1不出现在S2中元素与出现在S2不出现在S1中元素的并集
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first1 == *first2) {
				++first1;
				++first2;
			}
			else {
				*result = *first2;
				++first2;
				++result;
			}
		}
		return copy(first1, last1, copy(first2,last2,result));
	}
	//---------------------------percolate_up-----------------------------
	template<class RandomIterator>
	void percolate_up(RandomIterator first, RandomIterator last) { //[first, last)
		typedef iterator_traits<RandomIterator>::difference_type distance;
		distance hole_index = last - first -1;
		distance parent_index = (hole_index-1) / 2;
		while (parent_index>=0&&(*(first + hole_index)>*(first+ parent_index))) {
			iter_swap(first+hole_index, first+parent_index);
			hole_index = parent_index;
			parent_index = (hole_index-1) / 2;
		}
	}
	//--------------------------percolate_down------------------------------
	template<class RandomIterator>
	void percolate_down(RandomIterator first, RandomIterator last, RandomIterator head) { //[first,last)]first为整个序列的头，head为要下溯的起点
		typedef iterator_traits<RandomIterator>::difference_type distance;
		distance len = last - first;
		distance hole_index = head - first;
		distance second_index = (hole_index + 1) * 2;
		while (second_index < len) {
			RandomIterator lchild = first + second_index - 1;
			RandomIterator rchild = first + second_index;
			RandomIterator hole = first + hole_index;
			if (*lchild > *rchild) {
				TinyStl::iter_swap(lchild, hole);
				hole_index = second_index - 1;
			}
			else {
				TinyStl::iter_swap(rchild, hole);
				hole_index = second_index;
			}
			second_index = (hole_index + 1) * 2;
			}
		if (second_index == len) {
			iter_swap(first + second_index - 1, first + hole_index);
			hole_index = second_index - 1;
		}
	}
	template<class RandomIterator,class Comparation>
	void percolate_down(RandomIterator first, RandomIterator last,Comparation comp) {
		typedef iterator_traits<RandomIterator>::difference_type distance;
		distance len = last - first-1;
		distance hole_index = distance(0);
		distance child_index = first * 2 + 1;
		while (child_index < len) { //找的左儿子，不会出现找右儿子超出边界的问题
			if (child_index + 1 < len&&(*(first + child_index) < *(first + child_index + 1)))
				child_index = child_index + 1;
			RandomIterator child = first + child_index;
			RandomIterator hole = first + hole_index;
			if (comp(child, hole)) {
				iter_swap(child, hole);
				hole_index = child_index;
			}
			child_index = child_index * 2 + 1;
		}
	}
	//---------------------------push_heap()-------------------------------
	template<class RandomIterator>
	void push_heap(RandomIterator first, RandomIterator last) {
		percolate_up(first, last);
	}
	//---------------------------pop_heap()--------------------------------
	template<class RandomIterator>
	void pop_heap(RandomIterator first, RandomIterator last) {
		iter_swap(first, last - 1);
		if (last - first > 1) {
			percolate_down(first, last - 1, first);
		}
	}
	//---------------------------make_heap()------------------------------
	template<class RandomIterator>
	void make_heap(RandomIterator first, RandomIterator last) {
		typedef iterator_traits<RandomIterator>::difference_type distance;
		if (last - first < 2) return;
		distance index = (last - first) / 2-1;
		for (; index >= 0; --index) {
			percolate_down(first, last, first + index);
		}
	}
	//---------------------------sort_heap()--------------------------------
	template<class RandomIterator>
	void sort_heap(RandomIterator first, RandomIterator last) {
		typedef iterator_traits<RandomIterator>::difference_type distance;
		distance len = last - first-1;
		for (; len > 0; --len)
			pop_heap(first, last--);
	}
	//---------------------------adjacent_find------------------------------
	
	template<class InputIterator>
	InputIterator adjacent_find(InputIterator first, InputIterator last) {
		if (first == last) return last;
		InputIterator next= first;
		while (++next != last) {
			if (*first == *next)
				return first;
			else
				first = next;
		}
		return last;
	}
	//---------------------------count---------------------------------
	template<class InputIterator,class T>
	typename iterator_traits<InputIterator>::size_type count(InputIterator first, InputIterator last, const T& value) {
		typename  iterator_traits<InputIterator>::size_type n = 0;
		for (; first != last; ++first) {
			if (*first == value)
				++n;
		}
		return n;
	}
	template<class InputIterator, class Predicate>
	typename iterator_traits<InputIterator>::size_type count_if(InputIterator first, InputIterator last, Predicate pred){
		typename  iterator_traits<InputIterator>::size_type n = 0;
		for (; first != last; ++first) {
			if (pred(*first))
				++n;
		}
		return n;
	}
	//-----------------------------find----------------------------------
	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value) {
		for (; first != last && !pred(*first); ++first);	
		return first;
	}
	template<class InputIterator, class Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
		for (; first != last; ++first)
			if (pred(*first)) return first;
		return last;
	}
	//------------------------------------search--------------------------------------------
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
		while (first1 != last1) {
			ForwardIterator1 iter1 = first1;
			ForwardIterator2 iter2 = first2;
			while (iter2 != last2) {
				if (iter1 != last1 && *iter1 == *iter2) {
					++iter1; ++iter2;
				}
				else
					break;
			}
			if (iter2 == last2)
				return iter1;
			if (iter1 == last1)
				return last1;
			++first1;
		}
	}
	template<class ForwardIterator, class size,class T>
	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, size n, const T& value) {
		size i = n;
		for (; first != last&&i>0; ++first) {
			if (*first == value)
				--i;
			else
				i = n;
		}
		return i == 0 ? first : last;
	}

	//------------------------------------find_end--------------------------------------------
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
		typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
		typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
		return __find_end(first1, last1, first2, last2, category1(), category2());
	}
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,forward_iterator_tag,forward_iterator_tag) {
		if (first1 = last1) return last1;
		ForwardIterator1 result = last1;
		while (1) {
			ForwardIterator1 new_result = search(first1, last1, first2, last2);
			if (new_result = last1)
				return result;
			else {
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
	template<class BidirectionIterator1, class BidirectionIterator2>
	BidirectionIterator1 __find_end(BidirectionIterator1 first1, BidirectionIterator1 last1, BidirectionIterator2 first2, BidirectionIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
		//使用reverse iterator进行查找
		typedef  reverse_iterator<BidirectionIterator1> reverse1;
		typedef  reverse_iterator<BidirectionIterator2> reverse2;
		reverse1 rlast1(first1);
		reverse2 rlast2(first2);
		reverse1 rresult= search(reverse1(last1), rlast1, reverse2(last2),rlast2);
		if (rrsesult == rlast1)
			return last1;
		else {
			BidirectionIterator1 result = rresult.base();
			advance(result, -distance(first2, last2));
			return result;
		}
	}
	//--------------------------------find_first_of------------------------------------------
	template<class ForwardIterator1, class ForwardIterator2>
	ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
		for (; first1 != last1; ++first1)
			ForwardIterator2 result = first2;
		for (; result != last2; ++result)
			if (*first1 = *result)
				return first1;
	}
	//-----------------------------------for_each-------------------------------------------
	template<class InputIterator,class Function>
	void for_each(InputIterator first, InputIterator last, Function f) {
		for (; first != last; ++first)
			f(first);
	}
	//----------------------------------generate--------------------------------------------
	template<class ForwardIterator, class Function>
	void generate(ForwardIterator first, ForwardIterator last, Function f) {
		for (; first != last; ++first)
			*first=f();
	}
	template<class ForwardIterator, class size, class Function>
	ForwardIterator generate_n(ForwardIterator first, ForwardIterator last, size n, Function f) {
		for (; n>0; --n,++first)
			*first = f();
		return first;
	}
	//-----------------------------------includes----------------------------------------------
	template<class ForwardIterator1, class ForwardIterator2>
	bool includes(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 < *first2)
				++first1;
			else if (*first1 > *first2)
				return false;
			else {
				++first1;
				++first2;
			}
			return (first1 != last1)? true : false;
			//return first2==last2;//更直接
		}
	}
	//---------------------------------max&min_element----------------------------------------
	template<class ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
		ForwardIterator max = *first;
		for (; first != last; ++first) 
			if (*first > *max)
				max = first;
		return max;
	}
	template<class ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
		ForwardIterator min = *first;
		for (; first != last; ++first)
			if (*first < *min)
				min = first;
		return min;
	}
	//-----------------------------------merge--------------------------------------
	template<class InputIterator1,class InputIterator2,class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
		while (first1 != last1 && first2 != last2) {
			if (*first1 <= *first2) {
				*result = *first1;
				++first1;
			}
			else {
				*result = *first2;
				++first2;
			}
			++result;
		}
		return result;
	}
	//-------------------------------------partition----------------------------------------
	/*template<class BidrectionalIterator,class Predicate>
	BidrectionalIterator partition(BidrectionalIterator first, BidrectionalIterator last, Predicate pred) {
		while (first != last) {
			for (; first != last && pred(*first); ++first);
			for (; --last != first && !pred(*--last); --last);
			iter_swap(first, last);
			++first;
		}
		return first;
	}*/
	//-----------------------------------------remove-----------------------------------------
	template<class InputIterator, class OutputIterator, class T>
	OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
		for (; first != last; ++first) {
			if (*first != value) {
				*result = *first;
				++result;
			}
		}
		return result;
	}
	template<class InputIterator,class OutputIterator,class Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
		for (; first != last; ++first) {
			if (!pred(*first)) {
				*result = *first;
				++result;
			}
		}
		return result;
	}
	template<class InputIterator,  class T>
	InputIterator remove(InputIterator first, InputIterator last, const T& value) {
		InputIterator next = first;
		return first==last? first: remove_copy(next, last, first, value);
	}
	//------------------------------reverse---------------------------------
	template<class BidirectionalIterator>
	void reverse(BidirectionalIterator first, BidirectionalIterator last) {
		return _reverse(first, last, iterator_traits<BidirectionalIterator>::iterator_category());
	}
	template<class BidirectionalIterator>
	void _reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
			/*if (first == last || first == --last) return;
			for (; first != last; ++first,--last) {
				iter_swap(first, last);
			}*/ //错误，first,last可能交叉错开
		while (true) {
			if (first == last || first == --last) return;
			else
				iter_swap(first++, last);
		}
	}
	template<class Random_access_Iteartor>
	void _reverse(Random_access_Iteartor first, Random_access_Iteartor last, random_access_iterator_tag) {
		while (first < last) //只有random Iterator可以使用<比较
			iter_swap(first++, --last);	
	}
	//-----------------------------------------rotate-------------------------------------
	template<class ForwardIterator>
	void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
		return _rotate(first, middle, last, iterator_traits<ForwardIterator>::iterator_category());
	}
	
	template<class ForwardIterator>
	void _rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, forward_iterator_tag) {
		ForwardIterator i = middle;
		while (true) {
			iter_swap(first++, i++);
			if (first == middle) {
				if (i == last) return;
				middle = i;
			}
			else if (i == last)
				i = middle;   
		   }
	}
	template<class ForwardIterator>
	void _rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, bidirectional_iterator_tag) {
		reverse(first, middle);
		reverse(middle, last);
		reverse(first, last);
	}
	//-------------------------------unique----------------------------------
	template<class ForwardIterator>//仍然有残余数据
	ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
		ForwardIterator result = first;
		while(++first != last ) 
			if (*result != *first)
				*++result = *first;
		return ++result;
	}
	//-------------------------binary_search-------------------------------
	//-------------------------lower_bound--------------------------------
	//可插入value的第一个位置
	template<class ForwardIterator,class T>
	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,const T& value) {
		return __lower_bound(first, last, value, iterator_traits<ForwardIterator>::iterator_category());
	}
	template<class ForwardIterator, class T>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value,forward_iterator_tag) {
		typedef iterator_traits<ForwardIterator>::difference_type Distance;
		Distance len = distance(first,last);
		Distance half ;
		ForwardIterator middle;
		while (len>0) {
			half = len >> 1;
			middle = advance(first, half);
			if (*middle < value) {
					first = ++middle;
					len = len - half - 1;
			}
			else
				len = half;
		}
		return first;
	}
	template<class RandomIterator, class T>
	RandomIterator __lower_bound(RandomIterator first, RandomIterator last, const T& value, random_access_iterator_tag) {
		typedef typename iterator_traits<RandomIterator>::difference_type Distance;
		Distance len = last - first;
		Distance half;
		RandomIterator middle;
		while (len > 0) {
			half = len >> 2;
			middle = first + half;
			if (*middle < value) {
				first = ++middle;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return first;
	}
	//----------------------------------upper_bound--------------------------------------
	//可插入value的最后一个位置
	template<class ForwardIterator, class T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
		return __upper_bound(first, last, value, iterator_traits<ForwardIterator>::iterator_category());
	}
	template<class ForwardIterator, class T> 
	ForwardIterator _upper_bound(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
		typedef iterator_traits<ForwardIterator>::difference_type Distance;
		Distance len = distance(first, last);
		Distance half;
		ForwardIterator middle;
		while (len>0) {
			half = len >> 1;
			middle = advance(first, half);
				if (*middle > value) {
					len = half;
				}
				else {//=放在右侧处理
					first = ++middle;
					len = len-half-1;
				}
		}
		return first;
	}
	template<class RandomIterator, class T>
	RandomIterator _upper_bound(RandomIterator first, RandomIterator last, const T& value, random_access_iterator_tag) {
		typedef iterator_traits<RandomIterator>::difference_type Distance;
		Distance len = distance(first, last);
		Distance half;
		RandomIterator middle;
		while (len>0) {
			half = len >> 1;
			middle = advance(first, half);
				if (*middle > value) {
					len = half;
				}
				else {//=放在右侧处理
					first = ++middle;
					len = len - half - 1;
				}
		}
		return first;
	}
/*	template<class RandomIterator,class T>
	pair<RandomIterator, RandomIterator> equal_range(RandomIterator first, RandomIterator last,const T& value) {
		//lower_bound和upper_bound会重复比较一些地方，因此先找到某一值的两边
		auto len = last - first;
		auto half = len;
		while (len > 0) {
			half = len >> 1;
			auto middle = first + half;
			if (*middle < value) {
				first = ++middle;
				len = len - half - 1;
			}
			else if(*middle>value)
				len = half;
			else {
				auto left = lower_bound(first, middle, value);
				auto right = upper_bound(++middle, last, value);
				return pair<RandomIterator>(left, right);
			}
		}	
	}*/
	//------------------------------binary_search-----------------------------------
	template<class ForwardIterator, class T>
	ForwardIterator binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
		return __binary_search(first, last, value, iterator_traits<ForwardIterator>::iterator_category());
	}
	template<class ForwardIterator, class T>
	ForwardIterator __binary_search(ForwardIterator first, ForwardIterator last, const T& value, forward_iterator_tag) {
		typedef iterator_traits<ForwardIterator>::difference_type Distance;
		Distance len = distance(first, last);
		Distance half;
		ForwardIterator middle;
		while (len>0) {
			half = len >> 1;
			middle = advance(first, half);
				if (*middle == first)
					return middle;
				if (*middle < value) {
					first = ++middle;
					len = len - half - 1;
				}
				else
					len = half;
		}
		return last;
	}
	template<class RandomIterator, class T>
	RandomIterator __binary_search(RandomIterator first, RandomIterator last, const T& value, random_access_iterator_tag) {
		typedef iterator_traits<RandomIterator>::difference_type Distance;
		Distance len = distance(first, last);
		Distance half;
		RandomIterator middle;
		while (len>0) {
			half = len >> 1;
			middle = advance(first, half);
				if (*middle == first)
					return middle;
			if (*middle < value) {
				first = ++middle;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return last;
	}
	//-------------------------partial_sort--------------------------------
	template<class RandomIterator>
	void partial_sort(RandomIterator first, RandomIterator middle, RandomIterator last) {
		make_heap(first, middle);
		for (RandomIterator i=middle; i != last; ++i) {
			if (*i < *first) {
				iter_swap(first, i);
				percolate_down(first, last - 1);
				percolate_up(first, last - 1);
			}	
		}
	}
	//---------------------------insertion_sort------------------------------------
	template<class RandomIterator>
	void insertion_sort(RandomIterator first, RandomIterator last){
		RandomIterator iter1 = first;
		++iter1;
		for (; iter1 != last; ++iter1)
			for (RandomIterator iter2 = iter1; iter2!=first; --iter2) { //判断是否超出界
				if (*iter2 < *first) {
					iter_swap(first, iter2);
					break;
				}
				RandomIterator pre = iter2;
				if (*iter2 < *--pre)
					iter_swap(iter2, pre);
			}
	}
	template<class RandomIterator>
	void unguard_insertion_sort(RandomIterator first, RandomIterator last) {//不需要判断越界；使用__unguarded_linear_insert减少出界判断，因为最小值一定在边界处
		for (RandomIterator iter1 = first++; iter1 != last; ++iter1) {
			if (*iter1 < *first) {
				iter_swap(first, iter1);
				break;
			}
			RandomIterator last = iter1;
			RandomIterator pre = last;
			--pre;
			typename iterator_traits<RandomIterator>::value_type value = *iter1;
			while (*pre > value) {
				*last = *first;
				last = pre;
				--pre;
			}
			*last = value;

		}
	}
	//--------------------------------------quick_sort-----------------------------------
	template<class RandomIterator>
	typename iterator_traits<RandomIterator>::value_type midian_of_three(RandomIterator first, RandomIterator last) {
		auto middle = first + (last - first) / 2;
		--last;
		if (*first < *last)
			if (*first < *middle)
				if (*middle < *last)
					return *middle;
				else
					return *last;
			else
				return *first;
		else if (*middle < *first)
			if (*last < *middle)
				return *middle;
			else
				return *last;
		else
			return *first;
	}
	template<class RandomIterator,class T>
	RandomIterator partition(RandomIterator first, RandomIterator last,const T& value) {
		while (1) {
			while (*first<value) ++first;
			--last;
			while (*last > value) --last;
			if (first >= last) return first;
			iter_swap(first, last);
			++first;
		}
	}
	template<class RandomIterator>
	void sort(RandomIterator first, RandomIterator last) {
		if (last - first < 16)
			insertion_sort(first, last);
		else {
			auto pivot = midian_of_three(first, last);
			RandomIterator curr=partition(first, last, pivot);
			sort(first, curr);
			sort(curr, last);
		}
		return;
	}

			
}
#endif
