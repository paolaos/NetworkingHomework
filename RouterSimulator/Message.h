#include "Action.h"
#ifndef messageh   // if x.h hasn't been included yet...
#define messageh  //   #define this so the compiler knows it has been included

class Message{
    public:
        Message(char*, char*, char*, Action);
	Message();
        char* getIpSender();
        char* getIpReceiver();
        char* getMessage();
        Action getRequestedAction();

    private:
        char* ipSender;
        char* ipReceiver;
        char* message;
        Action action;
};

#endif
