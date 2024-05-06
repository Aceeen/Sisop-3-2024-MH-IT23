#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

typedef struct {
    char name[100];
    float rating;
} Place;

void find_highest_rated(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file: %s\n", filepath);
        return;
    }

    Place highest_rated = {"", 0.0};
    char buffer[1024];
    int first_line = 1;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (first_line) {  // Skip the header line
            first_line = 0;
            continue;
        }

        Place current;
        if (sscanf(buffer, "%99[^,],%f", current.name, &current.rating) == 2) {
            if (current.rating > highest_rated.rating) {
                highest_rated = current; // Update the highest rated place
            }
        }
    }

    fclose(file);

    // Determine the type based on the filename
    const char *type = strstr(filepath, "trashcan") ? "Trash Can" : "Parking Lot";

    // Extract just the filename from the filepath
    const char *filename = strrchr(filepath, '/');
    filename = (filename) ? filename + 1 : filepath; // If no '/' found, use the whole filepath

    // Print the result
    printf("Type : %s\n", type);
    printf("Filename : %s\n", filename);
    printf("--------------------\n");
    printf("Name : %s\n", highest_rated.name);
    printf("Rating : %.1f\n\n", highest_rated.rating);
}

int main() {
    DIR *d;
    struct dirent *dir;
    char filepath[PATH_MAX];

    d = opendir(".");  // Change from "database" to "."
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {  // Check if it's a regular file
                char *ext = strrchr(dir->d_name, '.');
                if (ext && strcmp(ext, ".csv") == 0) {  // Check if the file is a CSV file
                    snprintf(filepath, sizeof(filepath), "%s", dir->d_name);  // Simplify the filepath
                    find_highest_rated(filepath);
                }
            }
        }
        closedir(d);
    } else {
        perror("Unable to open directory");
    }

    return 0;
}
