#include "NameServer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) 
  : prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
  cur_num_VM = 0;
  vendingMachines = new VendingMachine* [numVendingMachines];
  for (unsigned int i = 0 ; i < numVendingMachines; i++) {
    vendingMachines[i] = NULL;
  }  
  
  students = new unsigned int[numStudents];
  for (unsigned int i = 0; i < numStudents; i++) {
    students[i] = i % numVendingMachines;
  }
}

void NameServer::VMregister (VendingMachine* vendingmachine) {
  vendingMachines[cur_num_VM] = vendingmachine;
}

VendingMachine* NameServer::getMachine (unsigned int id) {
  VendingMachine* ret = vendingMachines[students[id]];
  cur_id = id;
  lk.wait();
  return ret;
}

VendingMachine** NameServer::getMachineList() {
  return vendingMachines;
}

void NameServer::main() {

  prt.print(Printer::NameServer, 'S');
  for(;;) {
    
    _Accept (~NameServer) {
      break;
    }
    or _Accept (VMregister) {
      prt.print(Printer::NameServer, 'R', vendingMachines[cur_num_VM]->getId());
      cur_num_VM++;
    }
    or _Accept (getMachine) {
      prt.print(Printer::NameServer, 'N', cur_id, vendingMachines[students[cur_id]] -> getId());
      if (students[cur_id] == numVendingMachines - 1) {
        students[cur_id] = 0;
      }
      else {
        students[cur_id]++;
      }
      lk.signalBlock();
    }
    or _Accept (getMachineList) {}    
  }

  prt.print(Printer::NameServer, 'F');
}