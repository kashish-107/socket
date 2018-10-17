// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT    22319  
#define AWS_PORT    20319  
#define MAXLINE 1024 

char *hello = "Message from backend server B"; 

int main() { 
	int sockfd;  //Server socket file descriptor
	char buffer[MAXLINE]; 
	struct sockaddr_in servaddr, cliaddr, aws_servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&servaddr, 0, sizeof(aws_servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 

	// Filling current server information 
	servaddr.sin_family    = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 

	// Filling aws server information 
	aws_servaddr.sin_family    = AF_INET; // IPv4 
	aws_servaddr.sin_addr.s_addr = INADDR_ANY; 
	aws_servaddr.sin_port = htons(AWS_PORT); 

	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 

	int len, n; 
	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0'; 
	printf("Message received from AWS: %s\n", buffer); 

    	sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &aws_servaddr, sizeof(aws_servaddr));
	printf("Message sent to AWS: %s\n", hello); 
	
	close(sockfd);
	return 0; 
} 
