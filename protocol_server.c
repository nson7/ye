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

int recv_have = 0;
int SendLength = 0;
char g_ucUARTBuffer[512] = {0};
char module_mac[12] = {0};
char ask_str[1024] = {0};
int protocol = 0;
int send_count = 0;
int server_sockfd, client_sockfd;

void parse(char *recv_data)
{
    int i;
    for (i = 0;i < 67;i++)
    {
        printf("%02X = %02X\r\n", i, (*(recv_data+i))&0xff);
    }
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");

    if (protocol == 0)
    {

        if (((*(recv_data + 0x20)) & 0xff) == 0x23)
        {
            if (((*(recv_data + 0x23)) & 0xff) == 0x22)
            {
                memcpy(module_mac, &recv_data[0x14], 12);

                memset(ask_str, 0, 1024);
                ask_str[0x00] = '+';
                ask_str[0x01] = '+';
                ask_str[0x02] = 'H';
                ask_str[0x03] = 'C';
                ask_str[0x04] = 0x01;
                ask_str[0x05] = 0x01;
                ask_str[0x06] = (42>>8)&0xff;
                ask_str[0x07] = 42;
                memcpy(&ask_str[0x08], module_mac, 12);
                memset(&ask_str[0x14], 0x00, 11);
                ask_str[0x1F] = 0x01;
                ask_str[0x20] = 0x0F;
                ask_str[0x21] = 0x00;
                ask_str[0x22] = 0x00;

                ask_str[0x23] = 0xE1;

                ask_str[42-5] = 0xFF;
                ask_str[42-4] = 'H';
                ask_str[42-3] = 'C';
                ask_str[42-2] = '\r';
                ask_str[42-1] = '\n';
                write(client_sockfd, ask_str, 42);
                protocol = 1;
                msleep(100);
            }
        }
    }

    else if(protocol == 1)
    {
        if (send_count == 0)
        {
            memset(ask_str, 0, 1024);
            ask_str[0x00] = '+';
            ask_str[0x01] = '+';
            ask_str[0x02] = 'H';
            ask_str[0x03] = 'C';
            ask_str[0x04] = 0x01;
            ask_str[0x05] = 0x01;
            ask_str[0x06] = (42>>8)&0xff;
            ask_str[0x07] = 42;
            memcpy(&ask_str[0x08], module_mac, 12);
            memset(&ask_str[0x14], 0x01, 12);
            ask_str[0x20] = 0xE0;
            ask_str[0x21] = 0x01;
            ask_str[0x22] = 0x00;

            ask_str[0x23] = 0x01;
            ask_str[0x24] = 0x00;

            ask_str[42-5] = 0x00;
            ask_str[42-4] = 'H';
            ask_str[42-3] = 'C';
            ask_str[42-2] = '\r';
            ask_str[42-1] = '\n';
            write(client_sockfd, ask_str, 42);
            printf("send_count = %d\r\n", send_count);
            send_count++;
        }
        else if (send_count == 1)
        {
        if (((*(recv_data + 0x20)) & 0xff) == 0x23)
        {
            if (((*(recv_data + 0x23)) & 0xff) == 0x01)
            {
            memset(ask_str, 0, 1024);
            ask_str[0x00] = '+';
            ask_str[0x01] = '+';
            ask_str[0x02] = 'H';
            ask_str[0x03] = 'C';
            ask_str[0x04] = 0x01;
            ask_str[0x05] = 0x01;
            ask_str[0x06] = (42>>8)&0xff;
            ask_str[0x07] = 42;
            memcpy(&ask_str[0x08], module_mac, 12);
            memset(&ask_str[0x14], 0x01, 12);
            ask_str[0x20] = 0xE0;
            ask_str[0x21] = 0x01;
            ask_str[0x22] = 0x00;

            ask_str[0x23] = 0x06;
            ask_str[0x24] = 0x93;

            ask_str[42-5] = 0x00;
            ask_str[42-4] = 'H';
            ask_str[42-3] = 'C';
            ask_str[42-2] = '\r';
            ask_str[42-1] = '\n';
            write(client_sockfd, ask_str, 42);
            printf("send_count = %d\r\n", send_count);
            }
            else if (((*(recv_data + 0x23)) & 0xff) == 0x07)
            {
            memset(ask_str, 0, 1024);
            ask_str[0x00] = '+';
            ask_str[0x01] = '+';
            ask_str[0x02] = 'H';
            ask_str[0x03] = 'C';
            ask_str[0x04] = 0x01;
            ask_str[0x05] = 0x01;
            ask_str[0x06] = (42>>8)&0xff;
            ask_str[0x07] = 42;
            memcpy(&ask_str[0x08], module_mac, 12);
            memset(&ask_str[0x14], 0x01, 12);
            ask_str[0x20] = 0xE0;
            ask_str[0x21] = 0x01;
            ask_str[0x22] = 0x00;

            ask_str[0x23] = 0x06;
            ask_str[0x24] = 0x93;

            ask_str[42-5] = 0x00;
            ask_str[42-4] = 'H';
            ask_str[42-3] = 'C';
            ask_str[42-2] = '\r';
            ask_str[42-1] = '\n';
            write(client_sockfd, ask_str, 42);
            printf("send_count = %d\r\n", send_count);
            }
        }
        }
    }
}

