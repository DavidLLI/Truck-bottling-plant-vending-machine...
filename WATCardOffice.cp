#include "WATCardOffice.h"
#include "Printer.h"
#include "WATCard.h"
#include "Bank.h"
#include "MPRNG.h"
#include <vector>
#include <map>

using namespace std;

extern MPRNG rng;

_Task WATCardOffice::Courier {
	WATCardOffice& watoff;
	Bank &bank;
	Printer &prt;
	map<unsigned int, WATCard *> &cardMap;
	unsigned int id;

	void main() {
		prt.print(Printer::Courier, id, 'S');
		for(;;) {
			_Accept(~Courier) {
				break;
			} _Else {
				Job *j = watoff.requestWork();
				Args args = j->args;

				WATCard::FWATCard fc = j->result;
				cardMap[args.sid] = new WATCard();

				prt.print(Printer::Courier, id, 't', args.sid, args.amount);

				bank.withdraw(args.sid, args.amount);

				prt.print(Printer::Courier, id, 'T', args.sid, args.amount);

				cardMap[args.sid]->deposit(args.amount);

				if(rng(1,6) == 1) {
					fc.exception(new Lost);
					delete cardMap[args.sid];
					cardMap.erase(args.sid);
				} else {
					fc.delivery(cardMap[args.sid]);				
				}
			}
		} // for
		prt.print(Printer::Courier, id, 'F');
	} // main

  public:
	Courier(WATCardOffice& watoff, Bank &bank, Printer &prt, map<unsigned int, WATCard *> &cardMap, unsigned int id) 
		: watoff(watoff), bank(bank), prt(prt), cardMap(cardMap), id(id) {}	
};


void WATCardOffice::main() {

	prt.print(Printer::WATCardOffice, 'S');
	Courier *courArray[numCouriers];
	for(unsigned int i=0; i < numCouriers; i++) {
		courArray[i] = new Courier(*this, bank, prt, cardMap, i);
	}

	for(;;) {

		_Accept(~WATCardOffice) {
			break;
		} or _Accept(create) {
		} or _Accept(transfer) {

		} or _When(jobVec.size() > 0) _Accept(requestWork) {
			prt.print(Printer::WATCardOffice, 'W');
		}
	} // for

	 map<unsigned int, WATCard *>::iterator it;
	 for(it = cardMap.begin(); it != cardMap.end(); it++) {
	 	delete it->second;
	 }

	for(unsigned int i=0; i < numCouriers; i++) {
		delete courArray[i];
	}

	prt.print(Printer::WATCardOffice, 'F');
}

WATCardOffice::WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers) 
	:prt(prt), bank(bank), numCouriers(numCouriers) {}


WATCard::FWATCard WATCardOffice::create(unsigned int sid, unsigned int amount) {
	Args arg;
	arg.sid = sid;
	arg.amount = amount;

	Job j(arg);

	jobVec.push_back(j);

	prt.print(Printer::WATCardOffice, 'C', sid, amount);

	return j.result;
}


WATCard::FWATCard WATCardOffice::transfer(unsigned int sid, unsigned int amount, WATCard *card) {
	Args arg;
	arg.sid = sid;
	arg.amount = amount;

	Job j(arg);
	delete cardMap[sid];
	cardMap[sid] = card;

	jobVec.push_back(j);

	prt.print(Printer::WATCardOffice, 'T', sid, amount);

	return j.result;
}


WATCardOffice::Job *WATCardOffice::requestWork() {
	Job *j= &jobVec.front();
	jobVec.erase(jobVec.begin()); // pop front
	return j;
}
