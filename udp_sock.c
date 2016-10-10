#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define msleep(m) usleep(m*1000)

char ask_str[1024] = {0};
int recv_have = 0;

void *sock_function(void *arg);
void *recv_function(void *arg);
void *udpbc_function(void *arg);

int main()
{
    int opt;
    int ret = -1;
    int res;
    pthread_t a_thread;
    pthread_t b_thread;
    pthread_t c_thread;
    void *thread_result;

    res = pthread_create(&a_thread, NULL, sock_function, NULL);
    if (res != 0) {
        perror("Thread a creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&b_thread, NULL, recv_function, NULL);
    if (res != 0) {
        perror("Thread b creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&c_thread, NULL, udpbc_function, NULL);
    if (res != 0) {
        perror("Thread c creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread a join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(b_thread, &thread_result);
    if (res != 0) {
        perror("Thread b join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(c_thread, &thread_result);
    if (res != 0) {
        perror("Thread c join failed");
        exit(EXIT_FAILURE);
    }

    printf("Thread joined\n");
    exit(EXIT_SUCCESS);
}

void *sock_function(void *arg)
{
    /* int sockfd; */
    /* int len; */
    /* struct sockaddr_in address; */
    /* int result; */
    /* int opt; */
    /* int socklink = 1; */
    /* int rx = 0; */
    /* char recv_str[1024]; */

    /* address.sin_family = AF_INET; */
    /* address.sin_addr.s_addr = inet_addr("192.168.1.113"); */
    /* address.sin_port = htons(8080); */
    /* len = sizeof(address); */

    /* for (;;) */
    /* { */
    /* while (socklink == 1) */
    /* { */
    /* sockfd = socket(AF_INET, SOCK_STREAM, 0); */
    /* if(sockfd == -1) { */
    /* close(sockfd); */
    /* printf("sockfd create error\n"); */
    /* msleep(100); */
    /* continue; */
    /* } */

    /* result = connect(sockfd, (struct sockaddr *)&address, len); */
    /* if(result == -1) { */
    /* close(sockfd); */
    /* printf("sockfd connect error\n"); */
    /* msleep(100); */
    /* continue; */
    /* } */

    /* opt = fcntl(sockfd, F_GETFL, 0); */
    /* opt |= O_NONBLOCK; */
    /* result = fcntl(sockfd, F_SETFL, opt); */
    /* if(result < 0) */
    /* { */
    /* close(sockfd); */
    /* printf("sockfd fcntl error\n"); */
    /* msleep(100); */
    /* continue; */
    /* } */

    /* socklink = 0; */
    /* } */

    /* while (socklink == 0) */
    /* { */
    /* msleep(100); */
    /* memset(recv_str, 0, 1024); */
    /* rx = read(sockfd, recv_str, 1024); */
    /* if (rx == 0) */
    /* { */
    /* close(sockfd); */
    /* socklink = 1; */
    /* printf("sockfd disconnect!\r\n"); */
    /* } */
    /* else if (rx > 0) */
    /* { */
    /* printf("recv_str=%s\r\n", recv_str); */
    /* } */

    /* if (recv_have == 1) */
    /* { */
    /* write(sockfd, ask_str, strlen(ask_str)); */
    /* recv_have = 0; */
    /* memset(ask_str, 0, 1024); */
    /* } */
    /* } */
    /* } */
    pthread_exit(NULL);
}

void *recv_function(void *arg)
{
    /* char c; */
    /* int length = 0; */
    /* unsigned char *precv = &ask_str[0]; */

    /* for (;;) */
    /* { */
    /* while (recv_have == 0) */
    /* { */
    /* c = getchar(); */
    /* if(c == '\n') */
    /* { */
    /* printf("ask_str=%s\r\n", ask_str); */
    /* msleep(100); */
    /* recv_have = 1; */
    /* length = 0; */
    /* continue; */
    /* } */

    /* *(precv+length) = c; */
    /* length++; */
    /* msleep(10); */
    /* } */
    /* msleep(10); */
    /* } */

    pthread_exit(NULL);
}

void *udpbc_function(void *arg)
{
    struct sockaddr_in BroadcastAddr;
    int BroadcastSock;
    int opt;
    int ret = -1;
    int broadcast = 1;
    int AddrSize = sizeof(struct sockaddr_in);
    char MacString[12] = {0};
    char BroadcastIp[16];
    char udp_str[64] = {0};

    memset(MacString, 0x31, 12);
    udp_str[0x00] = '+';
    udp_str[0x01] = '+';
    udp_str[0x02] = 'H';
    udp_str[0x03] = 'C';
    udp_str[0x04] = 0x01;
    udp_str[0x05] = 0x01;
    udp_str[0x06] = ((13+40)>>8)&0xff;
    udp_str[0x07] = (13+40)&0xff;
    memset(&udp_str[0x08], 0xff, 12);
    memcpy(&udp_str[0x14], &MacString[0], 12);
    udp_str[0x20] = 0x23;
    udp_str[0x21] = 0x00;
    udp_str[0x22] = 0x00;

    udp_str[0x23] = 0x22;
    memcpy(&udp_str[0x24], &MacString[0], 12);

    udp_str[13+40-5] = 0x00;
    udp_str[13+40-4] = 'H';
    udp_str[13+40-3] = 'C';
    udp_str[13+40-2] = '\r';
    udp_str[13+40-1] = '\n';

    memset(&BroadcastAddr, 0, sizeof(struct sockaddr_in));
    BroadcastAddr.sin_family = AF_INET;
    BroadcastAddr.sin_addr.s_addr = inet_addr("192.168.1.255");
    BroadcastAddr.sin_port = htons(59999);

    BroadcastSock = socket(AF_INET, SOCK_DGRAM, 0);
    if(BroadcastSock<0)
    {
        close(BroadcastSock);
        printf("BroadcastSock create error!\r\n");
        return ;
    }
    //printf("BroadcastSock create OK!\r\n");

    if( setsockopt(BroadcastSock,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) == -1)
    {
        perror("setsockopt function!\n");
        exit(1);
    }

    opt = fcntl(BroadcastSock, F_GETFL, 0);
    opt |= O_NONBLOCK;
    ret = fcntl(BroadcastSock, F_SETFL, opt);
    if(ret < 0)
    {
        close(BroadcastSock);
        printf("BroadcastSock fcntl error!\r\n");
        return ;
    }
    printf("BroadcastSock fcntl OK!\r\n");

    for(;;)
    {
        sendto(BroadcastSock, udp_str, 53,  \
                0, (struct sockaddr*)&BroadcastAddr, AddrSize);

        /* Sock_printf(udp_str, 53); */

        msleep(2000);
    }

    pthread_exit(NULL);
}

