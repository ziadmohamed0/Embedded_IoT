/**
 * @file client_tcp.c
 * @author Ziad Mohammed Fathy
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
	// 03. send data to server.
	char buffer[1024] = "Hello, I am Ziad Mohammed Fathy\n";
	send(clientSockFD, buffer, strlen(buffer), 0);
 
	// 04. receive data form server. 
	memset(buffer, 0, 1024);
	recv(clientSockFD, buffer, 1024, 0);
	// printf("the recieved data {%s}\n", buffer);
 
	return 0;
} 
 