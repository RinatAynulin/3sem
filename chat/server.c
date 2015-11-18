#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_PORT 51000
#define MAX_MESSAGE_LENGTH 1024
#define MAX_CLIENT_NICK_LENGTH 128
#define MAX_CLIENTS_COUNT 128

typedef struct {
    struct sockaddr_in clientAddress;
    char clientNick[MAX_CLIENT_NICK_LENGTH];
}Client;
int sockfd;
int clilen, n;
int clientsCount;
struct sockaddr_in serverAddress, clientAddress;
Client clients[MAX_CLIENTS_COUNT];

int firstMessageFrom(struct sockaddr_in currentClientAddress) {
    int i = 0;
    for (i; i < clientsCount; i++) {
        if (currentClientAddress.sin_addr.s_addr == clients[i].clientAddress.sin_addr.s_addr
            && currentClientAddress.sin_port == clients[i].clientAddress.sin_port) {
            return 0;
        }
    }
    return 1;
}

int getClientId(struct sockaddr_in currentClientAddress) {
    int i = 0;
    for (i; i < clientsCount; i++) {
        if (currentClientAddress.sin_addr.s_addr == clients[i].clientAddress.sin_addr.s_addr
            && currentClientAddress.sin_port == clients[i].clientAddress.sin_port) {
            return i;
        }
    }
}

char* createMessage(char* sendLine, char* clientNick) {
    char* tmp = strdup(sendLine);
    strcpy(sendLine, clientNick);
    strcat(sendLine, ": ");
    strcat(sendLine, tmp);
    free(tmp);
    return sendLine;
}

void sendToChat(char* sendLine, int clientId) {
    int i = 0;
    sendLine = createMessage(sendLine, clients[clientId].clientNick);
    for (i = 0; i < clientsCount; i++) {
        if (i != clientId) {
            if (sendto(sockfd, sendLine, strlen(sendLine) + 1, 0, (struct sockaddr*)&(clients[i].clientAddress), sizeof(clients[i].clientAddress)) < 0) {
                perror(NULL);
                close(sockfd);
                exit(1);
            }
        printf("Send %s to %s\n", sendLine, clients[i].clientNick);
        }
    }
}

void addNewClient(struct sockaddr_in currentClientAddress, char* clientNick) {
    char sendLine[MAX_MESSAGE_LENGTH];
    bzero(&clients[clientsCount].clientAddress, sizeof(clients[clientsCount].clientAddress));
    clients[clientsCount].clientAddress.sin_family = currentClientAddress.sin_family;
    clients[clientsCount].clientAddress.sin_port = currentClientAddress.sin_port;
    clients[clientsCount].clientAddress.sin_addr.s_addr = currentClientAddress.sin_addr.s_addr;
    strcpy(clients[clientsCount].clientNick, clientNick);
    printf("add new client %s № %d port : %d \n", clients[clientsCount].clientNick, clientsCount, clients[clientsCount].clientAddress.sin_port);
    strcpy(sendLine, " has entered the chat room");
    sendToChat(sendLine, clientsCount);
    clientsCount++;
}

int main() {
    char sendLine[MAX_MESSAGE_LENGTH];
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }
    else {
        printf("Sock fd:%d\n", sockfd);
    }

    if (bind(sockfd, (struct sockaddr*) &serverAddress,
        sizeof(serverAddress)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    while (1) {
        clilen = sizeof(clientAddress);
        if ((n = recvfrom(sockfd, sendLine, MAX_MESSAGE_LENGTH - 1, 0,
        (struct sockaddr*)&clientAddress, &clilen)) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        if (firstMessageFrom(clientAddress)) {
            addNewClient(clientAddress, sendLine);
        }
        else {
            int clientId = getClientId(clientAddress);
            sendToChat(sendLine, clientId);
        }
        printf("receive %s from %s\n", sendLine, clients[getClientId(clientAddress)].clientNick);
    }
    return 0;
}
