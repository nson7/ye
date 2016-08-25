#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

void *thread_function(void *arg);
void *Sock_function(void *arg);

int server_sockfd, client_sockfd[10];
int SockID = 0;

int main()
{
    int opt;
    int ret = -1;
    int res;
    pthread_t a_thread;
    pthread_t b_thread;
    void *thread_result;

    int server_len;
    struct sockaddr_in server_address;

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    opt = fcntl(server_sockfd, F_GETFL, 0);
    opt |= O_NONBLOCK;
    ret = fcntl(server_sockfd, F_SETFL, opt);
    if(ret < 0)
    {
        close(server_sockfd);
        printf("fcntl error!\n");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8080);
    server_len = sizeof(server_address);
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    listen(server_sockfd, 5);

    printf("function\n");
    res = pthread_create(&a_thread, NULL, thread_function, NULL);
    if (res != 0) {
        perror("Thread b creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&b_thread, NULL, Sock_function, NULL);
    if (res != 0) {
        perror("Thread b creation failed");
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
    printf("Thread joined\n");
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
    int opt;
    int ret = -1;
    int client_len;
    struct sockaddr_in client_address;
    printf("thread_function\n");

    int i;
    for (i = 0; i < 8; i++)
    {
        client_sockfd[i] = -1;
        while (client_sockfd[i] < 0)
        {
            /* printf("sockfd = %d\n", client_sockfd[i]); */
            client_len = sizeof(client_address);
            client_sockfd[i] = accept(server_sockfd, \
                (struct sockaddr *)&client_address, &client_len);
            usleep(100000);
        }
        printf("accept success\n");
        SockID += 1;

        opt = fcntl(client_sockfd[i], F_GETFL, 0);
        opt |= O_NONBLOCK;
        ret = fcntl(client_sockfd[i], F_SETFL, opt);
        if(ret < 0)
        {
            close(client_sockfd[i]);
            printf("fcntl error!\n");
            pthread_exit(NULL);
        }
        usleep(100000);
    }
    pthread_exit(NULL);
}

void *Sock_function(void *arg)
{
    printf("Sock_function\n");
    int i;
    int g_BytesRx;

    for (;;)
    {
        for(i=1;i<=SockID;i++)
        {
            char *SockBuf = (char *)malloc(sizeof(char)*2048);
            memset(SockBuf, 0, 2048);
            g_BytesRx = read(client_sockfd[i-1], SockBuf, 2032);
            //printf("g_BytesRx=%d\r\n",g_BytesRx);

            if (g_BytesRx > 0)
            {
                SockBuf[g_BytesRx] = 0;
                printf("%s\n", SockBuf);
                g_BytesRx = 0;
            }
            else if(g_BytesRx == 0)
            {
                close(client_sockfd[i-1]);
            }
            free(SockBuf);
            usleep(100000);
        }
        usleep(100000);
    }
    pthread_exit(NULL);
}

