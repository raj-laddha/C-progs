#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MSG_SIZE 512

int main() {
	int socketfd, new_socket;
	char message[MSG_SIZE];
	struct sockaddr_in server, client;
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(socketfd == -1) {
		printf("Error number: %d\n", errno);
		perror("Error in creating socket");
		return 1;
	}
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(6013);
	
	printf("Binding...\n");
	if(bind(socketfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
		printf("Error number: %d\n", errno);
		perror("Error in binding");
		return 2;
	}
	printf("Bind done\n");
	
	if(listen(socketfd, 3) == -1) {
		printf("Error number: %d\n", errno);
		perror("Error while listening");
		return 3;
	}
	printf("\nListening...\n");
	
	int c = sizeof(struct sockaddr_in);
	new_socket = accept(socketfd, (struct sockaddr *)&client, (socklen_t*)&c); 
	if(new_socket == -1) {
		printf("Error number: %d\n", errno);
		perror("Accept failed");
		return 4;
	}
	
	printf("Connection accepted\n");
	printf("Enter message: ");
	scanf("%[^\n]%*c", message);
	int end = strlen(message);
	message[end++] = '\n';
	message[end] = '\0';
	write(new_socket, message, strlen(message));
	
	return 0;
}
