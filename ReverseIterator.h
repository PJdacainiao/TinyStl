#ifndef __REVERSE_ITERATOR_H_
#define __REVERSE_ITERATOR_H_
/*参照其他还能补充一些功能*/
#include "iterator.h"
namespace TinyStl {
	template<class Iterator>
	class reverse_iterator {
	protected:
		Iterator curr;
	public:
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type value_type;
		typedef typename iterator_traits<Iterator>::difference_type difference_type;
		typedef typename iterator_traits<Iterator>::pointer pointer;
		typedef typename iterator_traits<Iterator>::reference reference;

		typedef Iterator iterator_type;
		typedef reverse_iterator<Iterator> self;
	public:
		reverse_iterator() {}
		reverse_iterator(const Iterator& x) :curr(x) {}
		reverse_iterator(const self &x) :curr(x.curr) {}

		Iterator base() { return curr; }
		reference operator*() const {
			Iterator tmp = curr;
			return *(--tmp);
		}

		pointer operator->() const {
			return &(operator*());
		}

		self& operator++() {
			--curr;
			return *this;
		}
		self& operator++(int) {
			self tmp = *this;
			--curr;
			return tmp;
		}
		self& operator--() {
			++curr;
			return self;
		}
		self& operator--(int) {
			self tmp = *this;
			++curr;
			return tmp;
		}
		self operator+(difference_type n) const {
			return self(curr - n);
		}
		self operator-(difference_type n) const {
			return self(curr + n);
		}
		self& operator+=(difference_type n) {
			curr -= n;
			return *this;
		}
		self& operator-=(difference_type n) {
			curr += n;
			return *this;
		}
		reference operator[](difference_type n) const { return *(*this + n); }
	};
}

#endif
