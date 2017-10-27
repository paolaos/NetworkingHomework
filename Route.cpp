#include "Route.h"

Route::Route(char* network, int distance, char* through){
    this->network = network;
    this->distance = distance;
    this->through = through;
}

char* Route::getNetwork(){
    return this->network;
}

int Route::getDistance(){
    return this->distance;
}

char* Route::getThrough(){
    return this->through;
}
