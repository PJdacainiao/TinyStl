#ifndef __ALLOCATOR_H_
#define __ALLOCATOR_H_
/*û��д���ݹ�������construct����
�����ڵ�construct������destroy������construct.h��ĺ���������construct�е�ָ�������allocate���ص�ָ��ptr,construct.h�е�placement new�е�ָ��������ģ�
����Ҿ��ÿ���ʹ��construct.h�е�construct()��default()
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