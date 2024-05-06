#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "actions.c"

#define PORT 8080
#define BUFFER_SIZE 1024

extern char* handleGap(float gap);
extern char* handleFuel(float fuel);
extern char* handleTire(int tireWear);
extern char* handleTireChange(char* tireType);

char* handleCommand(char* command, char* value) {
    char* response;
    if (strcmp(command, "Gap") == 0) {
        float gap = atof(value);
        response = handleGap(gap);
    } else if (strcmp(command, "Fuel") == 0) {
        float fuel = atof(value);
        response = handleFuel(fuel);
    } else if (strcmp(command, "Tire") == 0) {
        int tireWear = atoi(value);
        response = handleTire(tireWear);
    } else if (strcmp(command, "TireChange") == 0) {
        response = handleTireChange(value);
    } else {
        response = "Unknown command";
    }

    return response;
}

void logMessage(char* source, char* command, char* info, char* response) {
    FILE* logFile = fopen("race.log", "a");
    if (logFile == NULL) {
        fprintf(stderr, "Error opening log file\n");
        return;
    }

    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);

    fprintf(logFile, "[%s] [%02d/%02d/%04d %02d:%02d:%02d]: [%s] [%s] -> [%s]\n",
            source, localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
            command, info, response);

    fclose(logFile);
}

int main() {
    pid_t pid, sid;

    // Fork the parent process
    pid = fork();

    // An error occurred
    if (pid < 0) {
        fprintf(stderr, "Error forking process\n");
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // Change the file mode mask
    umask(0);

    // Create a new session
    sid = setsid();
    if (sid < 0) {
        fprintf(stderr, "Error creating new session\n");
    }

    // Change the current working directory
    if ((chdir("/")) < 0) {
        fprintf(stderr, "Error changing working directory\n");
    }

    // Close stdin, stdout, and stderr
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Open a log file in write mode
    int logfd = open("server.log", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (logfd < 0) {
        fprintf(stderr, "Error opening log file\n");
    }

    // Redirect stdout and stderr to the log file
    if (dup2(logfd, STDOUT_FILENO) < 0 || dup2(logfd, STDERR_FILENO) < 0) {
        fprintf(stderr, "Error redirecting output to log file\n");
    }

    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen;
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        fprintf(stderr, "Error creating socket\n");
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        fprintf(stderr, "Error binding socket\n");
    }

    // Listen for incoming connections
    if (listen(serverSocket, 3) < 0) {
        fprintf(stderr, "Error listening\n");
    }

    fprintf(stdout, "Server is running on port %d\n", PORT);

    while (1) {
        // Accept incoming connection
        clientAddrLen = sizeof(clientAddr);
        newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (newSocket < 0) {
            fprintf(stderr, "Error accepting connection\n");
            continue;
        }

        // Receive command and value from client
        memset(buffer, 0, BUFFER_SIZE);
        if (recv(newSocket, buffer, BUFFER_SIZE, 0) <= 0) {
            fprintf(stderr, "Error receiving data from client\n");
            close(newSocket);
            continue;
        }

        char command[32];
        char value[32];
        sscanf(buffer, "%s %s", command, value);

        // Handle command and get response
        char* response = handleCommand(command, value);

        // Log message
        logMessage("Driver", command, value, response);
        logMessage("Paddock", command, value, response);

        // Send response to client
        if (strlen(response) > 0) {
            if (send(newSocket, response, strlen(response), 0) <= 0) {
                fprintf(stderr, "Error sending response to client\n");
            }
        } else {
            if (send(newSocket, "No response", strlen("No response"), 0) <= 0) {
                fprintf(stderr, "Error sending response to client\n");
            }
        }

        // Wait for the client to close the connection
        char closeBuffer[BUFFER_SIZE];
        memset(closeBuffer, 0, BUFFER_SIZE);
        recv(newSocket, closeBuffer, BUFFER_SIZE, 0);

        // Close connection
        close(newSocket);
    }

    close(serverSocket);
    close(logfd);
    return 0;
}
