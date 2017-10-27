#include "Message.h"
#ifndef envelopeh   // if x.h hasn't been included yet...
#define envelopeh  //   #define this so the compiler knows it has been included

class Envelope{
    public:
    Envelope(char*, char*, Message, char);
    //Envelope();
    char* getMacSender();
    char* getMacReceiver();
    Message getMessage();
    char getSequenceNumber();

    private:
    char* macSender = new char[10];
    char* macReceiver = new char[10];
    Message message;
    char sequenceNumber;
};

#endif
