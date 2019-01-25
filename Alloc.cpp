#include "Alloc.h"
/*����ռ亯�����ص�ָ�붼ָ�����ռ��startλ��*/
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
		*myfreelist = result->next;   //����freelist,����Ŀռ�Ҫʹ�ã�ָ����һ����
		return result;
	}

	void alloc::deallocate(void *p, size_t n) {
		if (n > (size_t)MAX_BYTE) {
		    free(p);//�ͷŵ���malloc������ڴ棬����p��ռ���ڴ�
			return;
		}
		obj** myfreelist;
		obj* q = (obj*)p;
		myfreelist = freelist + FREELIST_INDEX(n);  //���տռ�������нڵ�֮ǰ
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
		chunk = chunk_alloc(n, objnum); //���ڴ�ط���ռ䣬������С��20���ڵ㣨С���飩
		if (objnum == 1) //objnum���ô��ݣ�����chunk_alloc���ܷ����ı�
			return (obj*)chunk;
		myfreelist = freelist + FREELIST_INDEX(n);
		result = (obj*) chunk;//��һ��Ҫ�ó�����
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
		size_t totalsize = n * objnum;//�����ܵ��ֽ���
		size_t leftsize = end-start; //memory pool��ʣ�೤��
		char* result;
		if (leftsize > totalsize) {
			result = start;
			start = start + totalsize;
			return result;
		}
		else if (leftsize > n) {//��������objnum��С���飬���ֶܷ��پͷֶ���
			objnum = leftsize / n;
			totalsize = objnum * n;
			result = start;
			start = start + totalsize;
			return result;
		}
		else {
			size_t bytes_to_get = 2 * totalsize + ROUND_UP(heap_size >> 4);
			if (leftsize > 0) {//������һ��size_t��С���飬�򽫸ÿռ�����СһЩ�Ľڵ����飬��������������ڵ��ǰ��
				obj** myfreelist;
				myfreelist = freelist + FREELIST_INDEX(leftsize);
				((obj*)start)->next = *myfreelist;  
				*myfreelist = (obj*)start;
			}
			start =(char*) malloc(bytes_to_get);
			if (start == 0) //����ڴ�ռ䲻��
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
				start = (char*)malloc(bytes_to_get);  //malloc���쳣������ƣ����ӳ�exception
			}
			heap_size += bytes_to_get;
			end = start + bytes_to_get;
			return chunk_alloc(n, objnum);
		}

	}
}