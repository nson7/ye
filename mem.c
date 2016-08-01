#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *v_memcpy(void *dest, const void *src, size_t count)
{
    char *tmp_dest = dest;
    const char *tmp_src = src;

    while (count--)
        *tmp_dest = *tmp_src;

    return dest;
}

void *v_memset(void *s, int c, size_t count)
{
    char *tmp = s;

    while (count--)
        *tmp++ = c;

    return s;
}

int v_strlen(const char *str)
{
    int ret = 0;

    while (*str++ != '\0')
        ret++;

    return ret;
}

int p_strlen(const char *str)
{
    const char *tmp;

    for (tmp = str; *tmp != '\0'; tmp++)
        /* nothing */;

    return (tmp - str);
}

char *v_strcpy(char *dest, const char *src)
{
    char *tmp_dest = dest;
    const char *tmp_src = src;

    while ((*tmp_dest++ = *tmp_src++) != '\0')
        /* nothing */;

    return dest;
}

char *strncpy(char *dest, const char *src, size_t count)
{
    char *tmp_dest = dest;
    const char *tmp_src = src;

    while ((count--) && ((*tmp_dest = *tmp_src) != '\0'))
        /* nothing */;

    return dest;
}

void *v_memmove(void *dest, const void *src, size_t count)
{
    char *tmp_dest = dest;
    const char *tmp_src = src;

    if (tmp_dest <= tmp_src)
    {
        while (count--)
            *tmp_dest++ = *tmp_src;
    }
    else
    {
        tmp_dest += count;
        tmp_src += count;
        while (count--)
            *(--tmp_dest) = *(--tmp_src);
    }

    return dest;
}

int v_memcmp(const void *s1, const void *s2, size_t count)
{
    const unsigned char *tmp_s1 = s1;
    const unsigned char *tmp_s2 = s2;
    int ret = 0;

    for (; count > 0; count--, tmp_s1++, tmp_s2++)
    {
        if ((ret = *tmp_s1 - *tmp_s2) != 0)
            break;
    }

    return ret;
}

void *v_memscan(void *src, int c, size_t count)
{
    unsigned char *tmp_src = src;

    while (count) {
        if (*tmp_src == c)
            return (void *)tmp_src;
        tmp_src++;
        count--;
    }
    return (void *)tmp_src;
}

void *v_memchr(const void *src, int c, size_t count)
{
    const unsigned char *tmp_src = src;

    while (count-- != 0) {
        if ((unsigned char)c == *tmp_src++)
            return (void *)(tmp_src - 1);
    }
    return NULL;
}

char *v_strcat(char *dest, const char *src)
{
    char *tmp = dest;

    while (*dest)
        dest++;
    while ((*dest++ = *src++) != '\0')
        /* nothing */;
    return tmp;
}

char *v_strncat(char *dest, const char *src, size_t count)
{
    char *tmp = dest;

    while (*dest)
        dest++;
    while ((count--) && ((*dest++ = *src++) != '\0'))
        /* nothing */;
    *dest = '\0';
    return tmp;
}

int v_strcmp(const char *s1, const char *s2)
{
    while (1) {
        if (*s1 != *s2)
            return *s1 > *s2 ? 1 : -1;
        if (*s1 == '\0')
            break;
        s1++;
        s2++;
    }
    return 0;
}

int v_strncmp(const char *s1, const char *s2, size_t count)
{
    unsigned char tmp1;
    unsigned char tmp2;

    while (count--) {
        tmp1 = *s1++;
        tmp2 = *s2++;
        if (tmp1 != tmp2)
            return tmp1 > tmp2 ? 1 : -1;
        if (tmp1 == '\0')
            break;
    }
    return 0;
}

char *v_strchr(const char *str, int c)
{
    for (; *str != (char)c; str++)
        if (*str == '\0')
            return NULL;
    return (char *)str;
}

char *v_strrchr(const char *str, int c)
{
    const char *tmp_str = str + strlen(str);

    do {
        if (*tmp_str == (char)c)
            return (char *)tmp_str;
    } while (--tmp_str >= str);
    return NULL;
}

size_t v_strlcpy(char *dest, const char *src, size_t count)
{
    size_t ret = strlen(src);

    if (count)
    {
        size_t len = ((ret >= count) ? count-1 : ret);
        memcpy(dest, src, len);
        dest[len] = '\0';
    }

    return ret;
}

char *v_strstr(const char *str1, const char *str2)
{
    size_t len1;
    size_t len2;

    len2 = strlen(str2);
    if (!len2)
        return (char *)str1;

    len1 = strlen(str1);
    while (len1 >= len2)
    {
        if (!memcmp(str1, str2, len2))
            return (char *)str1;

        len1--;
        str1++;
    }

    return NULL;
}

int main()
{
    const char *mStr = "memory and string";
    const char *tmpStr = "and";
    char tStr[70];
    char nStr[50] = "just do it";

    printf("str = %s\n", v_strstr(mStr, tmpStr));

    exit(0);
}

