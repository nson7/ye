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
volatile int protocol = 0;
int heart_connect = 0;
int SendLength = 0;
char g_ucUARTBuffer[512] = {0};
char MacString[12] = {0};
char phone_mac[12] = {0};
char ask_str[1024] = {0};

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
        if (((*(recv_data + 0x20)) & 0xff) == 0x0F)
        {
            if (((*(recv_data + 0x23)) & 0xff) == 0xE1)
            {
                if (((*(recv_data + 0x24)) & 0xff) == 0xFF)
                {
                    protocol = 1;
                }
            }
        }
    }

    else if (protocol == 1)
    {
        if (((*(recv_data + 0x20)) & 0xff) == 0xE0)
        {
            if (((*(recv_data + 0x23)) & 0xff) == 0x01)
            {
                if (((*(recv_data + 0x24)) & 0xff) == 0x00)
                {
                    memcpy(phone_mac, &recv_data[0x14], 12);
                    int i;
                    for (i = 0; i < 12; i++)
                        printf("%02X", phone_mac[i]);
                    printf("\n");

                    /* send */
                    memset(ask_str, 0, 1024);
                    ask_str[0x00] = '+';
                    ask_str[0x01] = '+';
                    ask_str[0x02] = 'H';
                    ask_str[0x03] = 'C';
                    ask_str[0x04] = 0x01;
                    ask_str[0x05] = 0x01;
                    ask_str[0x06] = (43>>8)&0xff;
                    ask_str[0x07] = 43;
                    memcpy(&ask_str[0x08], phone_mac, 12);
                    memcpy(&ask_str[0x14], MacString, 12);
                    ask_str[0x20] = 0x23;
                    ask_str[0x21] = 0x00;
                    ask_str[0x22] = 0x00;

                    ask_str[0x23] = 0x01;
                    ask_str[0x24] = 0x4F;
                    ask_str[0x25] = 0x4B;


                    ask_str[43-5] = 0x00;
                    ask_str[43-4] = 'H';
                    ask_str[43-3] = 'C';
                    ask_str[43-2] = '\r';
                    ask_str[43-1] = '\n';
                    write(sockfd, ask_str, 43);
                }
            }
            else if (((*(recv_data + 0x23)) & 0xff) == 0x06)
            {
                if (((*(recv_data + 0x24)) & 0xff) == 0x93)
                {
                    /* send */
                    memset(ask_str, 0, 1024);
                    ask_str[0x00] = '+';
                    ask_str[0x01] = '+';
                    ask_str[0x02] = 'H';
                    ask_str[0x03] = 'C';
                    ask_str[0x04] = 0x01;
                    ask_str[0x05] = 0x01;
                    ask_str[0x06] = (0x43>>8)&0xff;
                    ask_str[0x07] = 0x43;
                    memcpy(&ask_str[0x08], phone_mac, 12);
                    memcpy(&ask_str[0x14], MacString, 12);
                    ask_str[0x20] = 0x23;
                    ask_str[0x21] = 0x00;
                    ask_str[0x22] = 0x00;

                    ask_str[0x23] = 0x07;
                    memset(&ask_str[0x24], 0x00, 14);
                    memcpy(&ask_str[0x32],phone_mac, 12);


                    ask_str[0x43-5] = 0x00;
                    ask_str[0x43-4] = 'H';
                    ask_str[0x43-3] = 'C';
                    ask_str[0x43-2] = '\r';
                    ask_str[0x43-1] = '\n';
                    write(sockfd, ask_str, 0x43);
                }
            }

        }
        if (((*(recv_data + 0x20)) & 0xff) == 0x0F)
        {
            if (((*(recv_data + 0x23)) & 0xff) == 0xE0)
            {
                if (((*(recv_data + 0x24)) & 0xff) == 0xFF)
                {
                    protocol = 0;
                    printf("protocol change 0\r\n");
                }
            }
        }
    }
}

