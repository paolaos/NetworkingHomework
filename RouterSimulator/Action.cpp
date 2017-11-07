#include "Action.h"

Action::Action(int type, char* ip){
    this->type = type;
    this->ip = ip;
}

Action::Action(){}

int Action::getType(){
    return this->type;
}

char* Action::getIp(){
    return this->ip;
}
