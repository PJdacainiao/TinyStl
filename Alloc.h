#ifndef __ALLOC_H_
#define __ALLOC_H_

/*分配空间，要分配的空间小于128bytes时采用内存池，大于128bytes采用malloc()*/
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
		static obj* freelist[NFREELIST]; //16节点
	private:
		static size_t ROUND_UP(size_t bytes) {
			return (bytes + ALLIGN - 1)&~(ALLIGN - 1);
		}
		static size_t FREELIST_INDEX(size_t bytes) {
			return (bytes + ALLIGN - 1) / ALLIGN - 1; //enum是int类型，size_t是unsigned避免负数
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
