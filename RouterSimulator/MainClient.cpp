#include "Socket.h" 
#include <stdio.h>
#include <iostream>
 
int main( int argc, char * argv[] ) { 
   	Socket s;
   	char buffer[ 512 ];
   	s.Connect( "10.1.131.12", 9876 ); // Same port as server
	std::string test = "Bolinchas.Jorge;*;12.0.0.5";
   	s.Write(  (char*)test.c_str() , test.length() );
}
