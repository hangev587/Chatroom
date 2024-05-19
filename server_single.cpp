#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;

int main(){
    // 定义 sockfd
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(8023);
    server_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind, success=0, error=-1
    if(bind(server_sockfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    // listen, success=0, error=-1
    if(listen(server_sockfd, 20) == -1){
        perror("listen");
        exit(1);
    }

    // 客户端套接字
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    // accept, success>0, error=-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0){
        perror("connect");
        exit(1);
    }
    cout << "Success Linked to Client!\n" << endl;

    char buffer[1000];

    while(1){
        memset(buffer, 0, sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer), 0);
        //
        if(strcmp(buffer, "exit") == 0 || len <= 0){
            break;
        }
        else{
            cout << "Receive Cli Msg: " << buffer << endl;
        }
    }
    close(conn);
    close(server_sockfd);

    return 0;
}
