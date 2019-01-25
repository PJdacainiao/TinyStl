#ifndef __TYPE_TRAITS_
#define __TYPE_TRAITS_

namespace TinyStl {
	struct true_type{};
	struct false_type{};

	template<class T>
	struct type_traits {
		typedef false_type has_trivial_default_constructor;
		typedef false_type has_trivial_copy_cosntructor;
		typedef false_type has_trivial_assignment_operator;
		typedef false_type has_trivial_default_destructor;
		typedef false_type is_POD_type;
	};

	template<>
	struct type_traits<char> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<signed char> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<unsigned char> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<short> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<unsigned short> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<int> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<unsigned int> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<long> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<unsigned long> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<float> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<double> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<>
	struct type_traits<long double> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
	template<class T>//原生指针类型的偏特化
	struct type_traits<T*> {
		typedef true_type has_trivial_default_constructor;
		typedef true_type has_trivial_copy_cosntructor;
		typedef true_type has_trivial_assignment_operator;
		typedef true_type has_trivial_default_destructor;
		typedef true_type is_POD_type;
	};
}
#endif
