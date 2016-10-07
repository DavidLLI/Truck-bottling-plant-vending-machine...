#include "VendingMachine.h"

VendingMachine::VendingMachine( Printer &ptr, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour) : 
                                ptr(ptr), nameServer(nameServer), id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
  ExType = 0;
  for (unsigned int i = 0; i < num_flavour; i++) {
    stock[i] = 0;
  }
  nameServer.VMregister(this);
}

void VendingMachine::main() {
  ptr.print(Printer::Vending, id, 'S', sodaCost);
  for (;;) {
    
    Accept(~VendingMachine) {
      break;
    }
    try {
      or _Accept(buy) {
        if (card.getBalance() < sodaCost) {
          ExType = 1;
          lk.signalBlock();
        }
        else if ( stock[flavour] < 1) {
          ExType = 2;
          lk.signalBlock();
        }
        else {
          ExType = 0;
          stock[cur_flavour]--;
          card.withdraw(sodaCost);
          ptr.print(Printer::Vending, id, 'B', cur_flavour, stock[cur_flavour]);
          lk.signalBlock();
        }
      }
    }
    catch (uMutexFailure::RendezvousFailure) {}
    or _Accept(inventory) {
      ptr.print(Printer::Vending, id, 'r');
      _Accept(~VendingMachine) {
        break;
      } 
      or _Accept(restocked) {
        ptr.print(Printer::Vending, id, 'R');
      }
    }
    
  }
  ptr.print(Printer::Vending, id, 'F');
}

void VendingMachine::buy(Flavours flavour, WATCard &card) {
  cur_flavour = flavour;
  cur_card = card;
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