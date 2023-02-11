#include <iostream>

#include <cstring>

#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>

#include <arpa/inet.h>

#define PORT 8080

class Server{
    public:
    int server_fd;
    int new_socket[1000];
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };

    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&address;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    void initServer(){
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        char str[INET_ADDRSTRLEN];
        inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
        std::cout << "ip:" << str << std::endl;
    }
    void waitforconnect(int cnt){
        for(int i = 0; i!= cnt; i++){
            if (listen(server_fd, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }
            if ((new_socket[i] = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }
    }
    void reciveData(int cnt){
        read(new_socket[cnt], buffer, 1024);
    }
    void sendData(int cnt){
        send(new_socket[cnt], buffer, 1024, 0);
    }
    void closeServer(){
        for(int i = 0; i!= 1000; i++){
            close(new_socket[i]);
        }
        shutdown(server_fd, SHUT_RDWR);
    }
};

class Client{
    public:
    int sock = 0, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    void initClient(const char* ip){
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(-1);
        }
    
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        if (inet_pton(AF_INET, ip, &serv_addr.sin_addr)
            <= 0) {
            printf(
                "\nInvalid address/ Address not supported \n");
            exit(-1);
        }
    
        if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
            < 0) {
            printf("\nConnection Failed \n");
            exit(-1);
        }
    }
    void reciveData(){
        read(sock, buffer, 1024);
    }
    void sendData(){
        send(sock, buffer, 1024, 0);
    }
    void closeClient(){
        close(client_fd);
    }
};