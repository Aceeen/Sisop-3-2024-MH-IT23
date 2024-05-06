#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void log_change(char *type, char *message, FILE *logFile)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

    fprintf(logFile, "[%s] [%s] %s\n", dateStr, type, message);
}

void handleRequest(char *buffer, char *response, FILE *file, FILE *logFile)
{
    char line[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    char parameter[BUFFER_SIZE];
    int found = 0;

    sscanf(buffer, "%s %[^\n]", command, parameter);

    if (strcmp(command, "tampilkan") == 0)
    {
        int count = 1;
        rewind(file);
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (line[0] != '\n')
            {
                char *token = strtok(line, ",");
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                char numberedLine[BUFFER_SIZE];
                sprintf(numberedLine, "%d. %s", count, token);
                strcat(response, numberedLine);
                strcat(response, "\n");
                count++;
            }
        }
    }
    else if (strcmp(command, "hari") == 0)
    {
        rewind(file);
        int count = 1;
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (line[0] != '\n')
            {
                char *token = strtok(line, ",");
                if (strcmp(token, parameter) == 0)
                {
                    token = strtok(NULL, ",");
                    token = strtok(NULL, ",");
                    char numberedLine[BUFFER_SIZE];
                    sprintf(numberedLine, "%d. %s", count, token);
                    strcat(response, numberedLine);
                    strcat(response, "\n");
                    found = 1;
                    count++;
                }
            }
        }
        if (!found)
        {
            strcat(response, "Tidak ada anime pada hari tersebut\n");
        }
    }
else if (strcmp(command, "genre") == 0)
{
    rewind(file);
    int count = 1;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] != '\n')
        {
            char *token = strtok(line, ",");
            token = strtok(NULL, ",");
            if (strcmp(token, parameter) == 0)
            {
                token = strtok(NULL, ",");
                char numberedLine[1028];
                sprintf(numberedLine, "%d. %s", count, token);
                strcat(response, numberedLine);
                strcat(response, "\n");
                found = 1;
                count++;
            }
        }
    }
    if (!found)
    {
        strcat(response, "Tidak ada anime dengan genre tersebut\n");
    }
}
else if (strcmp(command, "status") == 0)
{
    rewind(file);
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (line[0] != '\n')
        {
            char *token = strtok(line, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            if (strcmp(token, parameter) == 0)
            {
                token = strtok(NULL, "\n");
                strcat(response, token);
                strcat(response, "\n");
                found = 1;
            }
        }
    }
    if (!found)
    {
        strcat(response, "Tidak ada anime dengan status tersebut\n");
    }
}
else if (strcmp(command, "add") == 0)
{
    rewind(file);
    FILE *temp = fopen("../temp.csv", "w");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        fputs(line, temp);
    }
    fprintf(temp, "\n%s", parameter);
    fclose(temp);
    fclose(file);
    remove("../myanimelist.csv");
    rename("../temp.csv", "../myanimelist.csv");
    file = fopen("../myanimelist.csv", "r");

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

    char *judul = strtok(parameter, ",");
    judul = strtok(NULL, ",");
    judul = strtok(NULL, ",");

    fprintf(logFile, "[%s] [ADD] %s ditambahkan.\n", dateStr, judul);
    strcat(response, "anime berhasil ditambahkan\n");
}
else if (strcmp(command, "edit") == 0)
{
    rewind(file);
    FILE *temp = fopen("../temp.csv", "w");

    char *oldJudul = strtok(parameter, ",");
    char *newLine = strtok(NULL, "\n");

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *oldLine = strdup(line);
        char *token = strtok(line, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        char *judul = token;

        if (strcmp(judul, oldJudul) != 0)
        {
            fputs(oldLine, temp);
        }
        else
        {
            fprintf(temp, "%s\n", newLine);

            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

            fprintf(logFile, "[%s] [EDIT] %s diubah menjadi %s.\n", dateStr, oldLine, newLine);
        }

        free(oldLine);
    }

    fclose(temp);
    fclose(file);
    remove("../myanimelist.csv");
    rename("../temp.csv", "../myanimelist.csv");
    file = fopen("../myanimelist.csv", "r");
    strcat(response, "anime berhasil diedit\n");
}
else if (strcmp(command, "delete") == 0)
{
    rewind(file);
    FILE *temp = fopen("../temp.csv", "w");
    char *judul = parameter;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *oldLine = strdup(line);
        char *judulLine = strtok(line, ",");
        judulLine = strtok(NULL, ",");
        judulLine = strtok(NULL, ",");

        if (strcmp(judulLine, judul) != 0)
        {
            fputs(oldLine, temp);
        }
        else
        {
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

            fprintf(logFile, "[%s] [DEL] %s berhasil dihapus.\n", dateStr, judul);
        }
    }
    fclose(temp);
    fclose(file);
    remove("../myanimelist.csv");
    rename("../temp.csv", "../myanimelist.csv");
    file = fopen("../myanimelist.csv", "r");
    strcat(response, "anime berhasil dihapus\n");
}
else
{
    strcat(response, "Invalid Command\n");
}

fclose(file);
fclose(logFile);
}

int main()
{
    int serverSocket, clientSocket, addrlen;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    FILE *file = fopen("../myanimelist.csv", "r");
    FILE *logFile = fopen("../change.log", "a");

    if (file == NULL || logFile == NULL)
    {
        printf("Error opening files\n");
        return -1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        printf("Error creating socket\n");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("Error binding socket\n");
        return -1;
    }

    if (listen(serverSocket, MAX_CLIENTS) < 0)
    {
        printf("Error listening\n");
        return -1;
    }

    printf("Server started. Listening on port %d\n", PORT);

    while (1)
    {
        addrlen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, (socklen_t *)&addrlen);
        if (clientSocket < 0)
        {
            printf("Error accepting connection\n");
            continue;
        }

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0)
            {
                break;
            }

            printf("Received: %s\n", buffer);
            if (strcmp(buffer, "exit") == 0)
            {
                printf("Closing connection\n");
                close(clientSocket);
                break;
            }

            memset(response, 0, sizeof(response));
            handleRequest(buffer, response, file, logFile);
            send(clientSocket, response, strlen(response), 0);
        }
    }

    fclose(file);
    fclose(logFile);
    close(serverSocket);
    return 0;
}
