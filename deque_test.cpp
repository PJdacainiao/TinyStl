#include <iostream>
#include "vector.h"
#include "Deque.h"
#include "Algorithm.h"
#include "Function.h"

void deque_test() {
	TinyStl::deque<int> dq1(1, 1);
	dq1.push_front(2);
	dq1.push_front(2);
	dq1.push_back(3);
	dq1.push_back(3);
	dq1.pop_front();
	dq1.pop_back();
	dq1.erase(dq1.begin() + 1);
	TinyStl::deque<int> dq2(dq1.begin(), dq1.end());
	dq1.insert(dq1.begin() + 1, 3, 10);
	dq1.insert(dq1.begin() + 1, dq2.begin(), dq2.end());
	TinyStl::deque<int> dq3(dq1);
	TinyStl::for_each(dq1.begin(), dq1.end(), TinyStl::display<typename TinyStl::deque<int>::iterator>());
	std::cout << std::endl;
	TinyStl::deque<int> dq4 = dq1;
	TinyStl::for_each(dq4.begin(), dq4.end(), TinyStl::display<typename TinyStl::deque<int>::iterator>());
	std::cout << std::endl;
	std::cout << dq1.size() << std::endl;
	std::cout << dq1.capacity() << std::endl;
	dq1.clear();
	std::cout << dq1.size() << std::endl;
	system("pause");
}
