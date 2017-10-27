/**
* Laboratorio #2 (Cliente)
* Paola Ortega S. B55204
*/

#include "Socket.h"


/**
* PARAM t: tipo de socket. 's' para "SOCK_STREAM" o 'd' para "SOCK_DGRAM"
* PARAM ipv6: aclara el tipo de socket, si usa v6 o no (v4 por omision)
*/
Socket::Socket(char t, bool ipv6 = false) {
	if(t == 's') {
		!ipv6 ? socketDescriptor = socket(AF_INET, SOCK_STREAM, 0) 
		: socketDescriptor = socket(AF_INET6, SOCK_STREAM, 0);
		
	} else if(t == 'd') {
		!ipv6 ? socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0) 
		: socketDescriptor = socket(AF_INET6, SOCK_DGRAM, 0);

	} else printf("Error, tipo invalido. ");

}

Socket::Socket(){
	socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

}

Socket::Socket(int id){
	socketDescriptor = id;
}

int Socket::getSocketD(){
	return socketDescriptor;
}

int Socket::Connect(char * Host, int Port) {
	struct sockaddr_in host_addr;
	int valid = -1;
	host_addr.sin_family = AF_INET;
	valid = inet_aton( Host,  &host_addr.sin_addr );

	if(valid == -1) printf("Error connecting to host. ");

	else {
		host_addr.sin_port = htons( Port );
		int len = sizeof( host_addr );
		connect(socketDescriptor, (sockaddr *) & host_addr, len );

	}

	return valid;
}

int Socket::Connect(char *Host, char *Service){
	size_t len;
    	int st;
    	struct addrinfo hints, *result, *rp;

    	memset(&hints, 0, sizeof(struct addrinfo));
    	hints.ai_family = AF_INET;    
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = 0;
    	hints.ai_protocol = 0;          /* Any protocol */

    	st = getaddrinfo( Host, Service, &hints, &result);

    	for ( rp = result; rp; rp = rp->ai_next ) {
        	st = connect( socketDescriptor, rp->ai_addr, rp->ai_addrlen );
        	if ( 0 == st ) break;
    	}

    	freeaddrinfo( result );

    	return st;
}


int Socket::Read(char * text, int len) {
	int amountRead = read(socketDescriptor, (void *) text, len);
	if(amountRead == -1) printf("Error reading text. ");

	return amountRead;
}

int Socket::Write(char * text, int len) {
	int amountWritten = write(socketDescriptor, (void *) text, len);
	if(amountWritten == -1) printf("Error writing text. ");

	return amountWritten;
}

int Socket::Shutdown() {
	int result = shutdown(socketDescriptor, SHUT_RDWR);

	return result;
}

Socket::~Socket(){
	close(socketDescriptor);
}

void Socket::Close(){
	close(socketDescriptor);
}

int Socket::Listen(int Queue){
	int result = listen(socketDescriptor, Queue);
	if(result == -1) printf("Error listening. ");
	return result;
}

int Socket::Bind(int Port){
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
        server_addr.sin_port = htons( Port );
	int len = sizeof( server_addr );

	int result = bind( socketDescriptor, (const sockaddr *) & server_addr, len );
	if(result == -1) printf("Error binding. ");

	return result;
}

Socket * Socket::Accept(){
	struct sockaddr addr;
	socklen_t len;
	int newSocketDescriptor = accept(socketDescriptor, &addr, &len);
	if(newSocketDescriptor == -1) printf("Error accepting socket. ");
	Socket * newSocket = new Socket(newSocketDescriptor); 
	return newSocket;
}


