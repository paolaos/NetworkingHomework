#include "Socket.h" 
#include <stdio.h>
 
int main( int argc, char * argv[] ) { 
   Socket s;
   char buffer[ 512 ];

   s.Connect( "10.30.7.171", 9876 ); // Same port as server
   s.Write(  "Bolinchas.jorge;Bolinchas.kevin;192.168.0.109;192.168.0.0;0;HOLA!!!" , 75 );
   /*s.Write(  argv[1] , 50 );*/		
   s.Read( buffer, 512 );	// Read the answer sent back from server
   printf( "%s", buffer );	// Print the string

}
