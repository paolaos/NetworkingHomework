#include "InterfacePaola.h"

//constructor a utilizar
InterfacePaola::InterfacePaola(list<Route>* ipTable, map<bool, queue<Message> >* messagePool, char* network, char* ipAddress, char* macAddress, char* realIpAddress, int realPort, char* dispatcherAddress, int dispatcherPort){
    this->empty = false;
    this->ipTable = ipTable;
    this->messagePool = messagePool;
    this->network = network;
	this->ipAddress = ipAddress;
	this->macAddress = macAddress;
	this->realIpAddress = realIpAddress;
    this->realPort = realPort;
	this->dispatcherAddress = dispatcherAddress;
    this->dispatcherPort = dispatcherPort;
    //this->wakeUp(macAddress, realIpAddress, dispatcherPort, dispatcherAddress);
}

//este metodo le envia la direccion al dispatcher
void InterfacePaola::wakeUp(char* macAddress, char* realIpAddress, int dispatcherPort, char* dispatcherAddress){
    Socket s;
    s.Connect( dispatcherAddress, dispatcherPort ); // Same port as server    
	char str[50];
	strcpy (str,"0");
    strcat (str,";");	
    strcpy (str,this->macAddress);
    strcat (str,";");
	strcpy (str,"Legos0");
    strcat (str,";");
    strcat (str,this->realIpAddress);
    strcat (str,";");
    strcat (str,"PONER SIZE");
    strcat (str,";");
    strcat (str,this->ipAddress);
    strcat (str,";");
    strcpy (str,this->macAddress);
    strcat (str,";");
    strcpy (str,this->realIpAddress);
    strcat (str,";");
	string realPort = to_string(this->realPort);
    strcat (str,realPort.c_str());
	strcat (str,";");

    string message = str;
    s.Write(  (char*)message.c_str() , message.length() );
}

//este metodo se encarga de ejecutar la simulacion
void InterfacePaola::run(){

    this->receive();
    
}


//este metodo actua como servidor para los otros nodos
void InterfacePaola::receive(){ 
	int childpid;
    int status = 0;
   	char receivedBuffer[512];
  	Socket s1, *s2;
    int id = shmget( 123456, 1024, 0700 | IPC_CREAT );
    char* sharedMemory = (char *) shmat( id, NULL, 0 );
   	s1.Bind( this->realPort );		// Port to access this mirror server
   	s1.Listen( 5 );		// Set backlog queue to 5 conections

    for( ; ; ) {
		s2 = s1.Accept();	 	// Wait for a conection
	  	childpid = fork();	// Create a child to serve the request
	  	if ( childpid < 0 )
	     	perror("server: fork error");
	  	else if (0 == childpid) {  // child code
	    	s1.Close();	// Close original socket in child
	        s2->Read( receivedBuffer, 512 ); // Read a string from client
            //printf(receivedBuffer);
            strcpy( sharedMemory, receivedBuffer );
            exit(0);
	  	}
        while (waitpid(childpid, NULL, 0)>0);   
        strcpy( receivedBuffer, sharedMemory );
        printf(receivedBuffer);
        printf("\n");
        Envelope envelope = this->assemblePackage(receivedBuffer);
	    this->inbox.push(envelope);
        this->processEnvelope();
        //this->send();
	  	s2->Close();		// Close socket in parent
   	}
    shmdt( sharedMemory );
    shmctl( id, IPC_RMID, NULL );
}


