#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <vector>

using namespace std;

struct objectState {
	bool valid;
	char state;
	unsigned int lid;
	int value1;
	int value2;
};

_Monitor Printer {
	unsigned int numStudents;
	unsigned int numVendingMachines;
	unsigned int numCouriers;
	unsigned int total;

	vector<objectState> osv;

	void flush();

  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
	void finish(Kind kind);
	void finish(Kind kind, unsigned int lid);
	objectState& getRef(Kind kind, unsigned int lid);    
};

#endif