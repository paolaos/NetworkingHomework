#include <queue>
#include "Envelope.h"
#include "Route.h"
#include "Socket.h"
#include "Message.h"
#include <string.h>
#include <list>
#include <map>

using namespace std;
#ifndef interfaceh   // if x.h hasn't been included yet...
#define interfaceh  //   #define this so the compiler knows it has been included

class Interface{
    public:
    Interface(list<Route>*, map<bool, queue<Message> >*, char*, char*, char*, char*);
    Interface();
    void run();
    void receive();
    Envelope assemblePackage(char*);
    bool isBroadcast(char*);
    Route checkIPTable(char*);
    Envelope packEnvelopeBroadcast(Route);
    void sendInternally(Envelope, boolean);
	void sendToSharedMemory(Message);

    private:
	Message unwrap(Envelope);
	Envelope wrap(Message);
	Message checkSharedMemory();
	void processMessage(Message, bool);
    list<Route>* ipTable;
    map<bool, queue<Message> >* messagePool;
    queue<Envelope> inbox;
    queue<Envelope> outbox;
    char* ipAddress;
    char* macAddress;
    char* realIpAddress;
    char* dispatcherAddress;
    map<char*, char*> macTable;
};

#endif
