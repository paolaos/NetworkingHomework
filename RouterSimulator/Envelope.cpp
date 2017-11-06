#include "Envelope.h"

//normal
Envelope::Envelope(char* macSender, char* macReceiver, Message message){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->message = message;
}

//ask broadcast
Envelope::Envelope(char* macSender, char* macReceiver, char* requestedIp){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->requestedIp = requestedIp;
}
//answer broadcast
Envelope::Envelope(char* macSender, char* macReceiver, int distance, char* requestedIp){
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

Message Envelope::getMessage(){
    return this->message;
}

char* Envelope::getRequestedIp(){
    return this->requestedIp;
}

int Envelope::getDistance(){
    return this->distance;
}
