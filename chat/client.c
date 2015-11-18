#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Засчитано. Небольшие формальные замечания.
 */

/*
 * Называйте константы единообразно: SERVER_IP, ...
 */

#define serverIp "127.0.0.1"
#define serverPort 51000
#define MAX_MESSAGE_LENGTH 1024

struct sockaddr_in serverAddress, clientAddress;
int sockfd;
int n, len;
char sendline[MAX_MESSAGE_LENGTH];
char receiveLine[MAX_MESSAGE_LENGTH];

/*
 * В названии ф-й должны быть глаголы, а не существительные.
 */
void sender() {
    while(1) {
        fgets(sendline, MAX_MESSAGE_LENGTH, stdin);
        if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
    }
}

void receiver() {
    while(1) {
        if ((n = recvfrom(sockfd, receiveLine, MAX_MESSAGE_LENGTH, 0, (struct sockaddr*) NULL, NULL)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        printf("%s\n", receiveLine);
    }
}

void enterChat() {
    printf("Welcome! Enter your nick \n");
    fgets(sendline, MAX_MESSAGE_LENGTH, stdin);
    sendline[strlen(sendline) - 1] = '\0';
    if (sendto(sockfd, sendline, strlen(sendline) + 1, 0,
    (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
}

int main() {
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }

    bzero(&clientAddress, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(0);
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr*)&clientAddress, sizeof(clientAddress)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_aton(serverIp, &serverAddress.sin_addr) == 0) {
        printf("Invalid IP address\n");
        close(sockfd);
        exit(1);
    }
    enterChat();
    pid_t pid = fork();
    if (pid > 0) {
        sender();
    }
    else {
        receiver();
    }
    close(sockfd);
    return 0;
}
