#ifndef __HUSH_FUNCTION_
#define __HUSH_FUNCTION_

namespace TinyStl {
	inline size_t char_to_int(const char* ch) {
		unsigned int n = 0;
		for (const char* c = ch; *c; ++c)
			n = (n >> 5) + *c;
		return n;
	}
	template<class T,class size_type> 
	struct hash {};
	template<class size_type> 
	struct hash<char*, size_type>{
		size_type operator() (const char* ch, size_type n) const { return char_to_int(ch) % n; }
	};
	template<class size_type> 
	struct hash<const char*, size_type> {
		size_type operator() (const char* ch, size_type n) const { return char_to_int(ch) % n; }
	};
	template<class size_type>
	struct hash<int, size_type> {
		size_type operator() (int x, size_type n) const { return x % n; }
	};
	template<class size_type>
	struct hash<unsigned int, size_type> {
		size_type operator() (unsigned int x, size_type n) const { return x % n; }
	};
	template<class size_type>
	struct hash<char, size_type> {
		size_type operator() (char x, size_type n) const { return x % n; }
	};
	template<class size_type>
	struct hash<short, size_type> {
		size_type operator() (short x, size_type n) const { return x % n; }
	};
	template<class size_type>
	struct hash<long, size_type> {
		size_type operator() (long x, size_type n) const { return x % n; }
	};
	template<class size_type>
	struct hash<unsigned long, size_type> {
		size_type operator() (unsigned long x, size_type n) const { return x % n; }
	};

}

#endif
