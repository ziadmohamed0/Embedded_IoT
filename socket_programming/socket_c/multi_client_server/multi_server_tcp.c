/**
 * @file multi_server_tcp.c
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

#define FALSE   0
#define TRUE    1
#define BUFFER_SIZE  50


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

    // 02. set socket for server.
    int option = TRUE;
    setsockopt(serverSockFD, SOL_SOCKET, SO_REUSEADDR, (void*)&option, sizeof(option));

    // 03. bind this socket to a specific port number
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

    // 04. listen to clients connection requests.
    if(listen(serverSockFD, 1) == 0) {
        printf("listen .... \n");
    }

    else {
        printf("faild to listen :(\n");
        exit(1);
    }

    // 05 accept the connection request.
    struct sockaddr_in connectionClientAddress;
    int clientAddLength = 0;

    while(1) {
        memset(&connectionClientAddress, 0, sizeof(connectionClientAddress));
        int connectionServerSockFD = accept(serverSockFD, (struct sockaddr*)&connectionClientAddress, &clientAddLength);
        if(connectionServerSockFD == -1) {
            printf("faild to accept the connection request :(\n");
            continue;
        }
        else {
            printf("accept the connection request socket ID : %d\n", connectionServerSockFD);
        }

        pid_t PID = fork();
        if(PID == -1) {
            close(connectionServerSockFD);
            continue;
        }
        if(PID == 0) {
            close(serverSockFD);
            char buffer[BUFFER_SIZE];
            while (1) {
            	memset(buffer, 0, BUFFER_SIZE);
                recv(connectionServerSockFD, buffer, BUFFER_SIZE, 0);
                printf("received data : %s\n", buffer);
                if(buffer[0] == 'Q' || buffer[0] == 'q') {
                    printf("client ID %d:%d is disconnected\n", connectionServerSockFD, PID);
                    close(connectionServerSockFD);
                    exit(15);
                }
                else {
                    send(connectionServerSockFD, buffer, strlen(buffer), 0);
                }
            }
            
        }
        else{
            close(connectionServerSockFD);
        }
    }


    return 0;
}