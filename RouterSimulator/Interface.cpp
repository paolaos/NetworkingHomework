#include "Interface.h"

//constructor a utilizar
Interface::Interface(list<Route>* ipTable, map<bool, queue<Message> >* messagePool, char* network, char* ipAddress, char* macAddress, char* realIpAddress, char* dispatcherAddress, int dispatcherPort){
    this->empty = false;
    this->ipTable = ipTable;
    this->messagePool = messagePool;
    this->network = network;
	this->ipAddress = ipAddress;
	this->macAddress = macAddress;
	this->realIpAddress = realIpAddress;
	this->dispatcherAddress = dispatcherAddress;
    //this->wakeUp(macAddress, realIpAddress,dispatcherPort, dispatcherAddress);
}

//este metodo le envia la direccion al dispatcher
void Interface::wakeUp(char* macAddress, char* realIpAddress, int dispatcherPort, char* dispatcherAddress){
   Socket s;
   char buffer[ 512 ];
   s.Connect( dispatcherAddress, dispatcherPort ); // Same port as server
   char str[50];
   strcpy (str,macAddress);
   strcat (str,";");
   strcat (str,realIpAddress);
   s.Write(  str , 50 );
   /*s.Read( buffer, 512 );	// Read the answer sent back from server
   printf( "%s", buffer );	// Print the string*/
}

//este metodo se encarga de ejecutar la simulacion
void Interface::run(){
    this->receive();
    this->processEnvelope();
    this->processMessage();
    this->send();
    
    //prueba
    /*Envelope envelope = inbox.front();
    Message message = envelope.getMessage();
    Action action = message.getRequestedAction();
    printf ("%s, %s, %s, %s, %d, %s, %s\n",envelope.getMacSender(), envelope.getMacReceiver(), message.getIPSender(), message.getIPReceiver(), action.getType(), action.getIP(), message.getMessage());*/
}

//este metodo actua como servidor para los otros nodos
void Interface::receive(){ //nice to have: dos hilos
	//Message message;
    Socket s1, *s2;
    char a[512];
    s1.Bind(9876);		// Port to access this mirror server
    s1.Listen( 5 );		// Set backlog queue to 5 connections
    s2 = s1.Accept();
    s1.Close();
    s2->Read( a, 75 ); // Read a string from client
    Envelope envelope = this->assemblePackage(a);
    this->inbox.push(envelope);
    s2->Write( "ACK", 3 ); // Write it back to client
    s2->Close();

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

void Interface::send(){
    Envelope envelope = this->outbox.front();
    switch(envelope.getType()){
        case 1:
            //normal, va para Daniel o Jorge
            break;
        case 2:
            //broadcast, va para Jorge
            break;
        default: //type==3
            //va para el dispatcher
            break;
    }
}

Message Interface::checkSharedMemory() {
    if((string)this->macAddress=="Bolinchas.kevin"){
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        if(it->second.empty()){
            this->empty = true;
        }
        return it->second.front();
    }
    else if((string)this->macAddress=="LEGO1"){
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        if(it->second.empty()){
            this->empty = true;
        }
        return it->second.front();
    }

}

void Interface::addToShareMemory(Message message){
    if((string)this->macAddress=="Bolinchas.kevin"){
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        it->second.push(message);
    }
    else if((string)this->macAddress=="LEGO1"){
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        it->second.push(message);
    }
}

//Posibles mensajes entrantes
//mensaje normal = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//mensaje de Broadcast local = MacFuente;*;IpSolicitada
//mensaje de dispatcher = *;MacSolicitada;IpAsociada
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
    
    //broadcast
    if(string(tokens[1])=="*"){
        if(n!=3){
            printf("Envelope can't assemble, the message received by the socket doesn't satisfy the requirement length");
            exit (EXIT_FAILURE);
        }
        else{
            Envelope envelope(tokens[0], tokens[1], tokens[2]);
            return envelope;
        }
    }
    
    //dispatcher
    else if(n==2){
        Envelope envelope(3, tokens[0], tokens[1]);
        return envelope;
    }
    
    //normal
    else{
        if(n<6 || n>7){
            printf("Envelope can't assemble, the message received by the socket doesn't satisfy the requirement length");
            exit (EXIT_FAILURE);
        }
        else{
            Message msg;
            if(atoi(tokens[4])==0){
                Action action(atoi(tokens[4]), "NoAction");
                Message msg1(tokens[2], tokens[3], tokens[5], action);
                msg = msg1;
            }
            else if(atoi(tokens[4])==1){
                Action action(atoi(tokens[4]), tokens[5]);
                Message msg1(tokens[2], tokens[3], tokens[6], action);
	            msg = msg1;
            }
            else{
                printf("Invalid action type");   
                exit (EXIT_FAILURE);
            }
            Envelope envelope(1, tokens[0], tokens[1], msg);
            return envelope;
        }
    }
}

