#include <iostream>

#include "networking.hpp"

using namespace std;

int main(){
    Client cl;
    string addr;
    cin >> addr;
    cl.initClient(addr.c_str());
    while(1){
        cin >> cl.buffer;
        cl.sendData();
        if(cl.buffer[0]=='!'&&cl.buffer[1]=='c'){
            break;
        }
        cl.reciveData();
        printf("%s\n", cl.buffer);
        if(cl.buffer[0]=='!'&&cl.buffer[1]=='c'){
            break;
        }
    } 
    cl.closeClient();
    return 0;
}