#include "Truck.h"
#include "BottlingPlant.h"
#include "MPRNG.h"

extern MPRNG rng;

Truck::Truck(Printer &prt, NameServer &nameServer, BottlingPlant &plant,
             unsigned int numVendingMachines, unsigned int maxStockPerFlavour) :
             prt(prt), nameServer(nameServer), plant(plant), 
             numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {
  for (unsigned int i = 0; i < 4; i++) {
    cargo[i] = 0;
  }
}

void Truck::main() {
  prt.print(Printer::Truck, 'S');
  VendingMachine **VendingMachines = nameServer.getMachineList();
  unsigned int cur_num_VM = 0;
  try {
    for (;;) {
      yield(rng(1,10));
      plant.getShipment(cargo);
      int total = 0;
      for (unsigned int i = 0; i < 4; i++) {
        total += cargo[i];
      }
      prt.print(Printer::Truck, 'P', total);
      for (unsigned int i = 0; i < numVendingMachines; i++) {
        prt.print(Printer::Truck, 'd', i, total);
        unsigned int *cur_stock = VendingMachines[cur_num_VM]->inventory();
        bool successful = true;
        total = 0;
        for (unsigned int j = 0; j < 4; j++) {
          if ((maxStockPerFlavour - cur_stock[j]) > cargo[j]) {
            cur_stock[j] += cargo[j];
            cargo[j] = 0;
            successful = false;
            total += (maxStockPerFlavour - cur_stock[j]);
          }
          else {
            cargo[j] -= (maxStockPerFlavour - cur_stock[j]);
            cur_stock[j] = maxStockPerFlavour;
          }
        }
        if (!successful) {
          prt.print(Printer::Truck, 'U', i, total);
        }
        VendingMachines[cur_num_VM]->restocked();
        total = 0;
        for (unsigned int i = 0; i < 4; i++) {
          total += cargo[i];
        } 
        prt.print(Printer::Truck, 'D', i, total);
        cur_num_VM++;
        if (cur_num_VM == numVendingMachines) {
          cur_num_VM = 0;
        }
        bool none_left = true;
        for (unsigned int j = 0; j < 4; j++) {
          if (cargo[j] != 0) {
            none_left = false;
          }
        }
        if (none_left) {
          break;
        }
      }
    }
  }
  catch (BottlingPlant::Shutdown) {}
  prt.print(Printer::Truck, 'F');
}