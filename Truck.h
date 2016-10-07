#ifndef __TRUCK_H__
#define __TRUCK_H__

#include "Printer.h"
#include "NameServer.h"
#include "VendingMachine.h"

_Task BottlingPlant;

_Task Truck {
    Printer &prt;
    NameServer &nameServer;
    BottlingPlant &plant;
    unsigned int numVendingMachines;
    unsigned int maxStockPerFlavour;
    unsigned int cargo[4];
    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif