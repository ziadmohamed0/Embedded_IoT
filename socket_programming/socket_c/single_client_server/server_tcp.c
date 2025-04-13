/**
 * @file server_tcp.c
 * @author Ziad mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-12
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
  
int main (int argc, int *argv[]) {
      
    // 01. create a socket for the server.
    int serverSockFD = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSockFD != 0) {
        printf("Client Socket IP: %d\n", serverSockFD);
    }
      
    else {
        printf("Error :( - Faild to create a socket\n");
        exit(1);
    }

    // 02. bind this socket to a specific port number
    struct sockaddr_in serverAdress;
    memset(&serverAdress, 0, sizeof(serverAdress));

    serverAdress.sin_family = AF_INET;
    serverAdress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAdress.sin_port = htons(4444);

    if(bind(serverSockFD, (struct sockaddr*)&serverAdress, sizeof(serverAdress))== 0) {
        printf("server is binding to port no. 4444\n");
    }

    else {
        printf("binding faild :(\n");
        exit(1);
    }

    // 03. listen to clients connection requests.
    if(listen(serverSockFD, 1) == 0) {
        printf("listen .... \n");
    }

    else {
        printf("faild to listen :(\n");
        exit(1);
    }

    // 04 accept the connection request.
    struct sockaddr_in connectionClientAddress;
    memset(&connectionClientAddress, 0, sizeof(connectionClientAddress));

    int clientAddLength = 0;
    int connectionServerSockFD = accept(serverSockFD, (struct sockaddr*)&connectionClientAddress, &clientAddLength);
    if(connectionServerSockFD == -1) {
        printf("faild to accept the connection request :(\n");
        exit(1);
    }
    else {
        printf("accept the connection request socket ID : %d\n", connectionServerSockFD);
    }

    // 05. send or receive data from the client
    char buffer[1024];
    recv(connectionServerSockFD, buffer, 1024, 0);
    printf("received data : %s\n", buffer);
    // 06 close socket.
    close(serverSockFD);
    close(connectionServerSockFD);
	printf("client socket has been closed\n");
}