#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#include <stdbool.h>

/* 
 * 单进程select服务模型，可以实现单进程一对多处理效果 
 */
int net_initializer(char *ip, int port) {
    // ip为NULL, 绑定本机任意ip
    struct sockaddr_in serveraddr;
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket call failed");
    }
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port); // 自定义端口 > 5000
    if (!ip) {
        serveraddr.sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, ip, &serveraddr.sin_addr.s_addr);
    }
    if ((bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) == -1) {
        perror("bind call failed");
    }
    if ((listen(sockfd, 128)) == -1) {
        perror("listen call failed");
    }
    return sockfd;
}

// 首次响应
void first_response(int client_sock, char *cip) {
    char response[1024];
    bzero(response, sizeof(response));
    sprintf(response, "hi, %s connect select server successfully.\n", cip);
    send(client_sock, response, strlen(response), MSG_NOSIGNAL);
    return; 
}

int main(void) {
    // net initializer
    int server_sock = net_initializer(NULL, 8111);
    // select initializer
    fd_set set, oset;
    FD_ZERO(&set);
    FD_SET(server_sock, &set); // 设置首次监听，先于 server_sock
    int readynum;
    int client_sock_array[1020]; // 客户端sock缓存数组
    int maxfd;
    int client_sock;
    struct sockaddr_in clientaddr;
    char cip[16];
    socklen_t addrlen;
    char buffer[1500];
    int recvlen;
    
    // 关于响应
    time_t tp;
    char timebuf[1024];
    maxfd = server_sock; // 设置最大值
    for (int i = 0; i < 1020; i++) {
        client_sock_array[i] = -1;
    }
    
    int flag = 0;
    char *msg = "please tryagain\n";
    printf("select server Runing...\n");
    
    while (true) {
        oset = set; // 传入传出分离，select中使用修改后oset
        printf("select 第%d轮监听\n", ++flag);
        if ((readynum = select(maxfd + 1, &oset, NULL, NULL, NULL)) == -1) {
            perror("select call failed");
            exit(0);
        }
        printf("就绪ready = %d\n", readynum);
        while (readynum) { // 循环处理就绪数据
            if (FD_ISSET(server_sock, &oset)) {
                // server sock ready
                addrlen = sizeof(clientaddr);
                if ((client_sock = accept(server_sock, (struct sockaddr *)&clientaddr, &addrlen)) == -1) {
                    perror("accept call failed");
                    
                }
                // 更新maxfd
                maxfd = client_sock;
                for (int i = 0; i < 1020; i++) {
                    if (client_sock_array[i] == -1) {
                        client_sock_array[i] = client_sock;
                        break;
                    }
                }
                
                // 设置监听
                FD_SET(client_sock, &set);
                // 首次响应客户端
                bzero(cip, 16);
                inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, cip, 16);
                printf("client %s, connect success!\n", cip);
                first_response(client_sock, cip);
            } else {
                // client sock ready
                for (int i = 0; i < 1020; i++) {
                    if (client_sock_array[i] != -1) {
                        if (FD_ISSET(client_sock_array[i], &oset)) {
                            // IO处理
                            bzero(buffer, sizeof(buffer));
                            recvlen = recv(client_sock_array[i], buffer, sizeof(buffer), MSG_DONTWAIT);
                            if (recvlen == 0) {
                                FD_CLR(client_sock_array[i], &set);
                                close(client_sock_array[i]);
                                client_sock_array[i] = -1;
                                break;
                            }
                            buffer[strlen(buffer) - 1] = '\0';
                            if (strcmp(buffer, "localtime") == 0) {
                                bzero(timebuf, sizeof(timebuf));
                                tp = time(NULL);
                                ctime_r(&tp, timebuf);
                                send(client_sock_array[i], timebuf, strlen(timebuf), MSG_NOSIGNAL);
                            } else {
                                send(client_sock_array[i], msg, strlen(msg), MSG_NOSIGNAL);
                            }
                        }
                    }
                    break; 
                }
            }
            --readynum;
        }
    }
    
    printf("server exit\n");
    close(server_sock);
    return 0;
}
