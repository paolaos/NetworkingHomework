#include "Interface.h"

Interface::Interface(list<Route>* ipTable, map<bool, queue<Message> >* messagePool, char* ipAddress, char* macAddress, char* realIpAddress, char* dispatcherAddress){
    this->ipTable = ipTable;
    this->messagePool = messagePool;
	this->ipAddress = ipAddress;
	this->macAddress = macAddress;
	this->realIpAddress = realIpAddress;
	this->dispatcherAddress = dispatcherAddress;

}

Interface::Interface(){}

void Interface::run(){
	//mientras programa siga corriendo
    this->receive();

}

void Interface::receive(){ //nice to have: dos hilos
	//Message message;

/*primero voy a revisar del lado interno, cuyos mensajes estan en envelopes y en la cola*/

	//Envelope envelope = this->inbox.dequeue();
	//if(envelope != -1) {
		//message = this->unwrap(envelope);
		//this->processMessage(message, 0);

	//}

/*ahora reviso del shared memory, cuyos mensajes son simplemente mensajes*/
	//message = checkSharedMemory(); HACER
	//if(message != -1) {
		//this->processMessage(message, 1);

//}

}

Message Interface::checkSharedMemory() {
}

//message = MacFuente;MacDestino;IPFuente;IPDestino;Mensaje
Envelope Interface::assemblePackage(char* message){
    char* str = strdup(message);
    char** tokens = new char*[10];
    char* token = strtok (str,";");
    int n = 0;
    while (token != NULL){
        tokens[n] = token;
        token = strtok (NULL, ";");
        ++n;
    }
    for(int i=0; i<5; ++i)
        printf ("%s\n",tokens[i]);
    
    struct Action action;
	action.type = 1;
	strcpy(action.ip, tokens[2]);

    Message msg(tokens[2], tokens[3], tokens[4], action);
	Envelope envelope(tokens[0], tokens[1], msg, 1);
	return envelope;

}

bool Interface::isBroadcast(char*){
    return false;
}


Route Interface::checkIPTable(char* ipAddress){
}

Envelope Interface::packEnvelopeBroadcast(Route route){
}


void Interface::sendInternally(Envelope envelope, bool isBroadcast){
	//implementar
}

void Interface::sendToSharedMemory(Message message){
	//implementar
}


void Interface::processMessage(Message message, bool fromSharedMem){
	//char* sender = message.getIPSender();
	//char* receiver = message.getIPReceiver();	

	//if(receiver no es igual a mi direccion ip) {
		//if(!fromSharedMem) {
			//sendToSharedMemory(message);
		//}
		//else {
			//Envelope envelope = wrap(message);
			//sendInternally(envelope, 0); //por el momento siempre va a ser 0 porque se asume que ya se todas las direcciones y que solo conozco a David.
		//}		
	//} else {
		//cout <<message.getMessage(); //luego tenemos que ver que hacemos con el tipo de action!
	//}

}

Message Interface::unwrap(Envelope envelope) {
	//implementar
}

Envelope Interface::wrap(Message message) {
	//return this->assemblePackage(message.getMessage);
}





