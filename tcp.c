#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

bool is_directory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0) {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

void copy_file(const char *src, const char *dst) {
    FILE *sourceFile = fopen(src, "r");
    if (!sourceFile) {
        perror("Error opening source file");
        exit(1);
    }

    FILE *destFile = fopen(dst, "w");
    if (!destFile) {
        perror("Error opening destination file");
        fclose(sourceFile);
        exit(1);
    }

    char ch;
    while ((ch = fgetc(sourceFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(sourceFile);
    fclose(destFile);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        perror("Incorrect number of arguments");
        exit(1);
    }

    char dest[1000];
    if (argc == 3) {
        // Either copy file to file or file to directory
        if (is_directory(argv[2])) {
            // Make destination path
            char *filename = strrchr(argv[1], '/');
            if (filename) {
                filename++;
            } else {
                filename = argv[1];
            }
            snprintf(dest, sizeof(dest), "%s/%s", argv[2], filename);
            copy_file(argv[1], dest);
        } else {
            copy_file(argv[1], argv[2]);
        }
    } else {
        // Multiple sources to a directory
        if (!is_directory(argv[argc-1])) {
            perror("When listing multiple sources to copy, last argument type must be a directory.\n");
            exit(1);
        }
        for (int i = 1; i < argc - 1; i++) {
            char *filename = strrchr(argv[i], '/');
            if (filename) {
                filename++;
            } else {
                filename = argv[i];
            }
            snprintf(dest, sizeof(dest), "%s/%s", argv[argc-1], filename);
            copy_file(argv[i], dest);
        }
    }

    return 0;
}