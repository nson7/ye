#include<stdio.h>
#include<math.h>

#define MAXNUM 5

void shellInsert(int array[], int n, int dk)
{
    int i;
    int j;
    int temp;

    for (i = dk; i < n; i++)
    {
        temp = array[i];
        for (j = i - dk; (j >= i % dk) && array[j] > temp; j -= dk)
            array[j + dk] = array[j];
        if (j != i - dk)
            array[j + dk] = temp;
    }
}

int dkHibbard(int t,int k)
{
    return (int)(pow(2, t - k + 1) - 1);
}

void shellSort(int array[], int n, int t)
{
    int i;

    for (i = 1; i <= t; i++)
        shellInsert(array, n, dkHibbard(t, i));
}

void main()
{
    int i;
    int array[MAXNUM];

    for (i = 0; i < MAXNUM; i++)
        scanf("%d", &array[i]);

    shellSort(array, MAXNUM, (int)(log(MAXNUM+1)/log(2)));
    for(i = 0; i < MAXNUM; i++)
        printf("%d ", array[i]);

    printf("\n");
}

