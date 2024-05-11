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

Pada soal di minta untuk membuat :
1. Auth.c:
- Memeriksa apakah file yang masuk ke folder new-data adalah file CSV.
- Memeriksa apakah nama file berakhiran "trashcan" atau "parkinglot".
- Jika file lolos autentikasi, file tersebut akan dipindahkan ke shared memory.

2. Db.c:
- Memindahkan file yang lolos autentikasi dari new-data ke folder microservices/database menggunakan shared memory.
- Mencatat semua file yang masuk ke dalam folder microservices/database ke dalam file db.log.

3. Rate.c:
- Mengambil data CSV dari shared memory.
- Menampilkan output Tempat Sampah dan Parkiran dengan rating terbaik dari data tersebut.

4. Logging:
- Melakukan logging setiap kali sebuah file masuk ke dalam folder microservices/database. Log ini harus mencatat waktu masuknya file beserta jenisnya (Trash Can atau Parking Lot) dan nama filenya.

#### SOAL

#### AUTH.C 
Hasil dari kode ini adalah memindahkan file yang sesuai dengan kriteria (berakhiran "parkinglot.csv" atau "trashcan.csv") dari direktori "new-data" ke dalam shared memory. File-file yang tidak sesuai akan dihapus dari direktori "new-data". Saat file berhasil dipindahkan, program akan menampilkan pesan yang menyatakan bahwa file tersebut berhasil disimpan di shared memory.

1. cek_file(const char *filename):
- Fungsi ini bertujuan untuk memeriksa apakah nama file sesuai dengan ketentuan yang diminta dalam soal.
- Parameter filename adalah nama file yang akan diperiksa.
- Fungsi ini mengembalikan nilai 1 jika nama file berakhir dengan "parkinglot.csv" atau "trashcan.csv", dan mengembalikan nilai 0 jika tidak sesuai.

```sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 512
#define MAX_FILE_CONTENT_LENGTH 1024
#define MAX_FILES 10
#define SHARED_MEMORY_KEY 4321
#define SHARED_MEMORY_SIZE (sizeof(FileInfo) * MAX_FILES)

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char content[MAX_FILE_CONTENT_LENGTH];
} FileInfo;

// Fungsi untuk mengecek apakah nama file sesuai dengan ketentuan atau tidak 
int cek_file(const char *filename) {
    // Cek apakah nama file berakhiran dengan "parkinglot.csv" atau "trashcan.csv"
    if (strstr(filename, "parkinglot.csv") || strstr(filename, "trashcan.csv")) {
        return 1; 
    } else {
        return 0; // File selain csv tidak valid
    }
}
```

2. file_lolos():
- Fungsi ini bertanggung jawab untuk memindahkan file yang lolos autentikasi ke dalam shared memory.
- Fungsi ini membuka direktori "new-data" untuk membaca file-file yang ada di dalamnya.
- Selanjutnya, fungsi membuat shared memory dengan ukuran yang cukup untuk menyimpan data dari beberapa file.
- Fungsi kemudian menghubungkan shared memory yang telah dibuat ke ruang alamat proses menggunakan shmat.
- Setelah itu, fungsi membaca setiap file di dalam direktori "new-data", memeriksa apakah sesuai dengan kriteria menggunakan fungsi cek_file, dan memindahkan file yang lolos autentikasi ke shared memory.
- Jika file tidak sesuai, fungsi akan menghapusnya.
- Setelah semua file yang lolos autentikasi dipindahkan, shared memory akan dilepas menggunakan shmdt.

```sh
// Fungsi untuk memindahkan file yang lolos autentikasi menuju shared memory 
void file_lolos() {
    DIR *dir;
    struct dirent *entry;

    // Membuka direktori folder yang diinginkan, new-data
    dir = opendir("new-data");
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }
    
    // Membuat shared memory
    int shmid = shmget(SHARED_MEMORY_KEY, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error creating shared memory");
        exit(EXIT_FAILURE);
    }
    
    // Menghubungkan shared memory ke ruang alamat proses
    FileInfo *shmaddr = (FileInfo *) shmat(shmid, NULL, 0);
    if (shmaddr == (FileInfo *) -1) {
        perror("Error attaching shared memory");
        exit(EXIT_FAILURE);
    }

    int file_count = 0;
    
    // Looping untuk memindah file menuju shared memory 
    while ((entry = readdir(dir)) != NULL && file_count < MAX_FILES) {
        if (entry->d_type == DT_REG) { // Memeriksa jika entri adalah file regular
            char filename[MAX_FILENAME_LENGTH];
            snprintf(filename, MAX_FILENAME_LENGTH, "new-data/%s", entry->d_name);

            // Menyalin nama file dan isi file jika sesuai kriteria
            if (cek_file(entry->d_name)) {
                strcpy(shmaddr[file_count].filename, entry->d_name);

                FILE *fp = fopen(filename, "r");
                if (fp == NULL) {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }

                fread(shmaddr[file_count].content, 1, MAX_FILE_CONTENT_LENGTH, fp);
                fclose(fp);

                printf("File berhasil disimpan di shared memory:\t[%s]\n", shmaddr[file_count].filename);
                file_count++;
            } else {
                // File yang tidak sesuai akan dihapus
                printf("File tidak valid, dihapus:\t[%s]\n", entry->d_name);
                remove(filename);
            }
        }
    }

    // Melepaskan shared memory
    shmdt((void *) shmaddr);

    closedir(dir);
}
```

