#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr, cliaddr2;
    char buffer[MAX_BUFFER_SIZE];

    if (argc != 6) {
        fprintf(stderr, "Usage: %s <source_ip_address> <dest_ip_address_1> <port1> <dest_ip_address_2> <port2> \n", argv[0]);
        exit(1);
    }

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // Clear servaddr structure
    memset(&servaddr, 0, sizeof(servaddr));

    // Assign IP address and port to servaddr structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); // Set IP address from command line argument
    servaddr.sin_port = htons(atoi(argv[3]));

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr(argv[2]); // Set IP address from command line argument
    cliaddr.sin_port = htons(atoi(argv[3]));

    cliaddr2.sin_family = AF_INET;
    cliaddr2.sin_addr.s_addr = inet_addr(argv[4]); // Set IP address from command line argument
    cliaddr2.sin_port = htons(atoi(argv[5]));


    // Bind the socket with the servaddr structure
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    socklen_t len = sizeof(cliaddr);
    socklen_t len2 = sizeof(cliaddr2);


    srand(time(NULL));

    for(int i = 0; i < 100; i++) {
        // Send message back to client
        sprintf(buffer, "%d", i);

        int randomNumber = rand() % 2;

        if(randomNumber == 0) {
            sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            printf("Message sent back to %s\n", inet_ntoa(cliaddr.sin_addr));
        } else {
            sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr2, len2);
            printf("Message sent back to %s\n", inet_ntoa(cliaddr2.sin_addr));
        }

    }

    // Run the server
    while (1) {
        // socklen_t len = sizeof(cliaddr);

        // Receive message from client
        n = recvfrom(sockfd, (char *)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        // Print the received message
        printf("Received message from %s: %s\n", inet_ntoa(cliaddr.sin_addr), buffer);

    }

    // Close the socket
    close(sockfd);
    return 0;
}