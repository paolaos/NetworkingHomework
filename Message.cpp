#include "Message.h"

Message::Message(char* ipSender, char* ipReceiver, char* message, Action action){
    this->ipSender = ipSender;
    this->ipReceiver = ipReceiver;
    this->message = message;
    this->action = action;
}

Message::Message(){}

char* Message::getIPSender(){
    return this->ipSender;
}

char* Message::getIPReceiver(){
    return this->ipReceiver;
}

char* Message::getMessage(){
    return this->message;
}

Action Message::getRequestedAction(){
    return this->action;
}
