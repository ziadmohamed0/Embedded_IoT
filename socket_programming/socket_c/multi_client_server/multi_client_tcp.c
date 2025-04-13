/**
 * @file multi_client_tcp.c
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
#include <sys/wait.h>

#define BUFFER_SIZE  50

void write_routine(int socket, char* buf);
void read_routine(int socket, char* buf);

int main (int argc, int *argv[]) {
    // 01. create a socket for the client.
	int clientSockFD = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSockFD != 0) {
		printf("Client Socket IP: %d\n", clientSockFD);
	}
	 
	else {
		printf("Error :( - Faild to create a socket\n");
		exit(1);
	}
    
    // 02. connect the client to a specific server.
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	 
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("192.168.100.25");
	serverAddress.sin_port = htons(4444);
	 
	if(connect(clientSockFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == 0) {
		printf("connected successfuly\n");
	}
	else {
	 	printf("faild to connected\n");
	 	exit(1);
	}

    char buffer[BUFFER_SIZE];
    pid_t PID = fork();

    if(PID == 0) {
        write_routine(clientSockFD, buffer);
    }
    else{
        read_routine(clientSockFD, buffer);
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param socket 
 * @param buf 
 */
void write_routine(int socket, char* buf) {
    scanf("%s", buf);
    send(socket, buf, BUFFER_SIZE, 0);
    if(*buf == 'Q' || *buf == 'q') {
        close(socket);
        printf("client is disconnected\n");
        return;
    }
    memset(buf, 0, BUFFER_SIZE);
}

/**
 * @brief 
 * 
 * @param socket 
 * @param buf 
 */
void read_routine(int socket, char* buf) {
    while (1) {
        recv(socket, buf, BUFFER_SIZE, 0);
        if(strlen(buf) <= 0) {
            close(socket);
            return;
        }
        printf("message from the receiver: %s\n", buf);
        memset(buf, 0, BUFFER_SIZE);
    }
}