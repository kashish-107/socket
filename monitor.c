#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 26319 
   
int main(int argc, char const *argv[]) { 
    int sockfd = 0; /* Socket descriptor */
    int valread; 
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0};

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { /* TCP Socket creation */ 
        printf("\n TCP Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; //IP4
    serv_addr.sin_port = htons(PORT); 
       
    /* Convert IPv4 address from text to binary form */
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { /* Connecting to server */
        printf("\nConnection Failed \n"); 
        return -1; 
    }

    printf("The Monitor is up and running\n");
    valread = read(sockfd, buffer, 1024); 
    printf("Message received from AWS: %s\n", buffer); 
    return 0; 
} 
