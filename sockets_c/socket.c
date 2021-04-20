#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define REPLY_SIZE 2048

int main() {
	int socketfd;
	struct sockaddr_in server;
	char *message;
	char server_reply[REPLY_SIZE];
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1) {
		printf("Error number: %d\n", errno);
		perror("Error in creating socket");
		return 1;
	}
	
	server.sin_addr.s_addr = inet_addr("117.18.237.42");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	
	printf("Connecting...\n");
	if(connect(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		printf("Error number: %d\n", errno);
		perror("Connect error");
		return 2;
	}
	
	printf("Connected\n");
	
	message = "GET/ HTTP/1.1\r\n\r\n";
	printf("\nSending request...\n");
	if(send(socketfd, message, strlen(message), 0) == -1) {
		printf("Error number: %d\n", errno);
		perror("Error sending request");
		return 3;
	}
	
	printf("Data sent\n");
	
	printf("\nReceiving reply...\n");
	if(recv(socketfd, server_reply, REPLY_SIZE, 0) == -1) {
		printf("Error number: %d\n", errno);
		perror("Error receiving reply");
		return 4;
	}
	
	printf("Reply received\n");
	printf("%s\n", server_reply);
	
	if(close(socketfd) == -1) {
		printf("Error number: %d\n", errno);
		perror("Error while closing");
		return 5;
	}
	
	printf("\nClosed\n");
	return 0;
}
