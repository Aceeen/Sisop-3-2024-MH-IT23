#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void process_file(const char* directory, const char* filename) {
    // Memeriksa apakah file berakhiran yang diinginkan
    if (strstr(filename, "_trashcan.csv") || strstr(filename, "_parkinglot.csv")) {
        printf("File %s accepted.\n", filename);
    } else {
        char full_path[1024];
        sprintf(full_path, "%s/%s", directory, filename); // Membuat path lengkap ke file
        printf("File %s does not meet criteria and will be deleted.\n", filename);
        remove(full_path); // Menghapus file
    }
}

int main() {
    DIR* dir;
    struct dirent* ent;
    char* directory_path = "./new-data"; // Konfirmasi ini adalah direktori yang benar

    if ((dir = opendir(directory_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) { // Hanya proses file reguler
                process_file(directory_path, ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
