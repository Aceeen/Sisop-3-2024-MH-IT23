# Sisop-3-2024-MH-IT23


### SOAL 2
Membuat kalkulator sederhana dengan menerapkan konsep pipes dan fork <br />

Fungsi untuk mengkonversi integer ke string
```
void convertToWords(int num, char *result) {
   
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan", "sepuluh", "sebelas", "dua belas", "tiga belas", "empat belas", "lima belas", "enam belas", "tujuh belas", "delapan belas", "sembilan belas"};
    char *tens[] = {"", "", "dua puluh", "tiga puluh", "empat puluh", "lima puluh", "enam puluh", "tujuh puluh", "delapan puluh", "sembilan puluh"};

    if (num < 20) {
        strcpy(result, ones[num]);
    } else {
        sprintf(result, "%s %s", tens[num / 10], ones[num % 10]);
    }
}
```
<br /> <br />
Fungsi untuk mencatat histori perhitungan ke dalam file log dengan format sesuai instruksi soal, date, type, dan message.
```
void writeLog(char *type, char *input1, char *operation, char *input2, int result) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%d/%m/%y %H:%M:%S", timeinfo);

    char result_words[100];
    convertToWords(result, result_words);

    FILE *logfile = fopen("histori.log", "a");
    if (logfile != NULL) {
        fprintf(logfile, "[%s] [%s] %s %s %s sama dengan %s.\n", timestamp, type, input1, operation, input2, result_words);
        fclose(logfile);
    }
}
```
<br /> <br />
Fungsi main untuk mengambil input string dari user. Lalu string akan dikonversikan ke angka
```
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Penggunaan: %s <operasi>\n", argv[0]);
        printf("Operasi:\n");
        printf("-kali : perkalian\n");
        printf("-tambah : penambahan\n");
        printf("-kurang : pengurangan\n");
        printf("-bagi : pembagian\n");
        return 1;
    }


    char input1[20], input2[20];
    printf("Masukkan dua angka dalam kata (contoh: tiga tujuh): ");
    scanf("%s %s", input1, input2);

 
    int num1, num2;
    if (strcmp(input1, "satu") == 0) num1 = 1;
    else if (strcmp(input1, "dua") == 0) num1 = 2;
    else if (strcmp(input1, "tiga") == 0) num1 = 3;
    else if (strcmp(input1, "empat") == 0) num1 = 4;
    else if (strcmp(input1, "lima") == 0) num1 = 5;
    else if (strcmp(input1, "enam") == 0) num1 = 6;
    else if (strcmp(input1, "tujuh") == 0) num1 = 7;
    else if (strcmp(input1, "delapan") == 0) num1 = 8;
    else if (strcmp(input1, "sembilan") == 0) num1 = 9;
    else {
        printf("Input tidak valid.\n");
        return 1;
    }

    if (strcmp(input2, "satu") == 0) num2 = 1;
    else if (strcmp(input2, "dua") == 0) num2 = 2;
    else if (strcmp(input2, "tiga") == 0) num2 = 3;
    else if (strcmp(input2, "empat") == 0) num2 = 4;
    else if (strcmp(input2, "lima") == 0) num2 = 5;
    else if (strcmp(input2, "enam") == 0) num2 = 6;
    else if (strcmp(input2, "tujuh") == 0) num2 = 7;
    else if (strcmp(input2, "delapan") == 0) num2 = 8;
    else if (strcmp(input2, "sembilan") == 0) num2 = 9;
    else {
        printf("Input tidak valid.\n");
        return 1;
    }

    int result;
```
<br /> <br />
Untuk membuat 2 pipe
```
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }


    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
```
<br /> <br />
Kemudian membuat child process menggunakan fork() yang akan execute operasi aritmatika sesuai dengan argumen yang diinput user lalu disimpan ke dalam pipe f. 
```
    if (pid == 0) { 
        close(fd[0]); 

       
        if (strcmp(argv[1], "-kali") == 0) { 
            result = num1 * num2;
            writeLog("KALI", input1, "kali", input2, result);
        } else if (strcmp(argv[1], "-tambah") == 0) { 
            result = num1 + num2;
            writeLog("TAMBAH", input1, "tambah", input2, result);
        } else if (strcmp(argv[1], "-kurang") == 0) { 
            result = num1 - num2;
            if (result < 0) {
                printf("ERROR\n");
                return 1;
            }
            writeLog("KURANG", input1, "kurang", input2, result);
        } else if (strcmp(argv[1], "-bagi") == 0) { 
            if (num2 == 0) {
                printf("Pembagian oleh nol tidak diizinkan.\n");
                return 1;
            }
            result = num1 / num2;
            writeLog("BAGI", input1, "bagi", input2, result);
        } else {
            printf("Operasi tidak valid.\n");
            return 1;
        }

        write(fd[1], &result, sizeof(result));
        close(fd[1]); 
        return 0;
```
<br /> <br />
Lalu parent process akan membaca pipe
```
    } else { 
        close(fd[1]); 

        
        read(fd[0], &result, sizeof(result));
        close(fd[0]); 
        char result_words[100];
        convertToWords(result, result_words);
        printf("Hasil %s %s dan %s adalah %s.\n", argv[1]+1, input1, input2, result_words);

      
        wait(NULL);

        return 0;
    }
}
```
Untuk parent process bakalan baca hasil dari pipe, cetak hasil operasi dalam bentuk string, sama menunggu child process selesai baru parent process berhenti berjalan.
Jadi child process akan mengambil argumen dari user untuk operasi aritmatika lalu hasilnya dikirim ke pipe. Child process juga akan menulis log ke file histori.log dengan memanggil fungsi writeLog. Lalu parent process akan membaca hasil dari pipe, cetak hasil operasi dalam bentuk string, dan menunggu child process selesai berjalan baru kemudian parent process akan selesai berjalan.
## Soal 3
### actions.c
```
char* handleGap(float gap) {
    if (gap < 3.5) {
        return "Gogogo";
    } else if (gap >= 3.5 && gap <= 10.0) {
        return "Push";
    } else {
        return "Stay out of trouble";
    }
}

char* handleFuel(float fuel) {
    if (fuel > 80.0) {
        return "Push Push Push";
    } else if (fuel >= 50.0 && fuel <= 80.0) {
        return "You can go";
    } else {
        return "Conserve Fuel";
    }
}

char* handleTire(int tireWear) {
    if (tireWear > 80) {
        return "Go Push Go Push";
    } else if (tireWear >= 50 && tireWear <= 80) {
        return "Good Tire Wear";
    } else if (tireWear >= 30 && tireWear < 50) {
        return "Conserve Your Tire";
    } else {
        return "Box Box Box";
    }
}

char* handleTireChange(char* tireType) {
    if (strcmp(tireType, "Soft") == 0) {
        return "Mediums Ready";
    } else if (strcmp(tireType, "Medium") == 0) {
        return "Box for Softs";
    } else {
        return "Invalid Tire Type";
    }
}
```
`actions.c` berisi aksi-aksi apa saja yang akan keluar apabila kita memasukkan command serta informasi mengenai beberapa bidang dimana pada `actions.c` ini ada Fuel, Tire, Gap dan Tire change
### paddock.c
```
#include "actions.c"
extern char* handleGap(float gap);
extern char* handleFuel(float fuel);
extern char* handleTire(int tireWear);
extern char* handleTireChange(char* tireType);
```
Deklarasikan `actions.c` agar constraints yang telah kita buat dapat terhubung dengan `paddock.c` lalu gunakan extern juga agar dapat mengeluarkan response dari setiap constraints yang kita buat
```
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
```
Lalu, kita membuat fungsi untuk menghandle command serta informasi yang kita akan masukkan pada program lalu response yang dikeluarkan akan seperti yang ada pada `actions.c`
```
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
```
Lalu kita membuat fungsi log message agar semua command yang kita masukkan ke program akan tercatat pada `race.log` dengan format `[Source] [DD/MM/YY hh:mm:ss]: [Command] [Additional-info]`
```
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
```
Lalu pada fungsi main kita membuat program ini menjadi daemon serta dapat terhubung dengan `driver.c` melalui socket RPC serta memanggil fungsi `handleCommand` serta `logMessage`
### driver.c
```
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
```
Lalu kita membuat `driver.c` dimana kita menghubungkan program ini dengan `paddock.c` agar dapat mengirim pesan command yang akan kita masukkan serta kita juga menggunakan in-program CLI untuk memasukkan informasi
![image](https://github.com/v0rein/Sisop-3-2024-MH-IT23/assets/143814923/8b33d2c5-5b61-462b-82cc-d8f1b28496a4)
### Revisi
- Ada error apabila kita melakukan command `Tire change`
  Hal ini saya solve dengan mengganti command yang perlu dimasukkan menjadi `TireChange`
![image](https://github.com/v0rein/Sisop-3-2024-MH-IT23/assets/143814923/e8bcdb4a-e791-4281-88c2-a46dc76e3872)

- Ada error pada pencatatan command pada `race.log`

## SOAL 4
### client.c
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create the client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server IP address if needed

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error connecting to server\n");
        return -1;
    }

    printf("Connected to server\n");

    while (1) {
        printf("Enter command (Gap, Fuel, Tire, TireChange, exit): ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin); 
        buffer[strcspn(buffer, "\n")] = '\0';

        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
           exit(0);
        }

        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0) {
            printf("Error receiving data\n");
            break;
        }

        printf("Server response: %s\n", buffer);
    }

    close(clientSocket);
    return 0;
}
```
Kita membuat `client.c` dimana program ini akan membuat socket yang nantinya akan terhubung dengan `server.c` serta sebagai pengirim pesan kepada `server.c`
### server.c
```
void log_change(char *type, char *message, FILE *logFile)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

    fprintf(logFile, "[%s] [%s] %s\n", dateStr, type, message);
}
```
Pertama kita membuat fungsi `log_change` dimana ini akan mencatat seluruh command yang kita masukkan ke `change.log`
```
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
```
Lalu kita membuat fungsi untuk menghandle beberapa request command seperti `add`, `edit`, `del` lalu mengupdate file csv yang telah kita download serta mencatat perubahan tersebut pada `change.log`
```
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
```
Lalu pada fungsi main kita membuat inisiasi socket agar terhubung dengan `client.c` sebagai penerima pesan
![image](https://github.com/v0rein/Sisop-3-2024-MH-IT23/assets/143814923/7c738a7a-3c53-49e3-98da-49fe02ca3e8c)
### Revisi
-Pencatatan pada `change.log`
