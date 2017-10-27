#include "Interface.h"

Interface::Interface(list<Route>* ipTable, map<bool, queue<Message> >* messagePool){
    this->ipTable = ipTable;
    this->messagePool = messagePool;
}

Interface::Interface(){}

void Interface::run(){
    this->receive();

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

//message = MacFuente;MacDestino;IPFuente;IPDestino;Mensaje
void Interface::assemblePackage(char* message){
    char* str = strdup(message);
    char** tokens = new char*[10];
    char* token = strtok (str,";");
    int n = 0;
    while (token != NULL){
        tokens[n] = token;
        token = strtok (NULL, ";");
        ++n;
    }
    for(int i=0; i<5; ++i){
        printf ("%s\n",tokens[i]);
    }
    Action action;
    Message message(&tokens[2], &tokens[3], &tokens[4], action);
	Envelope envelope(&tokens[0], &tokens[1], message, 1);
	printf ("%s\n",envelope.getMacSender);
	printf ("%s\n",envelope.getMacReceiver);
	printf ("%s\n",envelope.getMessage.getIPSender);
}

bool Interface::isBroadcast(char*){
    return false;
}

Route Interface::checkIPTable(char*){
}

Envelope Interface::packEnvelopeBroadcast(Route route){
}

Envelope Interface::packEnvelope(Message message){
}

void Interface::send(Envelope envelope){
}






