#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>


void convertToWords(int num, char *result) {
   
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan", "sepuluh", "sebelas", "dua belas", "tiga belas", "empat belas", "lima belas", "enam belas", "tujuh belas", "delapan belas", "sembilan belas"};
    char *tens[] = {"", "", "dua puluh", "tiga puluh", "empat puluh", "lima puluh", "enam puluh", "tujuh puluh", "delapan puluh", "sembilan puluh"};

    if (num < 20) {
        strcpy(result, ones[num]);
    } else {
        sprintf(result, "%s %s", tens[num / 10], ones[num % 10]);
    }
}


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