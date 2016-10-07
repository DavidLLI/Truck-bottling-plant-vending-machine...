#include "Student.h"
#include "Printer.h"
#include "NameServer.h"
#include "WATCardOffice.h"
#include "Groupoff.h"
#include "WATCard.h"
#include "VendingMachine.h"
#include "MPRNG.h"
#include "uFuture.h"
#include "iostream"

using namespace std;

extern MPRNG rng;

Student::Student(Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, Groupoff &groupoff,
				unsigned int id, unsigned int maxPurchases)
				: prt(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id), 
				maxPurchases(maxPurchases) {}

void Student::main() {
	unsigned int purchases = rng(1, maxPurchases);

	VendingMachine::Flavours flavour;
	switch(rng(0,3)) {
		case 0:
			flavour = VendingMachine::Apple;
			break;
		case 1:
			flavour = VendingMachine::Orange;
			break;
		case 2:
			flavour = VendingMachine::Grape;
			break;
		case 3:
			flavour = VendingMachine::Lemon;
			break;
	}

	prt.print(Printer::Student, id, 'S', flavour, purchases);

	unsigned int sodaCost;

	WATCard::FWATCard fCard = cardOffice.create(id, 5);
	
	WATCard::FWATCard fGift = groupoff.giftCard();

	VendingMachine *vending = nameServer.getMachine(id);
	prt.print(Printer::Student, id, 'V', vending->getId());

	sodaCost = vending->cost();

	for(;;) {
		if(purchases == 0) break; // exit outter for
		yield(rng(1,10));

		// make to purchase 
		try {
			cout << "$$$$$$$$$select start" << id << endl;
			_Select(fGift) {
				vending->buy(flavour, *(fGift()));
				prt.print(Printer::Student, id, 'G', fGift()->getBalance());

				fGift.reset();
				cout << "reset" << id << endl;	
			} or _Select(fCard) {
				for(;;) {
					try {
						vending->buy(flavour, *(fCard()));
						prt.print(Printer::Student, id, 'B', fCard()->getBalance());
												
					} catch(WATCardOffice::Lost) {
						cout << "!!!!!!!!!!!!!!!!!lost exception " << id << endl;
						prt.print(Printer::Student, id, 'L');
						fCard = cardOffice.create(id, 5);
					}
					break;
				}			
			}
			cout << "$$$$$$$$$select end" << id << endl;

			purchases--;
		} catch(VendingMachine::Funds) {
			cout << "!!!!!!!!!!!!!!!!!fund exception " << id << endl;
			fCard = cardOffice.transfer(id, 5 + sodaCost, fCard);
		} catch(VendingMachine::Stock) {
			cout << "!!!!!!!!!!!!!!!!!stock exception " << id << endl;
			vending = nameServer.getMachine(id);
			prt.print(Printer::Student, id, 'V', vending->getId());
		} // outter try
	} // outter for

	prt.print(Printer::Student, id, 'F');
}