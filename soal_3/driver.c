#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    char info[BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error connecting to server\n");
        return 1;
    }

    while (1) {
        printf("Enter command (or 'exit' to quit): ");
        fgets(command, BUFFER_SIZE, stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline character

        if (strcmp(command, "exit") == 0) {
            break;
        }

        printf("Enter info: ");
        fgets(info, BUFFER_SIZE, stdin);
        info[strcspn(info, "\n")] = '\0'; // Remove newline character

        // Send command and info to server
        int bytesWritten = snprintf(buffer, BUFFER_SIZE, "%s %s", command, info);
        if (bytesWritten < 0 || bytesWritten >= BUFFER_SIZE) {
            fprintf(stderr, "Buffer overflow detected. Command and info strings are too long.\n");
            break;
        }

        if (send(clientSocket, buffer, strlen(buffer), 0) <= 0) {
            printf("Error sending data to server\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(clientSocket, buffer, BUFFER_SIZE, 0) <= 0) {
            printf("Error receiving response from server\n");
            break;
        }

        printf("Response: %s\n", buffer);
    }

    // Close socket
    close(clientSocket);
    return 0;
}
