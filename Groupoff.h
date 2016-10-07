#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include <vector>
#include "Printer.h"
#include "WATCard.h"

using namespace std;

_Task Groupoff {
	Printer &prt;
	unsigned int numStudents;
	unsigned int sodaCost;
	unsigned int groupoffDelay;

	vector<WATCard::FWATCard> fcardVec;
	vector<WATCard*> cardVec;

  public:
    Groupoff(Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay);
    WATCard::FWATCard giftCard();
  private:
  	void main();
};

#endif