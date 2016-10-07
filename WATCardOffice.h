#ifndef __WARCARDOFFICE_H__
#define __WARCARDOFFICE_H__

#include <vector>
#include <map>

#include "WATCard.h"

using namespace std;


_Monitor Printer;
_Monitor Bank;

struct Args {
    unsigned int sid;
    unsigned int amount;
};

_Task WATCardOffice {
    struct Job {                            // marshalled arguments and return future
        Args args;                          // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;           // return future
        Job(Args args) : args(args) {}
    };
    _Task Courier;                          // communicates with bank

    Printer &prt;
    Bank &bank;
    unsigned int numCouriers;

    vector<Job> jobVec;
    map<unsigned int, WATCard *> cardMap;

    void main();
  public:
    _Event Lost {};                         // lost WATCard
    WATCardOffice(Printer &prt, Bank &bank, unsigned int numCouriers);
    WATCard::FWATCard create(unsigned int sid, unsigned int amount);
    WATCard::FWATCard transfer(unsigned int sid, unsigned int amount, WATCard *card);
    Job *requestWork();
};

#endif