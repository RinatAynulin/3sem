#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main () {
    int msqid; 
    char pathname[] = "server.c"; 
    key_t key; 
    int lenout, lenin;
    struct mymsgbuf {
        long mtype;
        int digit1;
        int digit2;
        long pid;
    } outbuf;

    struct mymsgbuf2 {
        long mtype;
        int result;
    } inbuf;    

    lenin = sizeof(inbuf) - sizeof(long);
    lenout = sizeof(outbuf) - sizeof(long);
    outbuf.pid = getpid();
    outbuf.mtype = 1;
    scanf("%d", &(outbuf.digit1));
    scanf("%d", &(outbuf.digit2));
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    } 
    if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }
    if (msgsnd(msqid, (struct msgbuf *) &outbuf, lenout, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
            exit(-1);
    }

    if ((msgrcv(msqid, (struct msgbuf *)&inbuf, lenin, outbuf.pid, 0)) < 0) {
        printf("Can\'t receive message from queue\n");
        exit(-1);
    }

    printf("result = %d\n", inbuf.result);
    return 0;
       
}
