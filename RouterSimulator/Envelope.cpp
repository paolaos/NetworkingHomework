#include "Envelope.h"

//normal in and out, type 1
Envelope::Envelope(int type, char* macSender, char* macReceiver, Message message){
    this->type = type;
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->message = message;
}

//broadcast in
Envelope::Envelope(char* macSender, char* macReceiver, char* requestedIp){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->requestedIp = requestedIp;
}

//dispatcher in and out, type 3
Envelope::Envelope(int type, char* macRequested, char* ipRelated){
    this->type = type;
    this->macRequested = macRequested;
    this->ipRelated = ipRelated;
}

//broadcast out, type 2
Envelope::Envelope(int type, char* macSender, char* macReceiver, int distance, char* requestedIp){
    this->type;
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->distance = distance;
    this->requestedIp = requestedIp;
}

char* Envelope::getMacSender(){
    return this->macSender;
}

char* Envelope::getMacReceiver(){
    return this->macReceiver;
}

char* Envelope::getMacRequested(){
    return this->macRequested;
}

char* Envelope::getIpRelated(){
    return this->ipRelated;
}

Message Envelope::getMessage(){
    return this->message;
}

char* Envelope::getRequestedIp(){
    return this->requestedIp;
}

int Envelope::getDistance(){
    return this->distance;
}

int Envelope::getType(){
    return this->type;
} 
