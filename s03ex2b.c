#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#define N 5

int main() {
    pid_t id, parentId, pid = 0;
    int i, status;
    for (i = 0; i < N + 1; i++) {
        if (pid != 0)
            break;
        if (pid == 0) {
            id = getpid();
            parentId = getppid();
            printf("Create proccess %d (Parent: %d)\n", id, parentId);
            if (i < N) 
                pid = fork();
        }
    }
    wait(&status);
    printf("Proccess %d (Parent: %d) terminated\n", id, parentId);
    return 0;
}
