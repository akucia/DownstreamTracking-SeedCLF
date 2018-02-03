#ifndef COUNTERS_H
#define COUNTERS_H

#include <iostream>
#include <cmath>

class Counters{
	
	public:
		inline void print();
	
	long long totalNumOfEvents = 0;
};

void Counters::print(){
	
	std::cout << "counters:" << "\n\n";
	std::cout << "totalNumOfEvents:    " << totalNumOfEvents << "\n";
	
}
#endif