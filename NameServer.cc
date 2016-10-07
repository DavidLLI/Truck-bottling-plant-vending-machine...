#include "NameServer.h"

NameServer::NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents) : prt(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
  cur_num_VM = 0;
  vendingMachines = new VendingMachine* [numVendingMachines];
  for (int i = 0 ; i < numVendingMachines; i++) {
    vendingMachines[i] = NULL;
  }  
  
  students = new VendingMachine* [numStudents];
  for (int i = 0; i < numStudents; i++) {
    students[i] = vendingMachines;
  }
}

void NameServer::VMregister (VendingMachine* vendingmachine) {
  vendingMachines[cur_num_VM] = vendingmachine;
}

VendingMachine* NameServer::getMachine (unsigned int id) {
  VendingMachine* ret = *(students[id]);
  cur_id = id;
  lk.wait();
  return ret;
}

VendingMachine** NameServer::getMachineList() {
  return vendingMachines;
}

void NameServer::main() {
  ptr.print(Printer::NameServer, 'S');
  _Accept (~NameServer) {
    break;
  }
  or _Accept (VMregister) {
    ptr.print(Printer::NameServer, 'R', vendingMachines[cur_num_VM]->getId());
    cur_num_VM++;
  }
  or _Accept (getMachine) {
    ptr.print(Printer::NameServer, 'N', cur_id, (*(students[cur_id])) -> getId());
    if (students[cur_id] - vendingMachines == numVendingMachines - 1) {
      students[cur_id] = vendingMachines;
    }
    else {
      students[cur_id]++;
    }
    lk.signalBlock();
  }
  or _Accept (getMachineList) {

  }
  ptr.print(Printer::NameServer, 'F');
}