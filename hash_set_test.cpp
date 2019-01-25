#include <iostream>
#include "hash_set.h"
#include "Function.h"
#include "Algorithm.h"

void hash_set_test(){
	TinyStl::hash_set<int> h1;
	int arr[10] = { 2,3,4,5,6,4,5,6,2,8};
	h1.insert(arr, arr + 10);
	for_each(h1.begin(), h1.end(), TinyStl::display<typename TinyStl::hash_set<int>::iterator>());
	printf("\n");
	TinyStl::hash_set<int> h2;
	h2 = h1;
	for_each(h2.begin(), h2.end(), TinyStl::display<typename TinyStl::hash_set<int>::iterator>());
	printf("\n");
	h1.erase(h1.find(6));
	for_each(h1.begin(), h1.end(), TinyStl::display<typename TinyStl::hash_set<int>::iterator>());
	system("pause");
}