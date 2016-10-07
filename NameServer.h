#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "VendingMachine.h"
#include "Printer.h"

_Task NameServer {
    uCondition lk;
    Printer &prt;
    unsigned int numVendingMachines;
    unsigned int cur_num_VM;
    unsigned int numStudents;
    VendingMachine** vendingMachines;
    unsigned int *students;

    unsigned int cur_id;
    void main();
  public:
    NameServer(Printer &prt, unsigned int numVendingMachines, unsigned int numStudents);
    void VMregister(VendingMachine *vendingmachine);
    VendingMachine *getMachine(unsigned int id);
    VendingMachine **getMachineList();
};

#endif