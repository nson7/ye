#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *v_memcpy(void *v_dest, const void *v_src, unsigned int n)
{
    char *dest = v_dest;
    const *src = v_src;

    while(n--)
        *dest++ = *src++;

    return v_dest;
}

void *v_memset(void *s, int c, unsigned int n)
{
    char *tmp = s;

    while(n--)
        *tmp++ = c;

    return s;
}

int v_strlen(const char *str)
{
    int len = 0;

    while(*str++ != '\0')
        len++;

    return len;
}

int p_strlen(const char *str)
{
    const char *tmp = str;

    for(tmp = str; *tmp != '\0'; ++tmp);

    return tmp-str;
}

char *v_strcpy(char *v_dest, const char *v_src)
{
    const char *src = v_src;
    char *dest = v_dest;

    while((*dest++ = *src++) != '\0');

    return v_dest;
}

char *v_strncpy(char *v_dest, const char *v_src, unsigned int count)
{
    char *dest = v_dest;
    const char *src = v_src;

    while(((count--) && (*dest++ = *src++) != '\0'));

    return v_dest;
}

void *v_memmove(void *dest, void *src, size_t count)
{
    char *tmp = dest;
    const char *s = src;

    if (dest <= src) {
        while (count--)
            *tmp++ = *s++;
    }
    else {
        tmp += count;
        src += count;
        while (count--)
            *--tmp = *--s;
    }
    return dest;
}

int main()
{
    const char *mStr = "memory and string";
    char tStr[30];
    char nStr[50] = "just do it";
    unsigned int n = 5;
    int i;

    for (i = 0; i < 10; i++) {
        printf("n=%ud\n", n--);
    }
    v_strcpy(tStr, mStr);
    v_strncpy(nStr, mStr, 6);
    printf("len = %d\n", v_strlen(mStr));
    printf("tStr = %s\n", tStr);
    printf("nStr = %s\n", nStr);

    exit(0);
}

