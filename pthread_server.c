
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

int recv_have = 0;
int sock_link = 0;
int send_length = 0;
int heartbeart_connect[10] = {0};
int heartbeat[10] = {0};
char g_ucUARTBuffer[512] = {0};
int server_sockfd, client_sockfd[10];

void *sock_task(void *arg);

typedef struct __client_t
{
    int sockfd;
    char mac[16];
} client_t;

client_t client_data[10];

int search_mac(char *mac_str)
{
    int n;
    if (strlen(mac_str) != 12)
    {
        printf("strlen------\r\n");
        return 0;
    }

    if (strcmp(mac_str, "000000000000") == 0)
    {
        printf("memcmp-------\r\n");
        return 0;
    }

    for (n = 0; n < 10; n++)
    {
        if (client_data[n].sockfd > 0)
        {
            if (strcmp(client_data[n].mac, mac_str) == 0)
            {
                printf("mac_str=%s\r\n", mac_str);
                printf("client_data[%d].sockfd=%d\r\n", n, client_data[n].sockfd);
                printf("client_data[%d].mac=%s\r\n", n, client_data[n].mac);
                printf("----------------\r\n");
                printf("----------------\r\n");
                printf("----------------\r\n");

                return client_data[n].sockfd;
            }
        }
    }

    return 0;
}

void parse_json(char *text, int n)
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
    int temp_sockfd = 0;

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
            if (strlen(recv_self_mac->valuestring) == 12)
            {
                client_data[n].sockfd = client_sockfd[n];
                strcpy(client_data[n].mac, recv_self_mac->valuestring);

                send_root = cJSON_CreateObject();
                cJSON_AddStringToObject(send_root, "version", "v1.0");
                cJSON_AddStringToObject(send_root, "object_mac", recv_self_mac->valuestring);
                cJSON_AddStringToObject(send_root, "self_mac", "001");
                /* cJSON_AddNumberToObject(send_root, "keyvalue", 777); */
                cJSON_AddStringToObject(send_root, "msg_type", "register");
                cJSON_AddStringToObject(send_root, "msg_data", "OK");
                send_out = cJSON_Print(send_root);
                write(client_sockfd[n], send_out, strlen(send_out));
                heartbeat[n] = 1;
            }
        }

        else if (strcmp(recv_msg_type->valuestring, "heartbeat") == 0)
        {
            heartbeat[n] = 1;
        }

        else if (strcmp(recv_msg_type->valuestring, "uart_ready") == 0)
        {
            if ((temp_sockfd = search_mac(recv_object_mac->valuestring)) != 0)
            {
                write(temp_sockfd, text, strlen(text));
                printf("temp_sockfd=%d\r\n", temp_sockfd);
            }
        }

        else if (strcmp(recv_msg_type->valuestring, "uart_send") == 0)
        {
            if ((temp_sockfd = search_mac(recv_object_mac->valuestring)) != 0)
            {
                write(temp_sockfd, text, strlen(text));
                printf("temp_sockfd=%d\r\n", temp_sockfd);
            }
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
    cJSON *version = NULL;
    cJSON *object_mac = NULL;
    cJSON *self_mac = NULL;
    cJSON *msg_type = NULL;
    cJSON *msg_data = NULL;
    char *out;

    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "version", "v1.0");
    cJSON_AddStringToObject(root, "object_mac", "123456789027");
    cJSON_AddStringToObject(root, "self_mac", "123456789027");
    /* cJSON_AddNumberToObject(root, "keyvalue", 777); */
    cJSON_AddStringToObject(root, "msg_type", "uart_send");
    cJSON_AddStringToObject(root, "msg_data", g_ucUARTBuffer);
    out = cJSON_Print(root);
    cJSON_Delete(root);
    return out;
}


void sock_init()
{
    int opt;
    int ret = -1;
    int server_len;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(1234);
    server_len = sizeof(server_addr);

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0)
    {
        close(server_sockfd);
        printf("server_sockfd create error!\r\n");
        return ;
    }

    opt = fcntl(server_sockfd, F_GETFL, 0);
    opt |= O_NONBLOCK;
    ret = fcntl(server_sockfd, F_SETFL, opt);
    if (ret < 0)
    {
        close(server_sockfd);
        printf("server_sockfd fcntl error!\r\n");
        return ;
    }

    ret = bind(server_sockfd, (struct sockaddr *)&server_addr, server_len);
    if (ret != 0)
    {
        close(server_sockfd);
        printf("server_sockfd bind error!\r\n");
        return ;
    }

    ret = listen(server_sockfd, 10);
    if (ret != 0)
    {
        close(server_sockfd);
        printf("server_sockfd listen error!\r\n");
        return ;
    }

    sock_link = 1;
}

