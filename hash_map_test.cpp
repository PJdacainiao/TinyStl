#include <iostream>
#include "hash_map.h"
#include "Function.h"
#include "Algorithm.h"
#include "utility.h"

void hash_map_test() {
	TinyStl::hash_map<int, const char*> h1;
	using p = TinyStl::pair<int, const char*>;
	h1.insert(p(1,"panjin"));
	p arr[5] = { p(1,"panjin"),p(2,"pan"),p(3,"jin"),p(4,"czp"),p(5,"pzc") };
	printf("elements of h1\n");
	h1.insert(arr, arr+5);
	for (auto it = h1.begin(); it != h1.end(); ++it)
		printf("%s\n", (*it).second);
	TinyStl::hash_map<int,const char*> h2;
	h2 = h1;
	printf("elements of h2\n");
	for (auto it = h2.begin(); it != h2.end(); ++it)
		printf("%s\n", (*it).second);
	h1.erase(h1.find(*arr));
	printf("elements of h1\n");
	for (auto it = h1.begin(); it != h1.end(); ++it)
		printf("%s\n", (*it).second);
	system("pause");
}