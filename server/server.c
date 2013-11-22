#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "../headers/ascii.h"


int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0; // set file descriptors for listen and connection
    struct sockaddr_in serv_addr; // declare struct that will store connection info for socket

    char sendBuff[1025]; // declare buffer for sending data

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // initialize socket
    memset(&serv_addr, '0', sizeof(serv_addr)); // set serv_addr to all 0s
    memset(sendBuff, '0', sizeof(sendBuff));    // set sendBuff to all 0s

    serv_addr.sin_family = AF_INET; // set address family to IPV4, AF_INET6 would be IPV6
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // set address for socket
    serv_addr.sin_port = htons(5000); // set port for socket

    // bind socket based on address and ports set in serv_addr
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // listen on socket listenfd with max backlog of 10 connections
    listen(listenfd, 10);

    while(1) {
        printf("1) Waiting for a connection...\n");
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); // accept a connection
        printf("2) Connection initiated, sending data.\n");

        ascii_init(0, NULL);

        // get the frame as a big string
        //char *frame = ascii_test_string("/home/zfogg/code/c/jp2a2/imgs2/frame_000.jpg");
        char *frame = ascii_test_string("/Users/Craig/Pictures/Rabbit.jpg");

        // write sendBuff to the connection w/ file descriptor connfd
        write(connfd, frame, strlen(frame));
        // free memory for frame
        free(frame);

        printf("3) Closing connection.\n---------------------\n");
        close(connfd);
    }
}
