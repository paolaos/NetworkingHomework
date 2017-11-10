#include "Message.h"
#ifndef envelopeh   // if x.h hasn't been included yet...
#define envelopeh  //   #define this so the compiler knows it has been included

class Envelope{
    public:
    Envelope(char*, char[], Message); //normal in
    Envelope(int, char*, char*, char*, int, Message); //normal out, type 1
    Envelope(char*, char*, char*); //broadcast in
    Envelope(int, char*, char*, int, char*, int, char*); //broadcast out,type 2
    Envelope(char*, char*, int); //dispatcher in 
    Envelope(int, char*, char*); //dispatcher out, type 3

    char* getMacSender();
    char* getMacReceiver();
    char* getRequestedMac();
    Message getMessage();
    int getDistance();
    int getType();
    int getRealPort();
    char* getRealIpAddress();
    char* getRequestedIp();
    bool getIsDispatcher();

    private:
	char typeMessage;
    char* macSender;
    char* macReceiver;
	char payload[1513];	    


	int type;
    int realPort;
    char* realIpAddress;
    char* requestedMac;
    Message message;
    char* requestedIp;
    int requestedPort;
    int distance;
    bool isDispatcher;
};

#endif
