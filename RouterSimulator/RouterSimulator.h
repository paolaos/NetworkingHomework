#include "Message.h"
#include "Route.h"
#include "InterfaceKevin.h"
#include "InterfacePaola.h"
#include <iostream>
#include <fstream>
#ifndef routerSimulatorh   // if x.h hasn't been included yet...
#define routerSimulatorh  //   #define this so the compiler knows it has been included

using namespace std;
class RouterSimulator{
    public:
	RouterSimulator();
    void readIpTable();
 	void runInterfaceK();
	void runInterfaceP();
        
    private:
	list<Route> ipTable; 
	map<bool, queue<Message>> messagePool;
        
};

#endif
