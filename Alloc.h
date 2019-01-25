#ifndef __ALLOC_H_
#define __ALLOC_H_

/*����ռ䣬Ҫ����Ŀռ�С��128bytesʱ�����ڴ�أ�����128bytes����malloc()*/
#include <cstdlib>

namespace TinyStl {
	class alloc{
	enum {ALLIGN=8};
	enum {MAX_BYTE=128};
	enum {NFREELIST=MAX_BYTE/ALLIGN};
	private:
		union obj{
			obj * next;
			char client_data[1];
		}; 
		static obj* freelist[NFREELIST]; //16�ڵ�
	private:
		static size_t ROUND_UP(size_t bytes) {
			return (bytes + ALLIGN - 1)&~(ALLIGN - 1);
		}
		static size_t FREELIST_INDEX(size_t bytes) {
			return (bytes + ALLIGN - 1) / ALLIGN - 1; //enum��int���ͣ�size_t��unsigned���⸺��
		}
		static void *refill(size_t n);
		static char* chunk_alloc(size_t n, int &objn);

		static char* start;
		static char* end;
		static size_t heap_size;

	public:
		static void* allocate(size_t n);
		static void deallocate(void *p, size_t n);
		static void* reallocate(void *p, size_t oldsize, size_t newsize);
	};	 
}
#endif