/*

Ya tengo el envelope!

0: Me estoy despertando y este es mi ip
1: Ocupo el (ip, puerto) de este nodo
2: Respuesta del dispatcher
3: Broadcast
4: Red normal
*/
//posible mensaje de salida
//mensaje normal = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//mensaje de Broadcast local = MacFuente;MacReceiver;distance;IpSolicitada
//mensaje de dispatcher = macSender;macSolicitada
void InterfacePaola::send(){
        if(!this->outbox.empty()) {
            Envelope envelope = this->outbox.front();
            this->outbox.pop();
            Socket s;
            char buffer[ 512 ];
            s.Connect( envelope.getRealIpAddress(), envelope.getRealPort() ); // Same port as server
            char str[200];
            switch(envelope.getType()){
                case 1: { //wake up
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
                case 2: { //dispatcher question
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
				case 3: { //dispatcher answer
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
				case 4: { //mensaje normal
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
                default: { //no deberia caer aqui
                    strcpy (str,envelope.getMacSender());
                    strcat (str,";");
                    strcat (str,envelope.getRequestedMac());
                    break;
                }
            }
            string test = str;
   	        s.Write(  (char*)test.c_str() , test.length() );
        }
}

Message InterfacePaola::checkSharedMemory() {
    if((string)this->macAddress=="Bolinchas.Kevin"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        mutx.unlock();
        Message m = it->second.front();
        it->second.pop();
        return m;
    }
    else if((string)this->macAddress=="LEGO1"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        mutx.unlock();
        Message m = it->second.front();
        it->second.pop();
        return m;
    }

}

void InterfacePaola::addToSharedMemory(Message message){
    if((string)this->macAddress=="Bolinchas.Kevin"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        mutx.unlock();        
        it->second.push(message);
    }
    else if((string)this->macAddress=="LEGO1"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        mutx.unlock();        
        it->second.push(message);
    }
}

//Posibles mensajes entrantes
//mensaje normal = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//mensaje de Broadcast local = maquested;*;requestedIp
//mensaje de dispatcher = MacSolicitada;IpAsociada;puerto asociado
Envelope InterfacePaola::assemblePackage(char* message){
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
            //exit (EXIT_FAILURE);
        }
        else{
            //printf("%s, %s, %s\n", tokens[0], tokens[1], tokens[2]);
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
            printf("Envelope can't assemble, the message received by the socket doesn't satisfy the requirement length\n");
            //exit (EXIT_FAILURE);
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
                printf("Invalid action type\n");   
                //exit (EXIT_FAILURE);
            }
            Envelope envelope(tokens[0], tokens[1], msg);
            return envelope;
        }
    }
}

/*
Envelope: Tipo(char), MacFuente(char[7]), MacDestino(char[7]), Mensaje(Si )
*/
void InterfacePaola::processEnvelope(){
        if(!this->inbox.empty()) {
            Envelope envelope = this->inbox.front();
            this->inbox.pop();
            if(this->isBroadcast(envelope.getMacReceiver())){
                printf("Es un broadcast\n");
                int distance = this->getDistance(envelope.getRequestedIp());
                cout << distance << endl;
                if(distance > -1){
                    //cout << envelope.getMacReceiver();
                    map<char*, char*>::iterator it = this->cacheTable.find("Bolinchas.Jorge"); //busco si tengo la ip real
                    if(it!=cacheTable.end()) {
                        cout << "hola\n";
                        char* str = strdup(it->second);
                        char** tokens = new char*[10];
                        char* token = strtok (str,";");
                        int n = 0;
                        while (token != NULL){
                            tokens[n] = token;
                            token = strtok (NULL, ";");
                            ++n;
                        }
                        //cout << atoi(tokens[1]) << ";" << tokens[0]<< ";"  << distance<< ";"  << envelope.getRequestedIp();
                        Envelope envelopeBroadcast(2, this->macAddress, "*", atoi(tokens[1]), tokens[0], distance, envelope.getRequestedIp());
                        this->outbox.push(envelopeBroadcast);
                    }
                    	
                } else {
                    printf("Invalid requested ip address\n");   
                    //exit (EXIT_FAILURE);
                }

            }
            else if(envelope.getIsDispatcher()){
                char str[100] = "";
                strcpy (str,envelope.getRealIpAddress());
                strcat (str,";");
                string realPort = to_string(envelope.getRealPort());
                strcat (str,realPort.c_str());
                cout << str << endl;
                cacheTable.insert ( pair<char*,char*>(envelope.getRequestedMac(),str) );
            }
            else {
                Message message = envelope.getMessage();
                if((string)macAddress=="Bolinchas.Kevin"){
                    if((string)message.getIpReceiver() == "Bolinchas.Kevin")
                        cout << "Mensaje para Kevin: " << (string)message.getMessage() << endl;
                    else {
                        mutx.lock();
                        map<bool,queue<Message> >::iterator it = messagePool->find(false);
                        mutx.unlock();
                        it->second.push(message);
                    }
                }
                else if((string)macAddress=="LEGO1"){
                    if((string)message.getIpReceiver() == "LEGO1")
                        cout << "Mensaje para Paola: " << (string)message.getMessage() << endl;
                    else {
                        mutx.lock();
                        map<bool,queue<Message> >::iterator it = messagePool->find(true);
                        mutx.unlock();
                        it->second.push(message);
                    }
                }
                else {
                    printf("Message with invalid recepient\n");   
                    //exit (EXIT_FAILURE);
                }
            }
        }
}

bool InterfacePaola::isBroadcast(char* macReceiver){
    return (string)macReceiver == "*";

}

//este metodo devuelve la distancia que un broadcast pregunta
int InterfacePaola::getDistance(char* ipAddress){
    char* network = this->getNetwork(ipAddress);
    //cout << network;
    mutx.lock();
    list<Route>::iterator ite = this->ipTable->begin();
    while(ite!=this->ipTable->end() && *ite->getNetwork()!=*ipAddress){
        ++ite;
    }
    mutx.unlock();
    if(*ite->getNetwork()==*ipAddress){
        return ite->getDistance();
    }
    return -1;
}


//este metodo devuelve a traves de quien se manda
char* InterfacePaola::checkIpTable(char* ipAddress){
    char* network = this->getNetwork(ipAddress);
    mutx.lock();
    list<Route>::iterator ite = this->ipTable->begin();
    while(ite!=this->ipTable->end() && *ite->getNetwork()!=*ipAddress){
        ++ite;
    }
    mutx.unlock();
    if(*ite->getNetwork()==*ipAddress){
        return ite->getThrough();
    }
	
	return "";
}

char* InterfacePaola::getNetwork(char* ipAddress){
    char* str = strdup(ipAddress);
    char** tokens = new char*[10];
    char* token = strtok (str,".");
    int n = 0;
    while (token != NULL && n<4){
        tokens[n] = token;
        token = strtok (NULL, ".");
        ++n;
    }
   // cout << tokens[0] << "." << tokens[1] << "." << tokens[2] << "." << tokens[3];
    string network = "";
    if(atoi(tokens[0])>=0 && atoi(tokens[0])<128){
        network += (string)tokens[0] + ".0.0.0";
    }
    else if (atoi(tokens[0])>=128 && atoi(tokens[0])<192){
        network += (string)tokens[0] + "." + (string)tokens[1] + ".0.0";
    }
    else if(atoi(tokens[0])>=192& atoi(tokens[0])<224){
        network += (string)tokens[0] + "." + (string)tokens[1] + "." + (string)tokens[2] + ".0";
    }
    return (char*)network.c_str();
}

bool InterfacePaola::isSharedMemoryEmpty(){
    if((string)this->macAddress=="Bolinchas.Kevin"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(false);
        mutx.unlock();
        if(it->second.empty()){
            return true;
        }
    }
    else if((string)this->macAddress=="LEGO1"){
        mutx.lock();
        map<bool,queue<Message> >::iterator it = messagePool->find(true);
        mutx.unlock();
        if(it->second.empty()){
            return true;
        }
    }
    return false;
}

//este metodo ve que hacer con lo que hay en shared memory
void InterfacePaola::processMessage(){
    //while(true){
        if(!isSharedMemoryEmpty()){
            Message message = this->checkSharedMemory();
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
                        cout << "I'm gonna ask to the dispatcher\n";
                        Envelope envelope(3, macAddressNext, "*");
                        this->outbox.push(envelope);
                        //se anade de nuevo a la memoria compartida ya que no tengo la ipreal
                        this->addToSharedMemory(message);
                    }
                } else {
                    printf("Error with mac addressin ip table\n");
                    //exit (EXIT_FAILURE);
                } 
            }
        }
    //}
}



