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
#include "cJSON.h"

#define msleep(m) usleep(m*1000)

int sockfd;
int recv_have = 0;
int SendLength = 0;
int sock_connect = 0;
int heartbeat_connect = 0;
char g_ucUARTBuffer[512] = {0};
char g_object_mac[16] = {0};
char g_self_mac[16] = "123456789027";

void parse_json(char *text)
{
    cJSON *recv_root = NULL;
    cJSON *recv_version = NULL;
    cJSON *recv_object_mac = NULL;
    cJSON *recv_self_mac = NULL;
    cJSON *recv_msg_type = NULL;
    cJSON *recv_msg_data = NULL;

    cJSON *send_root = NULL;
    cJSON *send_version = NULL;
    cJSON *send_object_mac = NULL;
    cJSON *send_self_mac = NULL;
    cJSON *send_msg_type = NULL;
    cJSON *send_msg_data = NULL;
    char *recv_out;
    char *send_out;

    recv_root = cJSON_Parse(text);
    if (recv_root)
    {
        recv_version = cJSON_GetObjectItem(recv_root,"version");

        if (recv_version == NULL)
            return ;
        if (recv_version->valuestring == NULL)
            return ;

        recv_object_mac = cJSON_GetObjectItem(recv_root,"object_mac");

        if (recv_object_mac == NULL)
            return ;
        if (recv_object_mac->valuestring == NULL)
            return ;

        recv_self_mac = cJSON_GetObjectItem(recv_root,"self_mac");

        if (recv_self_mac == NULL)
            return ;
        if (recv_self_mac->valuestring == NULL)
            return ;

        recv_msg_type = cJSON_GetObjectItem(recv_root,"msg_type");

        if (recv_msg_type == NULL)
            return ;
        if (recv_msg_type->valuestring == NULL)
            return ;

        recv_msg_data = cJSON_GetObjectItem(recv_root,"msg_data");

        if (recv_msg_data == NULL)
            return ;
        if (recv_msg_data->valuestring == NULL)
            return ;

        printf("version=%s\n", recv_version->valuestring);
        /* printf("data=%d\n", data->valueint); */
        printf("object_mac=%s\n", recv_object_mac->valuestring);
        printf("self_mac=%s\n", recv_self_mac->valuestring);
        printf("msg_type=%s\n", recv_msg_type->valuestring);
        printf("msg_data=%s\n", recv_msg_data->valuestring);

        if (strcmp(recv_msg_type->valuestring, "register") == 0)
        {
            if (strcmp(recv_msg_data->valuestring, "OK") == 0)
            {
                heartbeat_connect = 1;
            }
        }

        else if (strcmp(recv_msg_type->valuestring, "uart_ready") == 0)
        {
            strcpy(g_object_mac, recv_self_mac->valuestring);

            send_root = cJSON_CreateObject();
            cJSON_AddStringToObject(send_root, "version", "v1.0");
            cJSON_AddStringToObject(send_root, "object_mac", g_object_mac);
            cJSON_AddStringToObject(send_root, "self_mac", g_self_mac);
            /* cJSON_AddNumberToObject(send_root, "keyvalue", 777); */
            cJSON_AddStringToObject(send_root, "msg_type", "uart_ready");
            cJSON_AddStringToObject(send_root, "msg_data", "OK");
            send_out = cJSON_Print(send_root);
            write(sockfd, send_out, strlen(send_out));
            printf("----------------\r\n");
            printf("----------------\r\n");
            printf("----------------\r\n");
        }

        else if (strcmp(recv_msg_type->valuestring, "uart_send") == 0)
        {
            printf("uart_print=%s\n", recv_msg_data->valuestring);
            printf("----------------\r\n");
            printf("----------------\r\n");
            printf("----------------\r\n");
        }

        /* printf("text=%s\r\n", text); */
        /* out = cJSON_Print(recv_root); */
        /* printf("out=%s\r\n", out); */
        /* free(out); */
        printf("\r\n\r\n\r\n");
    }

    cJSON_Delete(recv_root);
    cJSON_Delete(send_root);
}

char *send_json()
{
    cJSON *root = NULL;
    char *out;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", "v1.0");
    cJSON_AddStringToObject(root, "object_mac", g_object_mac);
    cJSON_AddStringToObject(root, "self_mac", g_self_mac);
    /* cJSON_AddNumberToObject(root, "keyvalue", 777); */
    cJSON_AddStringToObject(root, "msg_type", "uart_send");
    cJSON_AddStringToObject(root, "msg_data", g_ucUARTBuffer);
    out = cJSON_Print(root);
    cJSON_Delete(root);
    return out;
}

void *sock_task(void *arg)
{
    int len;
    struct sockaddr_in address;
    int result;
    int opt;
    int rx = 0;
    char recv_str[1024];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.1.177");
    address.sin_port = htons(1234);
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
                printf("recv_str=%s----------\r\n", recv_str);
                parse_json(recv_str);
            }

            if (recv_have == 1)
            {
                char *json_out = send_json();
                write(sockfd, json_out, strlen(json_out));
                printf("%s\r\n", json_out);
                SendLength = 0;
                recv_have = 0;
                memset(g_ucUARTBuffer, 0, 512);
            }

            msleep(10);
        }
    }
    pthread_exit(NULL);
}

void *recv_task(void *arg)
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

void *udp_task(void *arg)
{
    pthread_exit(NULL);
}

void *heartbeat_task(void *arg)
{
    cJSON *root = NULL;
    cJSON *version = NULL;
    cJSON *object_mac = NULL;
    cJSON *self_mac = NULL;
    cJSON *msg_type = NULL;
    cJSON *msg_data = NULL;
    char *heart_out;

    for (;;)
    {
        while (sock_connect == 1)
        {
            if (heartbeat_connect == 0)
            {
                root = cJSON_CreateObject();
                cJSON_AddStringToObject(root, "version", "v1.0");
                cJSON_AddStringToObject(root, "object_mac", "001");
                cJSON_AddStringToObject(root, "self_mac", "123456789027");
                /* cJSON_AddNumberToObject(root, "keyvalue", 777); */
                cJSON_AddStringToObject(root, "msg_type", "register");
                cJSON_AddStringToObject(root, "msg_data", "");
                heart_out = cJSON_Print(root);
                write(sockfd, heart_out, strlen(heart_out));
                free(heart_out);
                cJSON_Delete(root);
            }
            else if (heartbeat_connect == 1)
            {
                root = cJSON_CreateObject();
                cJSON_AddStringToObject(root, "version", "v1.0");
                cJSON_AddStringToObject(root, "object_mac", "001");
                cJSON_AddStringToObject(root, "self_mac", "123456789027");
                /* cJSON_AddNumberToObject(root, "keyvalue", 777); */
                cJSON_AddStringToObject(root, "msg_type", "heart");
                cJSON_AddStringToObject(root, "msg_data", "");
                heart_out = cJSON_Print(root);
                write(sockfd, heart_out, strlen(heart_out));
                free(heart_out);
                cJSON_Delete(root);
            }

            sleep(50);
        }

        sleep(1);
    }
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

    res = pthread_create(&a_thread, NULL, sock_task, NULL);
    if (res != 0) {
        perror("Thread a creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&b_thread, NULL, recv_task, NULL);
    if (res != 0) {
        perror("Thread b creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&c_thread, NULL, udp_task, NULL);
    if (res != 0) {
        perror("Thread c creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&d_thread, NULL, heartbeat_task, NULL);
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

