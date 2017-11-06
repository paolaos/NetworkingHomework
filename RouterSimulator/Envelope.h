#include "Message.h"
#ifndef envelopeh   // if x.h hasn't been included yet...
#define envelopeh  //   #define this so the compiler knows it has been included

class Envelope{
    public:
    Envelope(char*, char*, Message);
    Envelope(char*, char*, char*);
    Envelope(char*, char*, int, char*);
    //Envelope();
    char* getMacSender();
    char* getMacReceiver();
    Message getMessage();
    char* getRequestedIp();
    int getDistance();

    private:
    char* macSender;
    char* macReceiver;
    Message message;
    char* requestedIp;
    int distance;
};

#endif
