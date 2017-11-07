#include <queue>
#include "Envelope.h"
#include "Route.h"
#include "Socket.h"
#include "Message.h"
#include "Action.h"
#include <string.h>
#include <list>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <thread>

using namespace std;
#ifndef interfaceh   // if x.h hasn't been included yet...
#define interfaceh  //   #define this so the compiler knows it has been included

class Interface{
    public:
    Interface(list<Route>*, map<bool, queue<Message> >*, char*, char*, char*, char*, char*, int);
    void wakeUp(char*, char*, int, char*);
    void run();
    void receive();
    Envelope assemblePackage(char*);
    void processEnvelope();
    bool isBroadcast(char*);
    int getDistance(char*);
    char* checkIpTable(char*);
    Message checkSharedMemory();
	void processMessage();
    void send();
    void addToSharedMemory(Message);

    private:
	/*void sendInternally(Envelope);
	char* callDispatche(char*);*/
    list<Route>* ipTable;
    map<bool, queue<Message> >* messagePool;
    queue<Envelope> inbox;
    queue<Envelope> outbox;
    bool empty;
    char* network;
    char* ipAddress;
    char* macAddress;
    char* realIpAddress;
    char* dispatcherAddress;
	int dispatcherPort;
    map<char*, char*> cacheTable;
};

#endif
