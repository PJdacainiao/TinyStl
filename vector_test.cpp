/*VectorÈÝÆ÷µÄ²âÊÔ½Å±¾*/
#include <iostream>
#include "iterator.h"
#include "Algorithm.h"
#include "Function.h"
#include "Vector.h"
void vector_test() {
	int a[16] = { 1,6,2,4,4,5,7,12,6,9,23,1,5,3,7,8 };
	//auto i=TinyStl::search_n(a, a + 6, 1, 2);
	//std::cout << *(i) << std::endl;
	//auto t = TinyStl::unique(a, a + 6);
	//int a[7] = { 6, 7, 7, 12, 23, 9, 8 };
	//TinyStl::insertion_sort(a, a + 16);
	//TinyStl::for_each(a, a+16, TinyStl::display<int*>());
	//TinyStl::sort(a, a + 16);
	//TinyStl::for_each(a, a + 16, TinyStl::display<int*>());
	//TinyStl::vector<int> vec(16);
	//TinyStl::vector<int> vec(16,1);
	TinyStl::vector<int> vec(a, a + 16);
	TinyStl::vector<int> vec1;
	vec1 = vec;
	TinyStl::for_each(vec1.begin(), vec1.end(), TinyStl::display<TinyStl::vector<int>::iterator>());
	std::cout << std::endl;
	vec1.push_back(1);
	vec1.push_back(2);
	vec1.pop_back();
	auto position = vec1.insert(vec1.begin() + 5, 5, 10);
	vec1.insert(vec1.end(), 5, 20);
	vec1.insert(vec1.end() - 1, a, a + 6);
	vec1.erase(vec1.begin());
	vec1.erase(vec1.begin() + 2, vec1.begin() + 4);
	vec1.resize(40);
	TinyStl::for_each(vec1.begin(), vec1.end(), TinyStl::display<TinyStl::vector<int>::iterator>());
	std::cout << std::endl;
}