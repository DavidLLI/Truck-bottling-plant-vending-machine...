#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "Printer.h"
#include "WATCard.h"

_Task NameServer;

_Task VendingMachine {
    uCondition lk;

    Printer &ptr;
    NameServer &nameServer;
    unsigned int id;
    unsigned int sodaCost;
    unsigned int maxStockPerFlavour;

    int ExType;
    unsigned int stock[4];
    WATCard *cur_card;
    void main();
  public:
    enum Flavours { Apple = 0, Orange = 1, Grape = 2, Lemon = 3 };                 // flavours of soda (YOU DEFINE)
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
     Flavours cur_flavour;  
};

#endif