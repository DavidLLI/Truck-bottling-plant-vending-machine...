#include "VendingMachine.h"
#include "NameServer.h"

VendingMachine::VendingMachine( Printer &ptr, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour) : 
                                ptr(ptr), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
  ExType = 0;
  for (unsigned int i = 0; i < 4; i++) {
    stock[i] = 0;
  }
  nameServer.VMregister(this);
}

void VendingMachine::main() {
  ptr.print(Printer::Vending, id, 'S', sodaCost);
  for (;;) {
    
    try {
      _Accept(~VendingMachine) {
        break;
      }
      or _Accept(buy) {
        if (cur_card->getBalance() < sodaCost) {
          ExType = 1;
          lk.signalBlock();
        }
        else if ( stock[cur_flavour] < 1) {
          ExType = 2;
          lk.signalBlock();
        }
        else {
          ExType = 0;
          stock[cur_flavour]--;
          cur_card->withdraw(sodaCost);
          ptr.print(Printer::Vending, id, 'B', cur_flavour, stock[cur_flavour]);
          lk.signalBlock();
        }
      }
      or _Accept(inventory) {
        ptr.print(Printer::Vending, id, 'r');
        _Accept(~VendingMachine) {
          break;
        } 
        or _Accept(restocked) {
          ptr.print(Printer::Vending, id, 'R');
        }
      }
    } catch (uMutexFailure::RendezvousFailure) {}
    
  }
  ptr.print(Printer::Vending, id, 'F');
}

void VendingMachine::buy(Flavours flavour, WATCard &card) {
  cur_flavour = flavour;
  cur_card = &card;
  lk.wait();
  if (ExType == 1) {
    _Throw Funds();
  }
  else if (ExType == 2) {
    _Throw Stock();
  }
}

unsigned int * VendingMachine::inventory() {
  return stock;
}

void VendingMachine::restocked() {
  
}

unsigned int VendingMachine::cost() {
  return sodaCost;
}

unsigned int VendingMachine::getId() {
  return id;
}