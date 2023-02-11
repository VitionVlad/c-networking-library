#include <iostream>

#include "networking.hpp"

#define PORT 8080

using namespace std;

int main(){
    Server serv;
    serv.initServer();
    serv.waitforconnect(1);
    while(1){
        serv.reciveData(0);
        printf("%s\n", serv.buffer);
        if(serv.buffer[0]=='!'&&serv.buffer[1]=='c'){
            break;
        }
        cin >> serv.buffer;
        serv.sendData(0);
        if(serv.buffer[0]=='!'&&serv.buffer[1]=='c'){
            break;
        }
    }
    serv.closeServer();
    return 0;
}