#include <queue>
#include "Envelope.h"
#include "Route.h"
#include "Socket.h"
#include <string.h>
#include <list>
#include <map>
#include <stdlib.h>     /* exit, EXIT_FAILURE */

using namespace std;
#ifndef interfaceh   // if x.h hasn't been included yet...
#define interfaceh  //   #define this so the compiler knows it has been included
#define BROADCAST "192.168.0.0"

class Interface{
    public:
    Interface(list<Route>*, map<bool, queue<Message> >*);
    Interface();
    void run();
    void receive();
    Envelope assemblePackage(char*);
    bool isBroadcast(char*);
    int checkIPTable(Message);
    Envelope packEnvelope(Message);
    void send(Envelope);
    list<Route>* getIPTable();
    map<bool, queue<Message> >* getMessagePool();
    queue<Envelope> getInbox();
    queue<Envelope> getOutbox();
    char* getIPAddress();
    char* getMacAddress();
    map<char*, char*> getMacTable();
	
    private:
    list<Route>* ipTable;
    map<bool, queue<Message> >* messagePool;
    queue<Envelope> inbox;
    queue<Envelope> outbox;
    char* ipAddress = new char[4];
    char* macAddress = new char[10];
    map<char*, char*> macTable;
};

#endif