void *server_function(void *arg)
{
    int rx = 0;
    int opt = 0;
    int ret = -1;
    int sock_connect = 0;
    int server_len, client_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char recv_buff[1024];

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);
    server_len = sizeof(server_addr);

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        close(server_sockfd);
        printf("server_sockfd create error!\r\n");
    }
    //printf("server_sockfd create OK!\r\n");

    opt = fcntl(server_sockfd, F_GETFL, 0);
    opt |= O_NONBLOCK;
    ret = fcntl(server_sockfd, F_SETFL, opt);
    if(ret < 0)
    {
        close(server_sockfd);
        printf("fcntl error!\r\n");
    }
    //printf("server_sockfd fcntl OK!\r\n");

    ret = bind(server_sockfd, (struct sockaddr *)&server_addr,  sizeof(server_addr));
    if (ret != 0)
    {
        close(server_sockfd);
        printf("server_sockfd bind error!\r\n");
    }
    //printf("server_sockfd bind OK!\r\n");

    ret = listen(server_sockfd, 5);
    if (ret != 0)
    {
        close(server_sockfd);
        printf("server_sockfd listen error!\r\n");
    }
    client_len = sizeof(client_addr);

    for(;;)
    {
        client_sockfd = -1;
        while(client_sockfd < 0)
        {
            client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
            msleep(100);
        }

        sock_connect = 1;
        printf("client_sockfd = %d\r\n", client_sockfd);

        opt = fcntl(client_sockfd, F_GETFL, 0);
        opt |= O_NONBLOCK;
        ret = fcntl(client_sockfd, F_SETFL, opt);

        memset(ask_str, 0, 1024);
        ask_str[0x00] = '+';
        ask_str[0x01] = '+';
        ask_str[0x02] = 'H';
        ask_str[0x03] = 'C';
        ask_str[0x04] = 0x01;
        ask_str[0x05] = 0x01;
        ask_str[0x06] = (42>>8)&0xff;
        ask_str[0x07] = 42;
        memset(&ask_str[0x08], 0xFF, 12);
        memset(&ask_str[0x14], 0x00, 11);
        ask_str[0x1F] = 0x01;
        ask_str[0x20] = 0x0F;
        ask_str[0x21] = 0x00;
        ask_str[0x22] = 0x00;

        ask_str[0x23] = 0x22;
        ask_str[0x24] = 0xFF;

        ask_str[42-5] = 0x00;
        ask_str[42-4] = 'H';
        ask_str[42-3] = 'C';
        ask_str[42-2] = '\r';
        ask_str[42-1] = '\n';
        write(client_sockfd, ask_str, 42);
        msleep(100);
        write(client_sockfd, ask_str, 42);
        msleep(100);
        write(client_sockfd, ask_str, 42);
        msleep(100);

        while(sock_connect == 1)
        {
            memset(recv_buff, 0, 512);
            rx = read(client_sockfd, recv_buff, 512);
            if (rx == 0)
            {
                sock_connect = 0;
                printf("client_sockfd disconnect!\r\n");
                msleep(100);
                continue;
            }
            else if (rx > 0)
            {
                printf("recv = %s\r\n", recv_buff);
                parse(recv_buff);
            }

            if (recv_have)
            {
                write(client_sockfd, g_ucUARTBuffer, SendLength);
                SendLength = 0;
                recv_have = 0;
                memset(g_ucUARTBuffer, 0, 1024);
            }
            msleep(1000);
        }
    }
}

void *udpbc_function(void *arg)
{

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

    res = pthread_create(&a_thread, NULL, server_function, NULL);
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

