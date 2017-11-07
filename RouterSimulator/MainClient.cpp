#include "Socket.h" 
#include <stdio.h>
#include <iostream>
 
int main( int argc, char * argv[] ) { 
   	Socket s;
   	char buffer[ 512 ];

   	s.Connect( "10.1.130.92", 9876 ); // Same port as server
	/*std::string test = "Bolinchas.jorge;Bolinchas.kevin;192.168.0.109;192.168.0.0;0;HOLA!!!";
   	s.Write(  (char*)test.c_str() , test.length() );*/
	s.Write( "hola" , 4);
   	s.Read( buffer, 512 );	// Read the answer sent back from server
   	printf( "%s", buffer );	// Print the string

}
