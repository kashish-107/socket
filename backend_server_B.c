// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "csvparser.h"
#define PORT    21319  
#define AWS_PORT    20319  
#define MAXLINE 1024 

int main() { 
	int sockfd;  //Server socket file descriptor
	char buffer[MAXLINE], linkid[MAXLINE]; 
	struct sockaddr_in servaddr, cliaddr, aws_servaddr; 
	int n, len, ret;
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

	printf("The Server A is up and running using UDP on port %d", PORT);

	n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0'; 

	sscanf(buffer, "%s", linkid);	
	printf("The Server A received input %s\n",linkid);

	ret = csv_parser("database_a.csv", linkid, buffer);
	printf("The server A has found %d match", ret);

    	sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &aws_servaddr, sizeof(aws_servaddr));
	printf("The Server A finished sending the output to AWS"); 
	
	close(sockfd);
	return 0; 
} 
