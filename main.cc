#include <iostream>
#include "MPRNG.h"
#include "Config.h"
#include "Printer.h"
#include "Bank.h"
#include "WATCardOffice.h"
#include "Parent.h"
#include "Groupoff.h"
#include "BottlingPlant.h"
#include "NameServer.h"
#include "VendingMachine.h"
#include "Student.h"

using namespace std;

struct E {};

MPRNG rng;

void uMain::main() {
    
    //get arguments and parameters
    int seed = getpid();
    char* filename;
    struct ConfigParms parms;
    
    try {
        switch(argc) {
            case 3:
                seed = atoi(argv[2]);
                if(seed <= 0) {
                    throw E();
                }
            case 2:
                filename = argv[1];
            case 1:
                filename = "soda.config";
                break;
            default:
                throw E();
                
        } //switch  
    } catch(...) {
        cerr << "Usage: ./soda  config-file  Seed (> 0)" << endl;
        return;
    } //try
    
    processConfigFile(filename, parms);

    rng.seed(seed);


    //create tasks
    Printer *prt = new Printer(parms.numStudents, parms.numVendingMachines, parms.numCouriers);

    Bank *bank = new Bank(parms.numStudents);

    Parent *parent = new Parent(*prt, *bank, parms.numStudents, parms.parentalDelay);

    WATCardOffice *watoff = new WATCardOffice(*prt, *bank, parms.numCouriers);

    Groupoff *groupoff = new Groupoff(*prt, parms.numStudents, parms.sodaCost, parms.groupoffDelay);

    NameServer *nameserver = new NameServer(*prt, parms.numVendingMachines, parms.numStudents);

    VendingMachine *vending[parms.numVendingMachines];
    for(unsigned int i=0; i < parms.numVendingMachines; i++) {
        vending[i] = new VendingMachine(*prt, *nameserver, i, parms.sodaCost, 
            parms.maxStockPerFlavour);
    }

    BottlingPlant *bottling = new BottlingPlant(*prt, *nameserver, parms.numVendingMachines, 
        parms.maxShippedPerFlavour, parms.maxStockPerFlavour, parms.timeBetweenShipments);

    Student *stud[parms.numStudents];
    for(unsigned int i=0; i < parms.numStudents; i++) {
        stud[i] = new Student(*prt, *nameserver, *watoff, *groupoff, i, parms.maxPurchases);
    }

    //delete tasks
    for(unsigned int i=0; i < parms.numStudents; i++) {
        delete stud[i];
    }

    cout << "@@@@@@@@@@@@delete stud" << endl;

    delete bottling;
    cout << "@@@@@@@@@@@@delete bottling" << endl;

    for(unsigned int i=0; i < parms.numVendingMachines; i++) {
        delete vending[i];
    }
    cout << "@@@@@@@@@@@@delete vending" << endl;

    delete nameserver;
    cout << "@@@@@@@@@@@@delete nameserver" << endl;

    delete groupoff;
    cout << "@@@@@@@@@@@@delete groupoff" << endl;

    delete watoff;
    cout << "@@@@@@@@@@@@delete watoff" << endl;

    delete parent;
    cout << "@@@@@@@@@@@@delete parent" << endl;

    delete bank;
    cout << "@@@@@@@@@@@@delete bank" << endl;

    delete prt;
    cout << "@@@@@@@@@@@@delete prt" << endl;
}