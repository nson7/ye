#include <stdio.h>
#include <stdlib.h>
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

print_cb_t a_print = NULL;

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

void callback(print_cb_t print, int n,...)
{
    print(n);
    printf("\r\n------------\r\n");
}

void hanoi(size_t n, char a, char b, char c)
{
    if (n > 0)
    {
        if (n == 1)
        {
            printf("%c -> %c\r\n", a, c);
        }
        else
        {
            hanoi(n-1, a, c, b);
            printf("%c -> %c\r\n", a, c);
            hanoi(n-1, b, a, c);
        }
    }
}

struct A
{
    int a:5;
    int b:3;
};

struct C
{
    char *str_name;
    unsigned n;
};

struct B
{
    struct C x;
    int y;
    char *b_name;
};

#define SWAP(a, b)    \
    do {              \
        a ^= b;           \
        b ^= a;           \
        a ^= b;           \
    } while(0)

int sn = 0;

void permutation(char s[], int b, int e)
{
    if( (0 <= b) && (b <= e) )
    {
        if( b == e )
        {
            printf("%s\n", s);
        }
        else
        {
            int i = 0;

            for(i=b; i<=e; i++)
            {
                char c = s[b];
                s[b] = s[i];
                s[i] = c;

                permutation(s, b+1, e);

                c = s[b];
                s[b] = s[i];
                s[i] = c;
            }
        }
    }
}

int n = 100;

int main()
{
    size_t m = 777;
    int n = 999;

    printf("n=%d\r\n", n);

    printf("---\r\n");


#define Smart
#if !defined(Smart)
    /* #ifdef Smart */
    /* #ifndef Smart */
    sn = 1;
#else
    sn = 7;
#endif
    printf("sn=%d\r\n", sn);
    /* printf("Smart=%d\r\n", Smart); */

#undef Smart

#if !defined(Smart)
    /* #ifdef Smart */
    /* #ifndef Smart */
    sn = 1;
#else
    sn = 7;
#endif

    printf("sn=%d\r\n", sn);

    a_print = printWelcome;
    if (a_print != NULL)
        a_print(7);

    a_print = printGoodbye;
    if (a_print != NULL)
        a_print(7);

    SWAP(m, n);
    printf("m=%d, n=%d\r\n", m, n);

    struct B b =
    {
        .x =
        {
            .str_name = "nson7--",
            .n = 77,
        },
        .y = 9,
        .b_name = "ye-----",
    };

    printf("\r\nstr=%s, n=%d, y=%d, name=%s\r\n", b.x.str_name, b.x.n, b.y, b.b_name);

    int k[] = {9, 7, 1, 5, 3, 2};
    quick_sort(k, 0, 5);

    char str[10] = "01234567";

    struct A d;
    memcpy(&d, str, sizeof(struct A));

    printf("a=%d\r\n", d.a);
    printf("b=%d\r\n", d.b);

    int i;
    for (i = 0; i < 6; i++)
        printf("%d ", k[i]);
    printf("\n");

    callback(printWelcome, 7);

    callback(printGoodbye, 9);

    struct hanoita
    {
        char data[5];
        short time[2];
    };

    printf("sizeof=%d\r\n", sizeof(struct hanoita));

    int *N = malloc(sizeof(int)*5);

    //hanoi(3, 'a', 'b', 'c');

    exit(0);
}

