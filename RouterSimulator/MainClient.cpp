#include "Socket.h" 
#include <stdio.h>
#include <iostream>
 
int main( int argc, char * argv[] ) { 
   	Socket s;
   	char buffer[ 512 ];
   	s.Connect( "127.0.0.1", 9876 ); // Same port as server
	std::string test = "Bolinchas.Jorge;*;12.0.0.5";
   	s.Write(  (char*)test.c_str() , test.length() );
}
