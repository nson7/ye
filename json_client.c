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

int sockfd;
int recv_have = 0;
int SendLength = 0;
char g_ucUARTBuffer[1024] = {0};

void parse(char *recv_data)
{
}

void *sock_function(void *arg)
{
    int len;
    struct sockaddr_in address;
    int result;
    int opt;
    int sock_connect = 0;
    int rx = 0;
    char recv_str[1024];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.113");
    address.sin_port = htons(8080);
    len = sizeof(address);

    for (;;)
    {
        while (sock_connect == 0)
        {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if(sockfd == -1) {
                close(sockfd);
                printf("sockfd create error\n");
                msleep(100);
                continue;
            }

            result = connect(sockfd, (struct sockaddr *)&address, len);
            if(result == -1) {
                close(sockfd);
                /* printf("sockfd connect error\n"); */
                msleep(100);
                continue;
            }

            opt = fcntl(sockfd, F_GETFL, 0);
            opt |= O_NONBLOCK;
            result = fcntl(sockfd, F_SETFL, opt);
            if(result < 0)
            {
                close(sockfd);
                printf("sockfd fcntl error\n");
                msleep(100);
                continue;
            }

            sock_connect = 1;
            printf("sockfd connect successfully!\n");
        }

        while (sock_connect == 1)
        {
            memset(recv_str, 0, 1024);
            rx = read(sockfd, recv_str, 1024);
            if (rx == 0)
            {
                close(sockfd);
                sock_connect = 0;
                printf("sockfd disconnect!\r\n");
            }
            else if (rx > 0)
            {
                printf("recv_str=%s\r\n", recv_str);
                parse(recv_str);
            }

            if (recv_have == 1)
            {
                write(sockfd, g_ucUARTBuffer, SendLength);
                SendLength = 0;
                recv_have = 0;
                memset(g_ucUARTBuffer, 0, 1024);
            }

            msleep(100);
        }
    }
    pthread_exit(NULL);
}

void *recv_function(void *arg)
{
    char c;
    unsigned char *precv = &g_ucUARTBuffer[0];

    for (;;)
    {
        while (recv_have == 0)
        {
            c = getchar();
            if(c == '\n')
            {
                printf("g_ucUARTBuffer=%s\r\n", g_ucUARTBuffer);
                msleep(100);
                recv_have = 1;
                continue;
            }

            *(precv+SendLength) = c;
            SendLength++;
            msleep(10);
        }
        msleep(10);
    }

    pthread_exit(NULL);
}

void *udpbc_function(void *arg)
{
    pthread_exit(NULL);
}

void *heart_function(void *arg)
{
    pthread_exit(NULL);
}

int main()
{
    int opt;
    int ret = -1;
    int res;
    pthread_t a_thread;
    pthread_t b_thread;
    pthread_t c_thread;
    pthread_t d_thread;
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

    res = pthread_create(&d_thread, NULL, heart_function, NULL);
    if (res != 0) {
        perror("Thread d creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(a_thread, &thread_result);
    if (res != 0) {
        perror("Thread  a join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(b_thread, &thread_result);
    if (res != 0) {
        perror("Thread  b join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(c_thread, &thread_result);
    if (res != 0) {
        perror("Thread  c join failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_join(d_thread, &thread_result);
    if (res != 0) {
        perror("Thread  d join failed");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

