#include "Parent.h"
#include "Printer.h"
#include "WATCard.h"
#include "MPRNG.h"

extern MPRNG rng;


Parent::Parent(Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay) 
	: prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {}


void Parent::main() {
	prt.print(Printer::Parent, 'S');

	for(;;) {
		_Accept(~Parent) {
			break;
		} _Else {
			unsigned int amt = rng(1,3);
			unsigned int sid = rng(0,numStudents-1);

			yield(parentalDelay);

			prt.print(Printer::Parent, 'D', sid, amt);
			bank.deposit(sid, amt);
		}
	} // for

	prt.print(Printer::Parent, 'F');
}