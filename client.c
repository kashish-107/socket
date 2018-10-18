// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 25319 
   
int main(int argc, char const *argv[]) 
{ 
    int sockfd = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0};
    char buf[1024];

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { //TCP socket created 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {//Connecting to server
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    printf("The client is up and running\n");
    printf("The client sent link ID=%s,size=%s, and power=%s to AWS\n",argv[1], argv[2], argv[3]);
    snprintf(buf, sizeof(buf), "%s;%s;%s", argv[1], argv[2], argv[3]); 
    send(sockfd, buf, strlen(buf), 0); 
    printf("Message sent to AWS: %s\n", buf); 
    valread = read(sockfd, buffer, 1024); 
    printf("Message received from AWS: %s\n", buffer); 
    return 0; 
} 
