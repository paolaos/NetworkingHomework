#include "Message.h"
#ifndef envelopeh   // if x.h hasn't been included yet...
#define envelopeh  //   #define this so the compiler knows it has been included

class Envelope{
    public:
    Envelope(int, char*, char*, Message);
    Envelope(char*, char*, char*);
    Envelope(int, char*, char*);
    Envelope(int, char*, char*, int, char*);
    //Envelope();
    char* getMacSender();
    char* getMacReceiver();
    char* getMacRequested();
    char* getIpRelated();
    Message getMessage();
    char* getRequestedIp();
    int getDistance();
    int getType();

    private:
    int type;
    char* macSender;
    char* macReceiver;
    char* macRequested;
    char* ipRelated;
    Message message;
    char* requestedIp;
    int distance;
};

#endif
