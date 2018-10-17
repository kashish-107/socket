// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT    20319  
#define TCP_PORT 25319 
#define SERVER_A_PORT    21319  
#define SERVER_C_PORT    23319  
#define MAXLINE 1024 

int main() { 
	int udpsockfd;  //Udp server socket file descriptor
	int tcpsockfd, new_socket, valread;  // Tcp server socket file descriptor
	char buffer[MAXLINE]; 
	int opt = 1;
	int len, n; 
	struct sockaddr_in udpservaddr, cliaddr, udpservaddrA, udpservaddrC, tcpservaddr; 
	int tcp_addrlen = sizeof(tcpservaddr);

	// Creating socket file descriptor 
	if ( (udpsockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	if ( (tcpsockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

        if (setsockopt(tcpsockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	perror("setsockopt");
        	exit(EXIT_FAILURE);
    	}

	memset(&udpservaddr, 0, sizeof(udpservaddr)); 
	memset(&udpservaddrA, 0, sizeof(udpservaddrA)); 
	memset(&udpservaddrC, 0, sizeof(udpservaddrC)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 

	// Filling current server information 
	udpservaddr.sin_family    = AF_INET; // IPv4 
	udpservaddr.sin_addr.s_addr = INADDR_ANY; 
	udpservaddr.sin_port = htons(PORT); 

	// Filling backend server A information 
	udpservaddrA.sin_family    = AF_INET; // IPv4 
	udpservaddrA.sin_addr.s_addr = INADDR_ANY; 
	udpservaddrA.sin_port = htons(SERVER_A_PORT); 

	// Filling compute backend server C information 
	udpservaddrC.sin_family    = AF_INET; // IPv4 
	udpservaddrC.sin_addr.s_addr = INADDR_ANY; 
	udpservaddrC.sin_port = htons(SERVER_C_PORT); 

	// Filling tcp server  information 
	tcpservaddr.sin_family    = AF_INET; // IPv4 
	tcpservaddr.sin_addr.s_addr = INADDR_ANY; 
	tcpservaddr.sin_port = htons(TCP_PORT);
 
	// Bind the socket with the server address 
	if ( bind(udpsockfd, (const struct sockaddr *)&udpservaddr, sizeof(udpservaddr)) < 0 ) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
 	// Forcefully attaching socket to the port 8080
    	if (bind(tcpsockfd, (struct sockaddr *)&tcpservaddr, sizeof(tcpservaddr)) < 0) {
        	perror("bind failed");
        	exit(EXIT_FAILURE);
    	}

	if (listen(tcpsockfd, 3) < 0) {
        	perror("listen");
        	exit(EXIT_FAILURE);
    	}
	
	if ((new_socket = accept(tcpsockfd, (struct sockaddr *)&tcpservaddr, (socklen_t*)&tcp_addrlen)) < 0) {
        	perror("accept");
        	exit(EXIT_FAILURE);
    	}
	
	valread = read(new_socket, buffer, 1024);
	printf("Message received from client: %s\n", buffer);

	sendto(udpsockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &udpservaddrA, sizeof(udpservaddrA));
        printf("Client message sent to UDP backend server A: %s\n", buffer);

	n = recvfrom(udpsockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0'; 
	printf("Message received from UPD backend server A: %s\n", buffer);

	sendto(udpsockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *) &udpservaddrC, sizeof(udpservaddrC));
        printf("AWS message sent to compute UDP backend server C: %s\n", buffer);

	n = recvfrom(udpsockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len); 
	buffer[n] = '\0'; 
	printf("Message received from compute UPD backend server C: %s\n", buffer);

	send(new_socket , buffer, strlen(buffer), 0);
	printf("Backend server message is sent back to client: %s\n", buffer);

	close(udpsockfd);
	return 0; 
} 