void *connect_task(void *arg)
{
    struct sockaddr_in remote_addr;
    int n;
    int res;
    int ret;
    int opt;
    int client_len;
    void *thread_result;
    pthread_t a_thread[10];

    client_len = sizeof(remote_addr);
    //printf("TcpConnect_Task---\r\n");
    if(sock_link == 1)
    {
        for(n=0;n<10;n++)
        {
            client_sockfd[n] = -1;
            while(client_sockfd[n] < 0)
            {
                client_sockfd[n] = accept(server_sockfd, (struct sockaddr *)&remote_addr, (socklen_t *)&client_len);
                /* printf("client_sockfd[%d]=%d\r\n",n,client_sockfd[n]); */
                msleep(100);
            }

            printf("client_sockfd[%d]=%d\r\n",n,client_sockfd[n]);
            opt = fcntl(client_sockfd[n], F_GETFL, 0);
            opt |= O_NONBLOCK;
            ret = fcntl(client_sockfd[n], F_SETFL, opt);

            res = pthread_create(&a_thread[n], NULL, sock_task, (void *)&n);
            if (res != 0)
            {
                perror("Thread a creation failed");
                exit(EXIT_FAILURE);
            }

            msleep(100);
        }
    }

    for(n=0;n<10;n++)
    {
        res = pthread_join(a_thread[n], &thread_result);
        if (res != 0) {
            perror("Thread  a join failed");
            exit(EXIT_FAILURE);
        }
    }

    for(;;)
    {
        //printf("\r\nTcpConnect_Task---\r\n");
        msleep(1000);
    }

    pthread_exit(NULL);
}

void *sock_task(void *arg)
{
    int i;
    int j;
    int recv_rx;
    int sock_id = *(int *)arg;
    char recv_buf[1024] = {0};
    printf("sock_task %d---------\r\n", sock_id);

    for(;;)
    {
        msleep(10);

        recv_rx = read(client_sockfd[sock_id], recv_buf, 1024);

        if (recv_rx > 0)
        {
            recv_buf[recv_rx] = 0;
            parse_json(recv_buf, sock_id);
            recv_rx = 0;
            printf("recv_buf = %s\r\n", recv_buf);
        }
        else if(recv_rx == 0)
        {
            close(client_sockfd[sock_id]);
            printf("client_sockfd[%d]=%d closed!\r\n", sock_id, client_sockfd[sock_id]);
            client_sockfd[sock_id] = -1;
            msleep(100);
        }

        memset(recv_buf, 0, 1024);

        if (recv_have == 1)
        {
            char *json_out = send_json();
            write(client_sockfd[sock_id], json_out, strlen(json_out));
            recv_have = 0;
            send_length = 0;
            memset(g_ucUARTBuffer, 0, 512);
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
                recv_have = 1;
                continue;
            }

            *(precv+send_length) = c;
            send_length++;
            msleep(10);
        }
        msleep(10);
    }

    pthread_exit(NULL);
}

void *heartbeat_task(void *arg)
{
    int n = 0;
    int count[10] = {0};
    printf("heartbeat_task -------\r\n");

    for (;;)
    {
        for (n = 0; n < 10; n++)
        {
            if (client_sockfd[n] > 0)
            {
                if (heartbeat[n] == 1)
                {
                    count[n] = 0;
                    heartbeat[n] = 0;
                    printf("heartbeat----------\r\n");
                }
                else if (heartbeat[n] == 0)
                {
                    if (count[n] > 5)
                    {
                        count[n] = 0;
                        close(client_sockfd[n]);
                        printf("heartbeat error----------\r\n");
                        client_sockfd[n] = -1;
                        msleep(100);
                    }
                    count[n]++;
                }
            }
        }

        sleep(100);
    }

    pthread_exit(NULL);
}

int main()
{
    int res;
    pthread_t b_thread;
    pthread_t c_thread;
    pthread_t d_thread;
    void *thread_result;

    sock_init();

    res = pthread_create(&b_thread, NULL, recv_task, NULL);
    if (res != 0) {
        perror("Thread b creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&c_thread, NULL, connect_task, NULL);
    if (res != 0) {
        perror("Thread c creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&d_thread, NULL, heartbeat_task, NULL);
    if (res != 0) {
        perror("Thread d creation failed");
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