void *sock_function(void *arg)
{
    int len;
    struct sockaddr_in address;
    int result;
    int opt;
    int socklink = 1;
    int rx = 0;
    char recv_str[1024];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("120.25.163.9");
    address.sin_port = htons(50005);
    len = sizeof(address);

    for (;;)
    {
        while (socklink == 1)
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
                printf("sockfd connect error\n");
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

            socklink = 0;
            printf("sockfd connect successfully!\n");
        }

        while (socklink == 0)
        {
            memset(recv_str, 0, 1024);
            rx = read(sockfd, recv_str, 1024);
            if (rx == 0)
            {
                close(sockfd);
                socklink = 1;
                printf("sockfd disconnect!\r\n");
            }
            else if (rx > 0)
            {
                printf("recv_str=%s\r\n", recv_str);
                parse(recv_str);
            }

            msleep(100);

            if ((recv_have == 1) && (protocol == 1))
            {
                /* send */
                memset(ask_str, 0, 1024);
                ask_str[0x00] = '+';
                ask_str[0x01] = '+';
                ask_str[0x02] = 'H';
                ask_str[0x03] = 'C';
                ask_str[0x04] = 0x01;
                ask_str[0x05] = 0x01;
                ask_str[0x06] = ((SendLength+42)>>8)&0xff;
                ask_str[0x07] = (SendLength+42)&0xff;
                memcpy(&ask_str[0x08], phone_mac, 12);
                memcpy(&ask_str[0x14], MacString, 12);
                ask_str[0x20] = 0x23;
                ask_str[0x21] = 0x02;
                ask_str[0x22] = 0x00;

                ask_str[0x23] = 0x06;
                ask_str[0x24] = 0x82;
                memcpy(&ask_str[0x25], g_ucUARTBuffer, SendLength);

                ask_str[SendLength+42-5] = 0x00;
                ask_str[SendLength+42-4] = 'H';
                ask_str[SendLength+42-3] = 'C';
                ask_str[SendLength+42-2] = '\r';
                ask_str[SendLength+42-1] = '\n';
                write(sockfd, ask_str, SendLength+42);
                SendLength = 0;
                recv_have = 0;
                memset(g_ucUARTBuffer, 0, 1024);
            }
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
    struct sockaddr_in BroadcastAddr;
    int BroadcastSock;
    int opt;
    int ret = -1;
    int broadcast = 1;
    int AddrSize = sizeof(struct sockaddr_in);
    char BroadcastIp[16];
    char udp_str[64] = {0};

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

void *heart_function(void *arg)
{
    for (;;)
    {
        while(protocol == 0)
        {
            memset(ask_str, 0, 1024);
            ask_str[0x00] = '+';
            ask_str[0x01] = '+';
            ask_str[0x02] = 'H';
            ask_str[0x03] = 'C';
            ask_str[0x04] = 0x01;
            ask_str[0x05] = 0x01;
            ask_str[0x06] = ((53)>>8)&0xff;
            ask_str[0x07] = (53)&0xff;
            memset(&ask_str[0x08], 0x00, 11);
            ask_str[0x13] = 0x01;
            memcpy(&ask_str[0x14], &MacString[0], 12);
            ask_str[0x20] = 0x23;
            ask_str[0x21] = 0x00;
            ask_str[0x22] = 0x00;

            ask_str[0x23] = 0x22;
            memcpy(&ask_str[0x24], &MacString[0], 12);

            ask_str[53-5] = 0x00;
            ask_str[53-4] = 'H';
            ask_str[53-3] = 'C';
            ask_str[53-2] = '\r';
            ask_str[53-1] = '\n';
            write(sockfd, ask_str, 53);
            printf("heart_create --------\r\n");

            sleep(1);
        }

        while (protocol == 1)
        {
            time_t the_time;
            the_time = time((time_t *)0);

            /* registered */
            memset(ask_str, 0, 1024);
            ask_str[0x00] = '+';
            ask_str[0x01] = '+';
            ask_str[0x02] = 'H';
            ask_str[0x03] = 'C';
            ask_str[0x04] = 0x01;
            ask_str[0x05] = 0x01;
            ask_str[0x06] = ((49)>>8)&0xff;
            ask_str[0x07] = (49)&0xff;
            memset(&ask_str[0x08], 0x00, 11);
            ask_str[0x13] = 0x01;
            memcpy(&ask_str[0x14], &MacString[0], 12);
            ask_str[0x20] = 0x23;
            ask_str[0x21] = 0x00;
            ask_str[0x22] = 0x00;

            ask_str[0x23] = 0xE2;
            ask_str[0x24] = 0x00;
            ask_str[0x25] = 0x00;
            ask_str[0x26] = 0x00;
            ask_str[0x27] = 0x00;
            ask_str[0x28] = (the_time>>24)&&0xff;
            ask_str[0x29] = (the_time>>16)&&0xff;
            ask_str[0x2A] = (the_time>>8)&&0xff;
            ask_str[0x2B] = (the_time)&&0xff;


            ask_str[53-5] = 0x00;
            ask_str[53-4] = 'H';
            ask_str[53-3] = 'C';
            ask_str[53-2] = '\r';
            ask_str[53-1] = '\n';

            write(sockfd, &ask_str[0], 53);
            printf("heart_send\r\n");
            sleep(5);
        }
        sleep(5);
    }
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

    memset(MacString, 0x31, 12);

    /* time_t the_time; */
    /* time_t m_time; */
    /* the_time = time((time_t *)0); */
    /* the_time = time(&m_time); */
    /* printf("The time is %ld\n", the_time); */
    /* printf("The time is %ld\n", m_time); */

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

