#include <stdio.h>
#include <stdlib.h>

void quick_sort(int *a, int left, int right);
void bubble_sort(int *a, int count);
void insert_sort(int*array, unsigned int n);
void select_sort(int *array, unsigned int n);

int main()
{
    int i;
    int k[] = {9, 7, 7, 5, 3, 2};

    select_sort(k, 6);

    for (i = 0; i < 6; i++)
        printf("%d ", k[i]);
    printf("\n");

    exit(0);
}

void quick_sort(int *a, int left, int right)
{
    if (left >= right)
    {
        return ;
    }

    int n;
    int i = left;
    int j = right;
    int key = a[left];

    while (i < j)
    {
        while ((i < j) && (key <= a[j]))
        {
            j--;
        }

        a[i] = a[j];
        while ((i < j) && (key >= a[i]))
        {
            i++;
        }

        a[j] = a[i];
    }

    a[i] = key;
    quick_sort(a, left, i - 1);
    quick_sort(a, i + 1, right);
}

void bubble_sort(int *a, int count)
{
    int i;
    int j;

    for (j = 1; j < count; j++)
    {
        for (i = 0; i < count - j; i++)
        {
            if (a[i] > a[i+1])
            {
                a[i] ^= a[i+1];
                a[i+1] ^= a[i];
                a[i] ^= a[i+1];
            }
        }
    }

    return ;
}

void insert_sort(int *array, unsigned int n)
{
    int i;
    int j;
    int temp;

    for (i = 1; i < n; i++)
    {
        temp = *(array + i);
        for (j = i; (j > 0) && (*(array + j -1) > temp); j--)
        {
            *(array + j) = *(array + j -1);
        }

        *(array + j) = temp;
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

