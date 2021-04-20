#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

int main() {
	char hostname[200];
	char ip[100];
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	
	printf("Enter host name: ");
	scanf("%s", hostname);
	if((he = gethostbyname(hostname)) == NULL) {
		printf("Error number: %d\n", h_errno);
		herror("Error getting host");
		return 1;
	}
	
	addr_list = (struct in_addr **) he->h_addr_list;
	for(i = 0; addr_list[i] != NULL; ++i) {
		strcpy(ip, inet_ntoa(*addr_list[i]));
	}	
	
	printf("%s resolved to: %s\n", hostname, ip);
	
	return 0;
}
