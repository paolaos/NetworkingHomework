#include "Interface.h"

Interface::Interface(list<Route>* ipTable, map<bool, queue<Message> >* messagePool){
    this->ipTable = ipTable;
    this->messagePool = messagePool;
}

Interface::Interface(){}

void Interface::run(){
    /* un hilo hace this->receive(); this.assemblePackage() se pone en cola
    el otro saca de la cola, desempaqueta, luego revisa si es broadcast, luego si es va a la IPTable, si no mete a la memoria compartida,
    */

//Pruebas
    Envelope envelope = this->assemblePackage("Bolinchas.jorge;Bolinchas.kevin;201.1.1.1;192.168.0.0;0;HELLO!!!!");
//Solo para imprimir
    Message message = envelope.getMessage();
    Action action = message.getRequestedAction();
    printf ("%s, %s, %s, %s, %d, %s, %s\n",envelope.getMacSender(), envelope.getMacReceiver(), message.getIPSender(), message.getIPReceiver(), action.getType(), action.getIP(), message.getMessage());
    int distance = 0;
    if (this->isBroadcast(message.getIPReceiver())){
            //printf("It is Broadcast\n");
	    distance = this->checkIPTable(action.getIP());
    }
}

void Interface::receive(){
    Envelope envelope();
    //SocketServer
    /*int childpid;
    char a[512];
    Socket s1, *s2;

    s1.Bind( 9876 );		// Port to access this mirror server
    s1.Listen( 5 );		// Set backlog queue to 5 conections

    for( ; ; ) {
        s2 = s1.Accept();	 	// Wait for a conection
        childpid = fork();	// Create a child to serve the request
        if ( childpid < 0 )
            perror("server: fork error");
        else if (0 == childpid) {  // child code
            s1.Close();	// Close original socket in child
            s2->Read( a, 512 ); // Read a string from client
            //envelope = this->assemblePackage(a);
            //s2->Write( a, 500 );	// Write it back to client
            exit( 0 );	// Exit
        }
        s2->Close();		// Close socket in parent
    }*/
}

//current message = MacFuente;MacDestino;IPFuente;IPDestino;TipoAccion;IPAccion;Mensaje
//old message = MacFuente;MacDestino;IPFuente;IPDestino;
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
    /*for(int i=0; i<n; ++i){
	printf("%s\n",tokens[i]);
    }*/
    //printf("%d\n", n);
    if(n<6 && n>7){
       printf("Envelope can't assemble, the message received by the socket doesn't satisfy the requirement length");
       exit (EXIT_FAILURE);
    }
    Message msg;
    if(atoi(tokens[4])==0){
	//printf("No Action");
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
    Envelope envelope(tokens[0], tokens[1], msg, 1);
    return envelope;
}

bool Interface::isBroadcast(char* ip){
    if((string)ip==BROADCAST){
       return true;
    }
    return false;
}

int Interface::checkIPTable(char *network}{
}

Envelope Interface::packEnvelope(Message message){
}

void Interface::send(Envelope envelope){
}

list<Route>* Interface::getIPTable(){
    return this->ipTable;
}

map<bool, queue<Message> >* Interface::getMessagePool(){
    return this->messagePool;
}

queue<Envelope> Interface::getInbox(){
    return this->inbox;
}

queue<Envelope> Interface::getOutbox(){
    return this->outbox;
}

char* Interface::getIPAddress(){
    return this->ipAddress;
}

char* Interface::getMacAddress(){
    return this->macAddress;
}

map<char*, char*> Interface::getMacTable(){
    return this->macTable;
}








