// 2019054957_ Lee Yongwoo
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char *argv[]) {
    // socket create
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("Wrong socket fd\n");
        exit(0);
    }

    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Bind Error\n");
        exit(0);
    }

    // listen
    if(listen(sockfd, 5) < 0) {
        printf("Listen Error\n");
        exit(0);
    }

    int clientSockfd, clientLen;
    struct sockaddr_in clientAddr;

    char buf[MAXLINE];

    // create client socket & accept
    signal(SIGCHLD, SIG_IGN);
    for(;;) {
        clientLen = sizeof(clientAddr);
        clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientLen);
        
        if(clientSockfd < 0) {
            printf("Accept Error\n");
            exit(0);
        }
        printf("Client %d Connect\nIP Address: %s\n", clientSockfd, inet_ntoa(clientAddr.sin_addr));
        
        // multiprocess
        int pid = fork();

        // for child process, receive data & print
        if(pid == 0) {
            for(;;) {
                memset(buf, 0, MAXLINE);
                if(read(clientSockfd, buf, MAXLINE - 1) < 0) {
                    close(clientSockfd);
                    exit(0);
                }
                else {
                    // if data contains "quit", close client socket
                    if(strstr(buf, "quit") != NULL) {
                        printf("client %d close\n", clientSockfd);
                        close(clientSockfd);
                        exit(0);
                    }
                    
                    printf("%d> %s", clientSockfd, buf);
                    write(clientSockfd, buf, strlen(buf));
                }
            }
        }
        // Exception for fork error
        if(pid == -1) {
            printf("Fork Error\n");
            return 1;
        }
    }
    return 0;
}
