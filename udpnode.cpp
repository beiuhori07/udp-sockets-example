#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX_BUFFER_SIZE];

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip_address> <port> <number_to_check_modulo>\n", argv[0]);
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
    servaddr.sin_port = htons(atoi(argv[2]));

    // Bind the socket with the servaddr structure
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Run the server
    while (1) {
        socklen_t len = sizeof(cliaddr);

        // Receive message from client
        n = recvfrom(sockfd, (char *)buffer, MAX_BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';

        // Print the received message
        printf("Received message from %s: %s\n", inet_ntoa(cliaddr.sin_addr), buffer);
        printf("Server address:\n");
        printf("  Address family: %d\n", cliaddr.sin_family);
        printf("  Port number: %d\n", ntohs(cliaddr.sin_port));
        printf("  IP address: %s\n", inet_ntoa(cliaddr.sin_addr));

        if(atoi(buffer) % atoi(argv[3]) == 0) {
            // Send message back to client
            sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            printf("Message sent back to %s\n", inet_ntoa(cliaddr.sin_addr));
        }

    }

    // Close the socket
    close(sockfd);
    return 0;
}