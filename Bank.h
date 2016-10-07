#ifndef __BANK_H__
#define __BANK_H__

#include <map>

using namespace std;

_Monitor Bank {
	unsigned int numStudents;
	map<unsigned int, unsigned int> balance;

  public:
    Bank(unsigned int numStudents);
    void deposit(unsigned int id, unsigned int amount);
    void withdraw(unsigned int id, unsigned int amount);
};

#endif