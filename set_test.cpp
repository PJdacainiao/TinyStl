#include <iostream>
#include "Algorithm.h"
#include "Function.h"
#include "set.h"
void set_test() {
	TinyStl::set<int> set1;
	set1.insert(1);
	set1.insert(2);
	set1.insert(3);
	set1.insert(4);
	TinyStl::set<int> set2(set1);
	TinyStl::for_each(set2.begin(), set2.end(), TinyStl::display<typename TinyStl::set<int>::iterator>());
	std::cout << std::endl;
	std::cout << *(set2.find(2)) << std::endl;
	system("pause");
}