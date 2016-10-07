#include "Truck.h"

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
  VendingMachine **cur_VM = VendingMachines;
  unsigned int cur_num_VM = 0;
  try {
    for (;;) {
      yield(rng(1,10));
      plant.getShipment(cargo);
      for (unsigned int i = 0; i < numVendingMachines; i++) {
        unsigned int *cur_stock = VendingMachines[cur_num_VM]->inventory();
        cur_num_VM++;
        if (cur_num_VM == numVendingMachines) {
          cur_num_VM = 0;
        }
        for (unsigned int j = 0; j < 4; j++) {
          if ((maxStockPerFlavour - cur_stock[j]) >= cargo[j]) {
            cur_stock[j] += cargo[j];
            cargo[j] = 0;
          }
          else {
            cargo[j] -= (maxStockPerFlavour - cur_stock[j]);
            cur_stock[j] = maxStockPerFlavour;
          }
        }
        boolean none_left = true;
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
  catch (BottlingPlant::ShutDown s) {}
}