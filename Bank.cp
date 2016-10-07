#include "Bank.h"
#include <map>

using namespace std;

Bank::Bank(unsigned int numStudents) : numStudents(numStudents) {
	for(unsigned int i = 0; i < numStudents; i++) {
		balance[i] = 0;
	}
}


void Bank::deposit(unsigned int id, unsigned int amount) {
	balance[id] += amount;
}


void Bank::withdraw(unsigned int id, unsigned int amount) {
	if(balance[id] >= amount) _Accept(deposit);
	balance[id] -= amount;
}