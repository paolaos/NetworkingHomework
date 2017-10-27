#include "Envelope.h"

Envelope::Envelope(char* macSender, char* macReceiver, Message message, char sequenceNumber){
    this->macSender = macSender;
    this->macReceiver = macReceiver;
    this->message = message;
    this->sequenceNumber = sequenceNumber;
}

//Envelope::Envelope(){}

char* Envelope::getMacSender(){
    return this->macSender;
}

char* Envelope::getMacReceiver(){
    return this->macReceiver;
}
Message Envelope::getMessage(){
    return this->message;
}

char Envelope::getSequenceNumber(){
    return this->sequenceNumber;
}
