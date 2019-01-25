#ifndef __FUNCTION_
#define __FUNCTION_
#include <iostream>
namespace TinyStl {
	template<class Arg, class Result>
	struct unary_function {
		typedef Arg argument_type;
		typedef Result result_type;
	};
	template<class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1 argument1_type;
		typedef Arg2 argument2_type;
		typedef Result result_type;
	};
	template<class Iterator>
	struct display : public unary_function<Iterator,Iterator> {
		void operator() (  Iterator& iter) { std::cout << *iter << ' '; }
	};
	template<class T>
	struct less : public binary_function<T,T,bool>{
		bool operator() (const T&x, const T&y) { return x < y; }
	};
	template<class T>
	struct greater : public binary_function<T, T, bool> {
		bool operator() (const T&x, const T&y) { return x < y; }
	};
	template<class T>
	struct key_of_value :public unary_function<T, T> {
		const T& operator() (const T& x) const { return x; }
	};
}
#endif
