#include "server.h"

server::server(int port, string ip):server_port(port), server_ip(ip){}

server::~server(){
    for(auto conn:sock_arr){
        close(conn);
    }
    close(server_sockfd);
}

void server::run(){
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(server_port);
    server_sockaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if(bind(server_sockfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(server_sockfd, 20) == -1){
        perror("listen");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    while(1){
        int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
        if(conn < 0){
            perror("connect");
            exit(1);
        }
        cout << "Client: [" << conn << "] connect successful!" << endl;
        sock_arr.push_back(conn);
        thread t(server::RecvMsg, conn);
        t.detach();
    }
}

void server::RecvMsg(int conn){
    char buffer[1000];
    while(1){
        memset(buffer, 0, sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer), 0);
        if(strcmp(buffer, "exit") == 0 || len <= 0){
            break;
        }
        cout << "Recv cli [" << conn << "] msg: " << buffer << endl;
    }
}
