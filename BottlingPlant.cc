#include "BottlingPlant.h"

extern MPRNG rng;

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) : prt(prt), nameServer(nameServer), numVendingMachines(numVendingMachines),
                 maxShippedPerFlavour(maxShippedPerFlavour), maxStockPerFlavour(maxStockPerFlavour), 
                 timeBetweenShipments(timeBetweenShipments) {
  isShutDown = false;
  for (unsigned int i = 0; i < 4; i++) {
    production[i] = 0;
  }
}

void BottlingPlant::getShipment(unsigned int cargo[]) {
  if (isShutDown == true) {
    _Throw Shutdown();
  }
  for (unsigned int i = 0; i < 4; i++) {
    cargo[i] = production[i];
  }
}

void BottlingPlant::main() {
  prt.print(Printer::BottlingPlant, 'S');
  Truck t(prt, nameServer, (*this), numVendingMachines, maxStockPerFlavour);
  for (;;) {
    unsigned int total = 0;
    unsigned int each = 0;
    for (unsigned int i = 0; i < 4; i++) {
      each = rng(0, maxShippedPerFlavour);
      production[i] = each;
      total += each;
    }
    prt.print(Printer::BottlingPlant, 'G', total);
    yield(timeBetweenShipments);

    _Accept (~BottlingPlant) {
      isShutDown = true;
      break;
    }
    or _Accept (getShipment) {
      prt.print(Printer::BottlingPlant, 'P');  
    }
  }
  prt.print(Printer::BottlingPlant, 'F');
}