#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"

#include "../ascii/ascii.h"


int sockfd = 0;

int main(int argc, char *argv[]) {
    char recvBuff[10000];
    struct sockaddr_in serv_addr;

    // Close socket on program exit.
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    // incorrect number of arguments
    if(argc != 2) {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    // try to open a socket
    memset(recvBuff, '0',sizeof(recvBuff));
    // error creating socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    // reserve memory space to store IP address
    memset(&serv_addr, '0', sizeof(serv_addr));

    // set type of address to IPV4 and port to 5000
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    // an error occurred when trying to set server address and port number
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0) {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // failed when trying to connect to the server
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    /* read from the socket as long as the size of the read is > 0 */
    ascii_write_init();
    int read_result = 0;
    while (0 < (read_result = read(sockfd, recvBuff, sizeof(recvBuff)-1))) {
        recvBuff[read_result] = 0;
        ascii_write(recvBuff);
    }
    ascii_write_destroy();

    return 0;
}

void sigint_handler(int sig_no) {
    fprintf(stderr, "\n SIGINT: cleaning up and exiting . . . \n");
    ascii_write_destroy();
    close(sockfd);
}
