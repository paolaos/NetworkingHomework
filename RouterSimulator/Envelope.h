#include "Message.h"
#ifndef envelopeh   // if x.h hasn't been included yet...
#define envelopeh  //   #define this so the compiler knows it has been included

class Envelope{
    public:
    Envelope(char*, char*, Message, int);
    //Envelope();
    char* getMacSender();
    char* getMacReceiver();
    Message getMessage();
    int getSequenceNumber();

    private:
    char* macSender;
    char* macReceiver;
    Message message;
    int sequenceNumber;
};

#endif
