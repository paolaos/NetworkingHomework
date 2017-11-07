#include "RouterSimulator.h"

RouterSimulator::RouterSimulator(){
    queue<Message> messageQueue;
    messagePool.insert ( pair<bool,queue<Message>>(true,messageQueue) );
    messagePool.insert ( pair<bool,queue<Message>>(false,messageQueue) );
    this->readIpTable();
}

void RouterSimulator::readIpTable(){
    string line;
    ifstream ipTable ("IpTable.txt");
    if (ipTable.is_open()){
        while ( getline (ipTable,line) ){
            char* str = strdup((char*)line.c_str());
            char** tokens = new char*[10];
            char* token = strtok (str,";");
            int n = 0;
            while (token != NULL){
                tokens[n] = token;
                token = strtok (NULL, ";");
                ++n;
            }
            if(n==3){
                Route route(tokens[0], atoi(tokens[1]), tokens[2]);
                this->ipTable.push_back(route);
            }
        }   
        ipTable.close();
    }
    else {
        cout << "Unable to open file"; 
        exit(EXIT_FAILURE);
    }
    /*for(list<Route>::iterator it=this->ipTable.begin(); it!=this->ipTable.end() ;++it){
        cout << it->getNetwork() << " " << it->getDistance() << " " << it->getThrough() << endl;
    }*/
}

void RouterSimulator::runInterfaceK(){
    Interface interface(&this->ipTable, &this->messagePool, "140.90.0.0", "140.90.0.10", "Bolinchas.kevin", "192.168.122.1", "192.168.122.1", 9876);
    interface.receive();
}

void RouterSimulator::runInterfaceP(){
}

int main(){
    RouterSimulator routerSimulator;
    routerSimulator.runInterfaceK();
}