3. main():
- Fungsi main() hanya memanggil fungsi file_lolos() untuk menjalankan proses pemindahan file ke shared memory.
- Setelah itu, program akan berakhir dengan mengembalikan nilai 0.

```sh
int main() {
    file_lolos();
    return 0;
}
```

- Berikut dokumentasi ketika AUTH.C di jalankan :
![Screenshot 2024-05-11 131752](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/151058945/fc550fbb-b4ac-4f54-a3b2-6680e5714958) 

#### RATE.C 
Hasil dari kode ini adalah mencetak rating tertinggi dari isi file yang ada di shared memory, baik untuk file-file "parkinglot.csv" maupun "trashcan.csv". Setiap entri akan mencetak jenis file, nama file, nama dengan rating tertinggi, dan rating tertinggi.

1. nama_rating(const char *content, char *name, float *rating):
- Fungsi ini bertujuan untuk memisahkan nama dan rating dari konten file yang diberikan.
- Parameter content adalah konten file yang akan diproses.
- Parameter name adalah pointer ke array char di mana nama akan disimpan.
- Parameter rating adalah pointer ke float di mana rating akan disimpan.
- Fungsi ini menggunakan sscanf() untuk memisahkan nama dan rating dari konten file yang dipisahkan oleh tanda koma (,).

```sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define MAX_FILENAME_LENGTH 512
#define MAX_FILE_CONTENT_LENGTH 1024
#define MAX_FILES 10
#define SHARED_MEMORY_KEY 4321
#define SHARED_MEMORY_SIZE (sizeof(FileInfo) * MAX_FILES)

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char content[MAX_FILE_CONTENT_LENGTH];
} FileInfo;

// Fungsi untuk memisahkan nama dan rating dari konten file
void nama_rating(const char *content, char *name, float *rating) {
    sscanf(content, "%[^,], %f", name, rating);
}
```

2. rating_max(const void *a, const void *b):
- Fungsi ini adalah fungsi pembanding untuk digunakan dalam pengurutan rating.
- Parameter a dan b adalah pointer ke dua rating yang akan dibandingkan.

```sh
// Fungsi untuk membandingkan dua entri berdasarkan rating untuk pengurutan
int rating_max(const void *a, const void *b) {
    float rating_a = *(float *)a;
    float rating_b = *(float *)b;
    return (rating_a < rating_b) - (rating_a > rating_b);
}
```

3. print_ratingmx(const char *filename, const char *content):
- Fungsi ini bertujuan untuk mencetak nama dan rating tertinggi dari konten file yang diberikan.
- Parameter filename adalah nama file yang akan dicetak.
- Parameter content adalah konten file yang akan diproses.
- Fungsi ini memisahkan konten file menjadi baris-baris, kemudian mencari rating tertinggi dari baris-baris tersebut.
- Setelah menemukan rating tertinggi, fungsi mencetak jenis file (Parking Lot atau Trash Can), nama file, nama dengan rating tertinggi, dan rating tertinggi.

