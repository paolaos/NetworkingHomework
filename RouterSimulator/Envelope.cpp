#include "Envelope.h"

//normal in
Envelope::Envelope(char* macSender, char* macReceiver, Message message){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->message = message;
    this->isDispatcher = false;
}

//normal out, type 1
Envelope::Envelope(int type, char* macSender, char* macReceiver, char* realIpAddress, int realPort, Message message){
    this->type = type;
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->message = message;
    this->realIpAddress = realIpAddress;
    this->realPort = realPort;
}

//broadcast in
Envelope::Envelope(char* macSender, char* macReceiver, char* requestedIp){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->requestedIp = requestedIp;
    this->isDispatcher = false;
}

//broadcast out, type 2
Envelope::Envelope(int type, char* macSender, char* macReceiver, int realPort, char* realIpAddress, int distance, char* requestedIp){
    this->type;
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->distance = distance;
    this->realIpAddress = realIpAddress;
    this->realPort = realPort;
    this->requestedIp = requestedIp;
}

//dispatcher in
Envelope::Envelope(char* requestedMac, char* requestedIp, int requestedPort){
    this->requestedMac = requestedMac;
    this->requestedIp = requestedIp;    
    this->requestedPort = requestedPort;
    this->isDispatcher = true;
}

//dispatcher out, type 3
Envelope::Envelope(int type, char* macSender, char* macRequested){
    this->type = type;
    this->macSender = macSender;
    this->requestedMac = requestedMac;
}

char* Envelope::getMacSender(){
    return this->macSender;
}

char* Envelope::getMacReceiver(){
    return this->macReceiver;
}

char* Envelope::getRequestedMac(){
    return this->requestedMac;
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

int Envelope::getRealPort(){
    return this->realPort;
}

char* Envelope::getRealIpAddress(){
    return this->realIpAddress;
}

bool Envelope::getIsDispatcher(){
    return this->isDispatcher;
}
