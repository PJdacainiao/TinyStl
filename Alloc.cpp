#include "Alloc.h"
/*分配空间函数返回的指针都指向分配空间的start位置*/
namespace TinyStl {
	char *alloc::start = 0;
	char *alloc::end = 0;
	size_t alloc::heap_size = 0;
	alloc::obj* alloc::freelist[NFREELIST] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	void* alloc::allocate(size_t n) { 
		obj** myfreelist;
		obj* result;
		if (n >(size_t)MAX_BYTE)
			return malloc(n);
		myfreelist = freelist + FREELIST_INDEX(n);
		result = *myfreelist;
		if (result == 0) {
			return  refill(ROUND_UP(n));	
		}
		*myfreelist = result->next;   //更新freelist,分配的空间要使用，指向下一区块
		return result;
	}

	void alloc::deallocate(void *p, size_t n) {
		if (n > (size_t)MAX_BYTE) {
		    free(p);//释放的是malloc分配的内存，不是p所占的内存
			return;
		}
		obj** myfreelist;
		obj* q = (obj*)p;
		myfreelist = freelist + FREELIST_INDEX(n);  //回收空间插入已有节点之前
		q->next = *myfreelist;
		*myfreelist = q;
	}

	void* alloc::reallocate(void *p, size_t old_size, size_t new_size) {
		deallocate(p, old_size);
		p=allocate(new_size);
		return p;
	}
	void* alloc::refill(size_t n) {
		char* chunk;
		int objnum = 20;
		obj** myfreelist;
		obj* result,*curr_obj,*next_obj;
		int i;
		chunk = chunk_alloc(n, objnum); //从内存池分配空间，但可能小于20个节点（小区块）
		if (objnum == 1) //objnum引用传递，经过chunk_alloc可能发生改变
			return (obj*)chunk;
		myfreelist = freelist + FREELIST_INDEX(n);
		result = (obj*) chunk;//第一个要拿出来用
		*myfreelist = next_obj=(obj*)(chunk+n);
		for (i = 1; ++i;) {
			curr_obj = next_obj;
			next_obj = (obj*)((char*)next_obj + n);
			if (i = objnum - 1) {
				curr_obj->next = 0; break;
			}
			curr_obj->next = next_obj;
		}
		return result;
	}
	char* alloc::chunk_alloc(size_t n,int &objnum) {
		size_t totalsize = n * objnum;//分配总的字节数
		size_t leftsize = end-start; //memory pool的剩余长度
		char* result;
		if (leftsize > totalsize) {
			result = start;
			start = start + totalsize;
			return result;
		}
		else if (leftsize > n) {//不够分配objnum个小区块，则能分多少就分多少
			objnum = leftsize / n;
			totalsize = objnum * n;
			result = start;
			start = start + totalsize;
			return result;
		}
		else {
			size_t bytes_to_get = 2 * totalsize + ROUND_UP(heap_size >> 4);
			if (leftsize > 0) {//不够分一个size_t的小区块，则将该空间分配给小一些的节点区块，并插在已有区块节点的前边
				obj** myfreelist;
				myfreelist = freelist + FREELIST_INDEX(leftsize);
				((obj*)start)->next = *myfreelist;  
				*myfreelist = (obj*)start;
			}
			start =(char*) malloc(bytes_to_get);
			if (start == 0) //如果内存空间不足
			{
				size_t i;
				obj* p;
				obj** myfreelist;
				for (i = n; i <= MAX_BYTE; i += ALLIGN)
				{
					myfreelist = freelist + FREELIST_INDEX(i);
					p = *myfreelist;
					if (p != 0) {
						*myfreelist = (*myfreelist)->next;
						start = (char*)p;
						end = start + i;
					}
					return chunk_alloc(n, objnum);
				}
				end = 0;
				start = (char*)malloc(bytes_to_get);  //malloc有异常处理机制，会扔出exception
			}
			heap_size += bytes_to_get;
			end = start + bytes_to_get;
			return chunk_alloc(n, objnum);
		}

	}
}