```sh
// Fungsi untuk mencetak rating tertinggi setelah diurutkan
void print_ratingmx(const char *filename, const char *content) {

    // Memisahkan dan menyimpan rating dari konten file
    char temp_content[MAX_FILE_CONTENT_LENGTH];
    strcpy(temp_content, content);

    float highest_rating = 0.0;
    char highest_rated_name[MAX_FILENAME_LENGTH] = "";

    char *token = strtok(temp_content, "\n");
    token = strtok(NULL, "\n");
   
    // Looping untuk mencari rating tertinggi
    while (token != NULL) {
        char name[MAX_FILENAME_LENGTH];
        float rating;
        nama_rating(token, name, &rating);

        if (rating > highest_rating) {
            highest_rating = rating;
            strcpy(highest_rated_name, name);
        }

        token = strtok(NULL, "\n");
    }

    printf("Type: %s\n", strstr(filename, "parkinglot") ? "Parking Lot" : "Trash Can");
    printf("Filename: %s\n", filename);
    printf("----------------------\n");
    printf("Name: %s\n", highest_rated_name);
    printf("Rating: %.1f\n", highest_rating);
    printf("--------------------------------\n\n");
}
```

4. main():
- Fungsi main() adalah titik masuk utama program.
- Fungsi ini membuka shared memory yang sudah ada. Kemudian, fungsi ini menghubungkan shared memory tersebut ke ruang alamat proses.
- Selanjutnya, fungsi membaca dan mencetak rating tertinggi dari isi file di shared memory menggunakan fungsi print_ratingmx().
- Setelah selesai, shared memory akan dilepas menggunakan shmdt().
- Program akan berakhir setelah itu.

```sh
int main() {
    // Mendapatkan akses ke shared memory yang sama
    int shmid = shmget(SHARED_MEMORY_KEY, SHARED_MEMORY_SIZE, 0666);
    if (shmid < 0) {
        perror("Error accessing shared memory");
        exit(EXIT_FAILURE);
    }

    // Menghubungkan shared memory ke ruang alamat proses
    FileInfo *shmaddr = (FileInfo *) shmat(shmid, NULL, 0);
    if (shmaddr == (FileInfo *) -1) {
        perror("Error attaching shared memory");
        exit(EXIT_FAILURE);
    }

    // Membaca dan mencetak rating tertinggi dari isi file di shared memory
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strlen(shmaddr[i].filename) > 0) {
            print_ratingmx(shmaddr[i].filename, shmaddr[i].content);
        }
    }

    // Melepaskan shared memory
    shmdt((void *) shmaddr);

    return 0;
}
```

- Berikut dokumentasi ketika RATE.C di jalankan :
![Screenshot 2024-05-11 132004](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/151058945/8203087d-cf56-455d-92f7-8cd555e26105)

#### DB.C
Hasil dari kode ini adalah memindahkan file-file yang ada di shared memory ke dalam folder "database", serta mencatat setiap pemindahan tersebut ke dalam file db.log. File asli akan dihapus setelah disalin ke dalam folder "database".


1. tipe_file(const char *filename, char *type):
- Fungsi ini bertujuan untuk menentukan tipe file berdasarkan judul file.
- Parameter filename adalah nama file yang akan dianalisis.
- Parameter type adalah pointer ke array char di mana tipe file akan disimpan.
- Fungsi ini menggunakan fungsi strstr() untuk mencari substring "trashcan" atau "TrashCan" dalam filename. Jika ditemukan, maka type akan diset menjadi "Trash Can". Jika tidak, fungsi akan mencari substring "parkinglot" atau "ParkingLot". Jika ditemukan, maka type akan diset menjadi "Parking Lot".

```sh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define MAX_FILENAME_LENGTH 512
#define MAX_FILE_CONTENT_LENGTH 1024
#define MAX_FILES 10
#define SHARED_MEMORY_KEY 4321
#define SHARED_MEMORY_SIZE (sizeof(FileInfo) * MAX_FILES)

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char content[MAX_FILE_CONTENT_LENGTH];
} FileInfo;

// Fungsi untuk menentukan tipe file dari judul file
void tipe_file(const char *filename, char *type) {
    if (strstr(filename, "trashcan") || strstr(filename, "TrashCan")) {
        strcpy(type, "Trash Can");
    } else if (strstr(filename, "parkinglot") || strstr(filename, "ParkingLot")) {
        strcpy(type, "Parking Lot");
    } 
}
```

2. catat_log(const char *filename, const char *type):
- Fungsi ini bertujuan untuk mencatat ke dalam file db.log.
- Parameter filename adalah nama file yang akan dicatat.
- Parameter type adalah tipe file yang akan dicatat.
- Fungsi ini mendapatkan waktu lokal saat ini menggunakan time() dan localtime() untuk membuat timestamp.
- Kemudian, fungsi membuka file db.log (jika belum ada, maka akan dibuat), mencatat timestamp, jenis file, dan nama file, dan menutup file tersebut.

