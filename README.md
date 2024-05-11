# Sisop-3-2024-MH-IT23

## Anggota Kelompok

| NRP        | Nama                            |
|:----------:|:-------------------------------:|
| 5027231020 | Acintya Edria Sudarsono         |
| 5027231044 | Dionisius Marcell Putra Indranto|
| 5027231072 | Aisyah Rahmasari                |

- [Peraturan](#peraturan) 

## Peraturan
1. Waktu pengerjaan dimulai hari Rabu (1 Mei 2024) setelah sesi lab hingga hari Senin (6 Mei 2024) pukul 23.59 WIB.
2. Praktikan diharapkan membuat laporan penjelasan dan penyelesaian soal dalam bentuk Readme(github).
3. Format nama repository github “Sisop-[Nomor Modul]-2023-[Kode Dosen Kelas]-[Nama Kelompok]” (contoh:Sisop-1-2024-MH-IT01).
4. Struktur repository seperti berikut:
			—soal_1:
				— auth.c
				— rate.c
				— db.c
                                    
—soal_2:
	— dudududu.c
				
			—soal_3:
				— actions.c
				— driver.c
				— paddock.c
				
			—soal_4:
				— client/
					— client.c 
				— server/
					— server.c

<br />
Jika melanggar struktur repo akan dianggap sama dengan curang dan menerima konsekuensi sama dengan melakukan kecurangan. <br />
        1. Setelah pengerjaan selesai, semua script bash, awk, dan file yang berisi cron job ditaruh di github masing - masing kelompok, dan link github diletakkan pada form yang disediakan. Pastikan     
           github di setting ke publik. <br />
        2. Commit terakhir maksimal 10 menit setelah waktu pengerjaan berakhir. Jika melewati maka akan dinilai berdasarkan commit terakhir. <br />
        3. Jika tidak ada pengumuman perubahan soal oleh asisten, maka soal dianggap dapat diselesaikan. <br />
        4. Jika ditemukan soal yang tidak dapat diselesaikan, harap menuliskannya pada Readme beserta permasalahan yang ditemukan. <br />
        5. Praktikan tidak diperbolehkan menanyakan jawaban dari soal yang diberikan kepada asisten maupun praktikan dari kelompok lainnya. <br />
        6. Jika ditemukan indikasi kecurangan dalam bentuk apapun di pengerjaan soal shift, maka nilai dianggap 0. <br />
        7. Pengerjaan soal shift sesuai dengan modul yang telah diajarkan. <br />
        8. Zip dari repository dikirim ke email asisten penguji dengan subjek yang sama dengan nama judul repository, dikirim sebelum deadline dari soal shift <br />
        9. Jika terdapat revisi soal akan dituliskan pada halaman terakhir

### SOAL 1


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

