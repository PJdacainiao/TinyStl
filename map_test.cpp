#include <iostream>
#include "Algorithm.h"
#include "Function.h"
#include "map.h"
void map_test() {
	TinyStl::map<int, const char*> map1;
	map1[1] = "panjin";
	map1[2] = "pj";
	TinyStl::pair<int, const char*> p1(2, "pjdaboss");
	TinyStl::pair<int, const char*> p2(3, "pjdacainiao");
	map1.insert(p1);
	map1.insert(p2);
	typename TinyStl::map<int, const char*>::iterator it = map1.begin();
	map1.erase(it);
	for (typename TinyStl::map<int, const char*>::iterator iter = map1.begin(); iter != map1.end(); ++iter) {
		for (const char* ch = (*iter).second; *ch != '\0'; ++ch)
			std::cout << *ch;
		std::cout << std::endl;
	}
	system("pause");

}
