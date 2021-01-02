// 2019054957_Lee Yongwoo
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char* argv[]) {
    // socket create
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0)  {
        printf("Wrong socket fd\n");
        return 1;
    }

    // connect to server
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockaddr.sin_port = htons(8888);
    
    if(connect(fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        printf("Connect Error\n");
        return 2;
    }

    printf("Connect Success\n");
    
    // send data
    char buf[MAXLINE];
    for(;;) {
        memset(buf, 0, MAXLINE);
        read(0, buf, MAXLINE - 1);
        write(fd, buf, strlen(buf));
        
        // if user types "quit", socket closes
        if(strstr(buf, "quit") != NULL) {
            break;
        }
        
        memset(buf, 0, MAXLINE);
        read(fd, buf, MAXLINE - 1);
    }

    // Connection closed
    printf("Connect closed\n");
    close(fd);
    return 0;
}
