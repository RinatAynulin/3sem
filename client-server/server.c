#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#define N 2
int main() {
    struct mymsgbuf {
        long mtype;
        int digit1;
        int digit2;
        long pid;
    } inbuf;

    struct mymsgbuf2 {
        long mtype;
        int result;
    } outbuf;

    int msqid = 0;
    int semid = 0;
    char pathname[] = "server.c";
    key_t key;
    int lenout, lenin;

    lenin = sizeof(inbuf) - sizeof(long);
    lenout = sizeof(outbuf) - sizeof(long);

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    semid = semget(key, 1, 0666 | IPC_CREAT);
    struct sembuf semaphore;
    semaphore.sem_op  = N;
    semaphore.sem_flg = 0;
    semaphore.sem_num = 0;
    semop(semid, &semaphore, 1);
    while (1) {
        if ((msgrcv(msqid, (struct msgbuf *)&inbuf, lenin, 1, 0)) < 0) {
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        int pid = fork();
        if (pid == 0)  {
            semaphore.sem_op  = -1;
            semaphore.sem_flg = 0;
            semaphore.sem_num = 0;
            semop(semid, &semaphore, 1);
            printf("started computing for %ld\n", inbuf.pid);
            sleep(10);
            outbuf.result = inbuf.digit1 * inbuf.digit2;
            outbuf.mtype = inbuf.pid;

            if (msgsnd(msqid, (struct msgbuf *) &outbuf, lenout, 0) < 0) {
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
                exit(-1);
            }
            semaphore.sem_op  = 1;
            semaphore.sem_flg = 0;
            semaphore.sem_num = 0;
            semop(semid, &semaphore, 1);
            exit(0);
        }
    }
    semctl(semid, IPC_RMID, 0);
    msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
    return 0;
}
