#include <iostream>
#include "vector.h"
#include "Algorithm.h"
#include "Function.h"
#include "priority_queue.h"

void priority_queue_test() {
	TinyStl::priority_queue<int> pq1;
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	pq1.push(1);
	pq1.push(2);
	pq1.pop();
	//printf("%d\n", pq1.top());
	TinyStl::priority_queue<int> pq2(arr,arr+10);
	pq1 = pq2;	
	int n = pq1.size();
	for (int i = 0; i < n; ++i) {
		printf("%d\n", pq1.top());
		pq1.pop();
	}
	system("pause");
}