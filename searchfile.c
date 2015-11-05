#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#define MAX_FILE_NAME_SIZE 256
#define MAX_DIRECTORY_PATH_SIZE 512
#define ERROR "Couldn't find this file"

char* search(char* currentDirectory, int depth, char* fileName) {
    if (depth < 0) {
        return ERROR;
    }
    DIR *dir;
    struct dirent *qDirent;
    struct stat buf;
    if ((dir = opendir(currentDirectory)) == NULL) {
       return ERROR;
    }
    while ((qDirent = readdir(dir)) != NULL) {
        if (strcmp(fileName, qDirent->d_name) == 0) {
            currentDirectory = strcat(currentDirectory, "/");
            currentDirectory = strcat(currentDirectory, qDirent->d_name);
            return currentDirectory;
        }
        if(strcmp(qDirent->d_name, ".") != 0 && strcmp(qDirent->d_name, "..") != 0 && qDirent->d_type == 4) {
            currentDirectory = strcat(currentDirectory, "/");
            currentDirectory = strcat(currentDirectory, qDirent->d_name);
            currentDirectory = search(currentDirectory, depth - 1, fileName);
            return currentDirectory;
        }
    }
    return ERROR;
}

int main(int argc, char** argv) {
    char startDirectory[MAX_DIRECTORY_PATH_SIZE];
    char fileName[MAX_FILE_NAME_SIZE];
    int maxDepth;
    char* result;
    result = malloc(MAX_DIRECTORY_PATH_SIZE * sizeof(char));
    strcpy(startDirectory, argv[1]);
    maxDepth = atoi(argv[2]);
    strcpy(fileName, argv[3]);
    strcpy(result, startDirectory);
    result = search(startDirectory, maxDepth, fileName);
    printf("%s\n", result);
    free(result);
    return 0;
}
