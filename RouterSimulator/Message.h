#include "Action.h"
#ifndef messageh   // if x.h hasn't been included yet...
#define messageh  //   #define this so the compiler knows it has been included

class Message{
    public:
        Message(char*, char*, char*, Action);
	Message();
        char* getIPSender();
        char* getIPReceiver();
        char* getMessage();
        Action getRequestedAction();

    private:
	char* ipSender = new char[4];
        char* ipReceiver= new char[4];
        char* message;
        Action action;
};

#endif
