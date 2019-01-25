#include <iostream>
#include "Algorithm.h"
#include "Function.h"
#include "RBtree.h"
void rbTree_test() {
	TinyStl::rb_tree<int, int> rb1;
	rb1.insert_unique(1);
	rb1.insert_unique(2);
	rb1.insert_unique(3);
	rb1.insert_unique(4);
	rb1.insert_unique(5);
	rb1.insert_unique(6);
	rb1.insert_unique(6);
	rb1.insert_unique(5);
	rb1.insert_unique(4);
	rb1.insert_unique(3);
	rb1.insert_unique(2);
	rb1.insert_unique(1);
	rb1.erase(rb1.begin());
	std::cout << *(rb1.find(2)) << std::endl;
	TinyStl::for_each(rb1.begin(), rb1.end(), TinyStl::display<typename TinyStl::rb_tree<int, int>::iterator>());
	std::cout <<  std::endl;
	TinyStl::rb_tree<int, int> rb2(rb1);
	rb2.insert_unique(rb1.begin(), rb1.end());
	rb2.clear();
	TinyStl::for_each(rb2.begin(), rb2.end(), TinyStl::display<typename TinyStl::rb_tree<int, int>::iterator>());
	system("pause");
}