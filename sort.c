#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void swap(int *a, int *b)
{
    int temp;

    temp = *a;
    *a = *b;
    *b = temp;
}

void insert_sort(int *a, size_t n)
{
    int i;
    int j;
    int temp;

    for (i = 0; i < n; i++)
    {
        temp = a[i];
        for (j = i; (j > 0) && (a[j-1] > temp); j--)
        {
            a[j] = a[j-1];
        }

        a[j] = temp;
    }
}

void bubble_sort(int *a, size_t n)
{
    int i;
    int j;

    for (i = 1; i < n; i++)
    {
        for (j = 0; j < n - i; j++)
        {
            if (a[j] > a[j+1])
            {
                swap(&a[j], &a[j+1]);
            }
        }
    }

    return ;
}

void select_sort(int *array, unsigned int n)
{
    int i;
    int j;

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j <= n; j++)
        {
            if (array[i] > array[j])
            {
                array[i] ^= array[j];
                array[j] ^= array[i];
                array[i] ^= array[j];
            }
        }
    }

    return ;
}

void quick_sort(int *a, size_t left, size_t right)
{
    if (left >= right)
        return ;

    int i = left;
    int j = right;
    int key = a[i];

    while (i < j)
    {
        while ((i < j) && (key <= a[j]))
            j--;
        a[i] = a[j];

        while ((i < j) && (key >= a[i]))
            i++;
        a[j] = a[i];
    }

    a[i] = key;
    quick_sort(a, left, i - 1);
    quick_sort(a, i + 1, right);
}

int ascii_to_integer(const char *str)
{
    int value = 0;

    while ((*str >= '0') && (*str <= '9'))
    {
        value *= 10;
        value += *str++ - '0';
    }

    if (*str != '\0')
        value = 0;

    return value;
}

char *strrev(char *str)
{
    char *p = str;
    char *q = str + strlen(str) - 1;

    if (!str || !*str)
        return NULL;

    for (; p < q; p++, q--)
    {
        *p ^= *q;
        *q ^= *p;
        *p ^= *q;
    }

    return str;
}

char *integer_to_ascii(int n, char *s, int b)
{
    const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int temp = n;
    char *str_temp = s;

    if (n < 0)
        n = -n;

    while (n > 0)
    {
        *s++ = digits[n % b];
        n /= b;
    }

    if (temp < 0)
        *s++ = '-';

    *s = '\0';

    return strrev(str_temp);
}

typedef void (*print_cb_t)(int n);

void printWelcome(int n)
{
    printf("Welcome %d", n);
    return ;
}

void printGoodbye(int n)
{
    printf("Goodbye %d", n);
    return ;
}

void callback(print_cb_t print, int n)
{
    print(n);
    printf("\r\n------------\r\n");
}

int main()
{
    int k[] = {9, 7, 1, 5, 3, 2};
    quick_sort(k, 0, 5);

    int i;
    for (i = 0; i < 6; i++)
        printf("%d ", k[i]);
    printf("\n");

    callback(printWelcome, 7);

    callback(printGoodbye, 9);

    exit(0);
}

