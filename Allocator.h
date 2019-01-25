#ifndef __ALLOCATOR_H_
#define __ALLOCATOR_H_
/*没有写传递构造器的construct函数
该类内的construct函数与destroy函数与construct.h里的函数的区别（construct中的指针必须是allocate返回的指针ptr,construct.h中的placement new中的指针是任意的）
这边我觉得可以使用construct.h中的construct()和default()
*/
#include "Alloc.h"
#include "Construct.h"
#include <new>
namespace TinyStl {
	template<class T>
	class allocator {
	public:
		typename T value_type;
		typename T* pointer;
		typename T& reference;
		typename const T* const_pointer;
		typename const T& const_reference;
		typename const size_t size_type;
		typename ptrdiff_t deference_type;

	public:
		static T * allocate();
		static T* allocate(size_t n);
		static void deallocate(T *ptr);
		static void deallocate(T *ptr, size_t n);
		static void construct(T *ptr);
		static void construct(T *ptr, const T& value);
		static void destroy(T* ptr);
		static void destroy(T *first, T *last);
	};
	template<class T>
	T *allocator<T>::allocate() {
		return static_cast<T*>(alloc::allocate(sizeof(T)));
	}
	template<class T>
	T *allocator<T>::allocate(size_t n) {
		return static_cast<T*>(alloc::allocate(sizeof(T)*n));
	}
	template<class T>
	void allocator<T>::deallocate(T* ptr) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T));
	}
	template<class T>
	void allocator<T>::deallocate(T* ptr, size_t n) {
		alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
	}
	template<class T>
	void allocator<T>::construct(T* ptr) {
		new(ptr) T();
		//construct(ptr, T()); 
	}
	template<class T>
	void allocator<T>::construct(T* ptr, const T& value) {
		new(ptr) T(value);
	}
	template<class T>
	void allocator<T>::destroy(T* ptr) {
		ptr->~T();
	}
	template<class T>
	void allocator<T>::destroy(T*first, T*last) {
		for (; first != last; ++first)
			first->~T();
	}
}
#endif