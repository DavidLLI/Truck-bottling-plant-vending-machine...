#include "Groupoff.h"
#include "WATCard.h"

#include <vector>
#include <iostream>

using namespace std;


Groupoff::Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay) 
	: prt(prt), numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay) {}


WATCard::FWATCard Groupoff::giftCard() {
	WATCard::FWATCard c;
	fcardVec.push_back(c);
	return c;
}


void Groupoff::main() {

	prt.print(Printer::Groupoff, 'S');

	for(unsigned int i=0; i < numStudents; i++) {
		_Accept(~Groupoff) {
			break;
		} or _Accept(giftCard) {
			cout << "^^^^^^^giftcard " << i << endl;

			WATCard *c = new WATCard();
			cardVec.push_back(c);

			yield(groupoffDelay);
			c->deposit(sodaCost);

			

			prt.print(Printer::Groupoff, 'D', sodaCost);
			fcardVec[i].delivery(c);
		}
	} // for

	for(unsigned int i=0; i < cardVec.size(); i++) {
		delete cardVec[i];
	}

	prt.print(Printer::Groupoff, 'F');
}

