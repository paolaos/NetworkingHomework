#include <stdio.h>
#include <string.h>
#include <iostream>
#include "Interface.h"


using namespace std;
int main(){
    Interface interface; 
    interface.assemblePackage("Bolinchas.jorge;Bolinchas.kevin;201.1.1.1;192.168.1.1;HOLA!!!!"); 
    /*char* test = "Bolinchas.jorge;Bolinchas.kevin;201.1.1.1;192.168.1.1;HOLA!!!!";
    string str = (string)test;
    char** tokens = new char*[10];
    char * token;
    printf ("Splitting string \"%s\" into tokens:\n",str);
    token = strtok (/*(char *)str.c_str()test,";");
    int n = 0;
    while (token != NULL){
	//printf ("%s\n",token);
        tokens[n] = token;
        token = strtok (NULL, ";");
        ++n;
    }
    for(int i=0; i<5; ++i){
        printf ("%s\n",tokens[i]);
    }*/
    return 0;
}