//este metodo revisa si el mensaje es broadcast o si se mete en la sharedMemory
void Interface::processEnvelope(){
    Envelope envelope = inbox.front();
    if(this->isBroadcast(envelope.getMacReceiver())){
        int distance = this->getDistance(envelope.getRequestedIp());
		if(distance > -1){
        	Envelope envelope(2, this->macAddress, "*", distance, envelope.getRequestedIp());
        	this->outbox.push(envelope);
		} else {
			printf("Invalid requested ip address");   
            exit (EXIT_FAILURE);
		}
			
    }
    else {
        Message message = envelope.getMessage();
        if((string)macAddress=="Bolinchas.kevin"){
			if((string)message.getIpReceiver() == "Bolinchas.kevin")
				cout << "Mensaje para Kevin: " << (string)message.getMessage() << endl;
			else {
	            map<bool,queue<Message> >::iterator it = messagePool->find(false);
	            it->second.push(message);

			}
        }
        else if((string)macAddress=="LEGO1"){
			if((string)message.getIpReceiver() == "LEGO1")
				cout << "Mensaje para Paola: " << (string)message.getMessage() << endl;
			else {
            	map<bool,queue<Message> >::iterator it = messagePool->find(true);
            	it->second.push(message);
			}
        }
		else {
			printf("Message with invalid recepient");   
            exit (EXIT_FAILURE);
		}
    }
}

bool Interface::isBroadcast(char* macReceiver){
    return (string)macReceiver == "*";

}

//este metodo devuelve la distancia que un broadcast pregunta
int Interface::getDistance(char* ipAddress){
    list<Route>::iterator ite = this->ipTable->begin();
    while(ite!=this->ipTable->end() && *ite->getNetwork()!=*ipAddress){
        ++ite;
    }
    if(*ite->getNetwork()==*ipAddress){
        return ite->getDistance();
    }
    return -1;
}


//este metodo devuelve a traves de quien se manda
char* Interface::checkIpTable(char* ipAddress){
    list<Route>::iterator ite = this->ipTable->begin();
    while(ite!=this->ipTable->end() && *ite->getNetwork()!=*ipAddress){
        ++ite;
    }
    if(*ite->getNetwork()==*ipAddress){
        return ite->getThrough();
    }
	
	return "";
}

//este metodo ve que hacer con lo que hay en shared memory
void Interface::processMessage(){
    Message message = this->checkSharedMemory();
    if(!this->empty){
		if((string)message.getIpReceiver() == this->ipAddress)
			cout << "Mensaje para mi: " << (string)message.getMessage() << endl;

		else {
	        //lo empaqueto
			char* macAddressNext = this->checkIpTable(message.getIpReceiver());
			if((string)macAddressNext != "") {
	            map<char*, char*>::iterator it = this->cacheTable.find(macAddressNext); //busco si tengo la ip real
	            if(it!=cacheTable.end()) {
				    Envelope envelope(1, this->macAddress, macAddressNext, message);
	                this->outbox.push(envelope);
	            }
	            else { //se la pido al dispatcher
                    Envelope envelope(3, macAddressNext, "*");
                    this->outbox.push(envelope);
                    //se anade de nuevo a la memoria compartida ya que no tengo la ipreal
                    this->addToShareMemory(message);
	            }
			} else {
				printf("Error with mac address. ");
            	exit (EXIT_FAILURE);
			} 
            
            /*//ahora busco el IP asociado

			Envelope envelope(this->macAddress, macAddressNext, message);
			this->sendInternally(envelope);*/

			/*if((string)macAddress == "LEGO1")
				macAddressNext = "LEGO2";
			else if((string)macAddress == "bolinchas.Kevin")
				macAddressNext = "bolinchas.Jorge";

			else {
				printf("Error with Interface's mac address. ");
            	exit (EXIT_FAILURE);
			}*/
		}
    }
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


//char* Interface::callDispatcher(char* ) {}

/*void Interface::sendInternally(Envelope envelope){
	
}*/




