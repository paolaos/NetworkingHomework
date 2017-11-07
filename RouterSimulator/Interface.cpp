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
    this->dispatcherPort = dispatcherPort;
    this->wakeUp(macAddress, realIpAddress, dispatcherPort, dispatcherAddress);
}

//este metodo le envia la direccion al dispatcher
void Interface::wakeUp(char* macAddress, char* realIpAddress, int dispatcherPort, char* dispatcherAddress){
   Socket s;
   char buffer[ 512 ];
   s.Connect( dispatcherAddress, dispatcherPort ); // Same port as server
   char str[50];
   strcpy (str,this->macAddress);
   strcat (str,";");
   strcat (str,this->realIpAddress);
   s.Write(  str , 50 );
   /*s.Read( buffer, 512 );	// Read the answer sent back from server
   printf( "%s", buffer );	// Print the string*/
}

//este metodo se encarga de ejecutar la simulacion
void Interface::run(){
    thread startReceiver(&Interface::receive, this);
    thread startEnvelopeProcessor(&Interface::processEnvelope, this);
    thread startMessageProcessor(&Interface::processMessage, this);
    thread startSender(&Interface::processMessage, this);
    
    startReceiver.join();
    startEnvelopeProcessor.join();
    startMessageProcessor.join();
    startSender.join();
    
    
    //prueba
    /*Envelope envelope = inbox.front();
    Message message = envelope.getMessage();
    Action action = message.getRequestedAction();
    printf ("%s, %s, %s, %s, %d, %s, %s\n",envelope.getMacSender(), envelope.getMacReceiver(), message.getIPSender(), message.getIPReceiver(), action.getType(), action.getIP(), message.getMessage());*/
}

//este metodo actua como servidor para los otros nodos
void Interface::receive(){ //nice to have: dos hilos
	//Message message;
    for( ; ;) {
        Socket s1, *s2;
        char a[512];
        s1.Bind(9876);		// Port to access this mirror server
        s1.Listen( 5 );		// Set backlog queue to 5 connections
        s2 = s1.Accept();
        s1.Close();
        s2->Read( a, 75 ); // Read a string from client
        Envelope envelope = this->assemblePackage(a);
        this->inbox.push(envelope);
        s2->Close();
    }
}

//posible mensaje de salida
//mensaje normal = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//mensaje de Broadcast local = MacFuente;MacReceiver;distance;IpSolicitada
//mensaje de dispatcher = macSender;macSolicitada
void Interface::send(){
    for( ; ; ) {
        if(!this->outbox.empty()) {
            Envelope envelope = this->outbox.front();
            Socket s;
            char buffer[ 512 ];
            s.Connect( envelope.getRealIpAddress(), envelope.getRealPort() ); // Same port as server
            char str[200];
            switch(envelope.getType()){
                case 1: {
                    strcpy (str,envelope.getMacSender());
                    strcat (str,";");
                    strcat (str,envelope.getMacReceiver());
                    strcat (str,";");
                    strcat (str,envelope.getMessage().getIpSender());
                    strcat (str,";");
                    strcat (str,envelope.getMessage().getIpReceiver());
                    strcat (str,";");
                    string type = to_string(envelope.getMessage().getRequestedAction().getType());
                    strcat (str, type.c_str());
                    strcat (str,";");
                    strcat (str,envelope.getMessage().getRequestedAction().getIp());
                    strcat (str,";");
                    strcat (str,envelope.getMessage().getMessage());
                    break;
                }
                case 2: { //broadcast
                    strcpy (str,envelope.getMacSender());
                    strcat (str,";");
                    strcat (str,envelope.getMacReceiver());
                    strcat (str,";");
                    string distance = to_string(envelope.getDistance());
                    strcat (str, distance.c_str()); 
                    strcat (str,";");
                    strcat (str,envelope.getRequestedIp());   
                    break;
                }          
                default: { //dispatcher
                    strcpy (str,envelope.getMacSender());
                    strcat (str,";");
                    strcat (str,envelope.getRequestedMac());
                    break;
                }
            }
            s.Write(  str , 200 ); 
        }
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

void Interface::addToSharedMemory(Message message){
    if((string)this->macAddress=="Bolinchas.kevin"){
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        it->second.push(message);
    }
    else if((string)this->macAddress=="LEGO1"){
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        it->second.push(message);
    }
}

//Posibles mensajes entrantes
//mensaje normal = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//mensaje de Broadcast local = maquested;requestedIp;requestedIp
//mensaje de dispatcher = MacSolicitada;IpAsociada;puerto asociado
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
    else if(n==4){
        Envelope envelope(tokens[0], tokens[1], tokens[2]);
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
            Envelope envelope(tokens[0], tokens[1], msg);
            return envelope;
        }
    }
}

//este metodo revisa si el mensaje es broadcast o si se mete en la sharedMemory
void Interface::processEnvelope(){
    for( ; ; ){
        if(!this->inbox.empty()) {
            Envelope envelope = inbox.front();
            if(this->isBroadcast(envelope.getMacReceiver())){
                int distance = this->getDistance(envelope.getRequestedIp());
                if(distance > -1){
                    map<char*, char*>::iterator it = this->cacheTable.find(envelope.getMacReceiver()); //busco si tengo la ip real
                    if(it!=cacheTable.end()) {
                        char* str = strdup(it->second);
                        char** tokens = new char*[10];
                        char* token = strtok (str,";");
                        int n = 0;
                        while (token != NULL){
                            tokens[n] = token;
                            token = strtok (NULL, ";");
                            ++n;
                        }
                        Envelope envelopeBroadcast(2, this->macAddress, "*", atoi(tokens[1]), tokens[0], distance, envelope.getRequestedIp());
                        this->outbox.push(envelopeBroadcast);
                    }
                    else { //se la pido al dispatcher
                        Envelope envelopeDispatcher(3, envelope.getMacReceiver(), "*");
                        this->outbox.push(envelopeDispatcher);
                        //se anade de nuevo a la cola de entrada ya que no tengo la ipreal
                        this->inbox.push(envelope);
                    }	
                } else {
                    printf("Invalid requested ip address");   
                    exit (EXIT_FAILURE);
                }

            }
            else if(envelope.getIsDispatcher()){
                char str[100];
                strcpy (str,envelope.getRealIpAddress());
                strcat (str,";");
                string realPort = to_string(envelope.getRealPort());
                strcat (str,realPort.c_str());
                cacheTable.insert ( pair<char*,char*>(envelope.getRequestedMac(),str) );
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
    for( ; ;){
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
                        char* str = strdup(it->second);
                        char** tokens = new char*[10];
                        char* token = strtok (str,";");
                        int n = 0;
                        while (token != NULL){
                            tokens[n] = token;
                            token = strtok (NULL, ";");
                            ++n;
                        }

                        Envelope envelope(1, this->macAddress, macAddressNext, tokens[0], atoi(tokens[1]), message);
                        this->outbox.push(envelope);
                    }
                    else { //se la pido al dispatcher
                        Envelope envelope(3, this->macAddress, macAddressNext);
                        this->outbox.push(envelope);
                        //se anade de nuevo a la memoria compartida ya que no tengo la ipreal
                        this->addToSharedMemory(message);
                    }
                } else {
                    printf("Error with mac address. ");
                    exit (EXIT_FAILURE);
                } 
            }
        }
    }
}



