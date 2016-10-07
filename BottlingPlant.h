#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

#include "Truck.h"
#include "Printer.h"
#include "NameServer.h"

_Task BottlingPlant {
    Printer &prt;
    NameServer &nameServer;
    unsigned int numVendingMachines;
    unsigned int maxShippedPerFlavour;
    unsigned int maxStockPerFlavour;
    unsigned int timeBetweenShipments;
    unsigned int production[4];
    bool isShutDown;
    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif