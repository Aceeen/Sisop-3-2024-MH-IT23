#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

#define SHM_SIZE 1024  // Ukuran memori bersama

void log_activity(const char* filename, const char* type) {
    FILE* log = fopen("./db.log", "a");  // Simplify path since it's run from database/
    if (log == NULL) {
        perror("Failed to open log file");
        return;
    }

    // Dapatkan waktu dan format
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char date_buffer[20];
    strftime(date_buffer, sizeof(date_buffer), "%d/%m/%y %H:%M:%S", t);

    // Tulis ke log file
    fprintf(log, "[%s] [%s] [%s]\n", date_buffer, type, filename);
    fclose(log);
}

int main() {
    key_t key = 5678;  // Kunci untuk memori bersama
    int shmid;
    char* data;

    // Buat segmen memori bersama
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Tempelkan segmen memori bersama
    data = shmat(shmid, NULL, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    // Buka direktori new-data, naik dua level dari database/
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir("../../new-data")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                strncpy(data, ent->d_name, SHM_SIZE);
                data[SHM_SIZE - 1] = '\0';

                char source_path[256], destination_path[256];
                snprintf(source_path, sizeof(source_path), "../../new-data/%s", data);
                snprintf(destination_path, sizeof(destination_path), "%s", data);

                // Pindahkan file dan log kegiatan
                if (rename(source_path, destination_path) == 0) {
                    const char* type = strstr(data, "trashcan") ? "Trash Can" : "Parking Lot";
                    log_activity(data, type);
                } else {
                    perror("Error moving file");
                }
            }
        }
        closedir(dir);
    } else {
        perror("Unable to open directory '../../new-data'");
    }

    // Lepaskan memori bersama
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
