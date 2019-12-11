
// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MSG_CONFIRM	0x800	/* Confirm path validity */

#define MAXLINE 1024 

#ifdef __linux__ 
#define RECVFROM_FLAGS ( MSG_WAITALL )
#else
#define RECVFROM_FLAGS ( 0 )
#endif

static int g_listen_port = 0;

// Driver code 
int main(int argc, char** argv) {
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in servaddr, cliaddr; 
	
    if (argc <= 1) {
        fprintf(stderr, ("Usage: <%s> <listen_port>\n"), argv[0]);
        exit(-1);
    }
    else {
        /// Load the configuration information
        g_listen_port = atoi(argv[1]);
    }

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = inet_addr("0.0.0.0"); 
	servaddr.sin_port = htons(g_listen_port); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, 
			sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len = sizeof(buffer), n; 

    while( 1 ) {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    RECVFROM_FLAGS, ( struct sockaddr *) &cliaddr, 
                    &len); 
        printf("Server recvfrom %d %d\n", n, errno);
        buffer[n] = '\0'; 
        printf("Client : %s\n", buffer); 
        n = sendto(sockfd, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
                len); 
        printf("Server sendto %d %d\n", n, errno); 
        printf("Hello message sent.\n"); 

        sleep(3);
    }
	
	return 0; 
} 

