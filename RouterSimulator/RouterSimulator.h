#include "Message.h"
#include "Route.h"
#ifndef routerSimulatorh   // if x.h hasn't been included yet...
#define routerSimulatorh  //   #define this so the compiler knows it has been included

class RouterSimulator{
    public:
	RouterSimulator();
	list<Route> getIPTable();
	map<boolean, queue<Message>> getMessagePool();
 	void runInterfaceA();
	void runInterfaceB();
        
    private:
	list<Route> ipTable; 
	map<boolean, queue<Message>> messagePool;
        
};

#endif
