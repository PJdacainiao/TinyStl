#include <iostream>
#include "list.h"
#include "Algorithm.h"
#include "Function.h"

void list_test() {
	TinyStl::list<int> list1(10, 5);
	TinyStl::list<int> list2(list1);
	TinyStl::list<int> list3 = list1;
	list1.push_front(1);
	list1.push_back(2);
	list1.push_back(2);
    list1.push_back(2);
	list1.push_back(3);
	list1.push_back(4);
	list1.pop_back();
	list1.pop_front();
	list1.remove(3);
	list1.unique();
	list1.insert(++(list1.begin()), 2, 3);
	list1.splice(list1.begin(),list2.begin(),list2.end());
	list1.sort();
	TinyStl::for_each(list1.begin(), list1.end(), TinyStl::display<typename TinyStl::list<int>::iterator>());
	system("pause");
}
