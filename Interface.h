#include <queue>
#include "Envelope.h"
#include "Route.h"
#include "Socket.h"
#include <string.h>
#include <list>
#include <map>

using namespace std;
#ifndef interfaceh   // if x.h hasn't been included yet...
#define interfaceh  //   #define this so the compiler knows it has been included

class Interface{
    public:
    Interface(list<Route>*, map<bool, queue<Message> >*);
    Interface();
    void run();
    void receive();
    void assemblePackage(char*);
    bool isBroadcast(char*);
    Route checkIPTable(char*);
    Envelope packEnvelopeBroadcast(Route);
    Envelope packEnvelope(Message);
    void send(Envelope);

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