```sh
// Fungsi untuk mencatat ke dalam file db.log
void catat_log(const char *filename, const char *type) {
    time_t current_time;
    struct tm *local_time;
    char time_string[80];

    current_time = time(NULL);
    local_time = localtime(&current_time);

    strftime(time_string, sizeof(time_string), "[%d/%m/%Y %H:%M:%S]", local_time);

    // Mencari dan membuka file db.log
    FILE *log_file = fopen("database/db.log", "a");
    if (log_file == NULL) {
        log_file = fopen("database/db.log", "w");
        if (log_file == NULL) {
            perror("Error creating log file");
            exit(EXIT_FAILURE);
        }
        fprintf(log_file, "Log File Created\n");
    }

    fprintf(log_file, "%s\t[%s]\t[%s]\n", time_string, type, filename);
    fclose(log_file);
}
```

3. main():
- Fungsi ini membuka shared memory yang sudah ada. Kemudian, fungsi menghubungkan shared memory tersebut ke ruang alamat proses.
- Selanjutnya, fungsi mendapatkan jalur direktori saat ini menggunakan getcwd() dan menambahkan "/database" ke jalur tersebut.
- Fungsi ini membuat folder "database" jika belum ada.
- Kemudian, fungsi menyalin file dari shared memory ke folder "database". Setiap file yang berhasil disalin akan dicatat ke dalam file db.log. File asli akan dihapus dari folder "new-data" setelah disalin.
- Setelah semua file diproses, shared memory akan dilepas menggunakan shmdt(). Program akan berakhir setelah itu.

```sh
int main() {
    // Mendapatkan akses ke shared memory yang sama
    int shmid = shmget(SHARED_MEMORY_KEY, SHARED_MEMORY_SIZE, 0666);
    if (shmid < 0) {
        perror("Error accessed shared memory");
        exit(EXIT_FAILURE);
    }

    // Menghubungkan shared memory ke ruang alamat proses
    FileInfo *shmaddr = (FileInfo *) shmat(shmid, NULL, 0);
    if (shmaddr == (FileInfo *) -1) {
        perror("Error attaching shared memory");
        exit(EXIT_FAILURE);
    }

    // Mendapatkan jalur direktori saat ini
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current directory");
        exit(EXIT_FAILURE);
    }

    // Menambahkan "/database" ke jalur direktori saat ini
    strcat(cwd, "/database");

    // Membuat folder database jika belum ada
    struct stat st = {0};
    if (stat(cwd, &st) == -1) {
        mkdir(cwd, 0700);
    }

    // Menyalin file dari shared memory ke folder database
    for (int i = 0; i < MAX_FILES; ++i) {
        if (strlen(shmaddr[i].filename) > 0) {
            char dest_path[MAX_FILENAME_LENGTH + 1024];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", cwd, shmaddr[i].filename);

            FILE *fp = fopen(dest_path, "w");
            if (fp == NULL) {
                perror("Error creating file");
                continue;
            }

            fwrite(shmaddr[i].content, 1, strlen(shmaddr[i].content), fp);
            fclose(fp);

            printf("Berhasil memindah file ke database : %s\n", shmaddr[i].filename);

            // Menentukan jenis berdasarkan nama file
            char type[MAX_FILENAME_LENGTH];
            tipe_file(shmaddr[i].filename, type);

            // Mencatat log
            catat_log(shmaddr[i].filename, type);

            // Menghapus file asli dari folder new-data setelah menyalinnya
            char source_path[MAX_FILENAME_LENGTH + 1000];
            snprintf(source_path, sizeof(source_path), "../new-data/%s", shmaddr[i].filename);
            if (remove(source_path) != 0) {
                perror("Error deleting original file");
            }
        }
    }

    // Melepaskan shared memory
    shmdt((void *) shmaddr);

    return 0;
}
```

- Berikut dokumentasi ketika DB.C di jalankan :
![Screenshot 2024-05-11 132155](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/151058945/ec733ece-8df1-43bc-a494-a3f596336be1)

- Berikut dokumentasi direktori sebelum dan sesudah AUTH.C dan DB.C di jalankan
![Screenshot 2024-05-11 131604](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/151058945/bc339bd3-7c65-4656-b086-acf3c3a381cc)

![Screenshot 2024-05-11 132312](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/151058945/d42dc8ff-09c9-48ea-ad92-262740c4eee2)

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
<br />
Jika kode dijalankan :
![Screenshot 2024-05-11 151021](https://github.com/Aceeen/Sisop-3-2024-MH-IT23/assets/150018995/fd924c47-5e2d-433e-b3f8-e2369f41b222)

