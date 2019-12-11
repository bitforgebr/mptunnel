
// Client side implementation of UDP client-server model 
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

static int g_target_port = 0;

// Driver code 
int main(int argc, char** argv) {
	int sockfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

    if (argc <= 1) {
        fprintf(stderr, ("Usage: <%s> <target_port>\n"), argv[0]);
        exit(-1);
    }
    else {
        /// Load the configuration information
        g_target_port = atoi(argv[1]);
    }

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(g_target_port); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	
	int n, len = sizeof(buffer);
	
    while( 1 ) {
        n = sendto(sockfd, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr)); 
        printf("Client sendto %d %d\n", n, errno); 
        printf("Hello message sent.\n"); 
		
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                RECVFROM_FLAGS, (struct sockaddr *) &servaddr, 
                &len); 
        printf("Client recvfrom %d %d\n", n, errno);
        buffer[n] = '\0'; 
        printf("Server : %s\n", buffer); 

        sleep(3);
    }

	close(sockfd); 
	return 0; 
} 
