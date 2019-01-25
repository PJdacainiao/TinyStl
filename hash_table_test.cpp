#include <iostream>
#include "Algorithm.h"
#include "Function.h"
#include "hash_table.h"

void hash_table_test() {
	TinyStl::hash_table<int, int> t1;
	int arr[10] = { 2,3,4,5,76,54,323,3 };
	t1.insert_unique(arr, arr +6);
	TinyStl::hash_table<int, int> t2(t1);
	TinyStl::hash_table<int, int> t3=t1;
	TinyStl::for_each(t3.begin(), t3.end(), TinyStl::display<TinyStl::hash_table<int, int>::iterator>());
	t2.insert_unique(4);
	t2.erase(t2.begin());
	t2.clear();
	TinyStl::for_each(t2.begin(), (t2.end()), TinyStl::display<TinyStl::hash_table<int, int>::iterator>());
	std::cout << std::endl;
	TinyStl::for_each(t1.begin(), (t1.end()), TinyStl::display<TinyStl::hash_table<int, int>::iterator>());
	system("pause");
}