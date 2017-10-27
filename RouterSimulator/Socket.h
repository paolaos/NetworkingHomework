/**
* Laboratorio #2 (Cliente)
* Paola Ortega S. B55204
*/

#ifndef _Socket
#define _Socket

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

class Socket{
	private:
		int socketDescriptor;
		

	public:
		Socket();
		Socket(char, bool);
		Socket(int);
		~Socket();
		void Close();
		int Connect(char *, int);
		int Connect(char *, char *);
		int Read(char *, int);
		int Write(char *, int);
		int Shutdown();
		int Listen(int);
		int Bind(int);
		Socket * Accept();
		int getSocketD();
};

#endif
