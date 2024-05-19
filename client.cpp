#include "client.h"

client::client(int port, string ip):server_port(port), server_ip(ip){}

client::~client(){
    close(sock);
}

void client::run(){
    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        exit(1);
    }
    cout << "Success connect to server!" << endl;

    thread send_t(SendMsg, sock), recv_t(RecvMsg, sock);
    send_t.join();
    cout << "send thread finish." << endl;
    recv_t.join();
    cout << "recv thread finish." << endl;
    return;
}

void client::SendMsg(int conn){
    char sendbuf[100];
    while(1){
        memset(sendbuf, 0, sizeof(sendbuf));
        cin >> sendbuf;
        int ret = send(conn, sendbuf, strlen(sendbuf), 0);
        if(strcmp(sendbuf, "exit") == 0 || ret <= 0){
            break;
        }
    }
}

void client::RecvMsg(int conn){
    char recvbuf[1000];
    while(1){
        memset(recvbuf, 0, sizeof(recvbuf));
        int len = recv(conn, recvbuf, sizeof(recvbuf), 0);
        if(len <= 0){
            break;
        }
        else{
            cout << "Recv ser msg: " << recvbuf << endl;
        }
    }
}
