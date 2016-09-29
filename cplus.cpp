
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class A
{
    public :
        void max(int *i, int *j);
        void show();
};

void A::max(int *i, int *j)
{
    *i ^= *j;
    *j ^= *i;
    *i ^= *j;

    return ;
}

void A::show()
{
    printf("A-----\r\n");
}

class B : public A
{
    public :
        void max(int *i, int *j);
};

void B::max(int *i, int *j)
{
    A::max(i, j);
}

class C
{
    public :
        void show();
};

void C::show()
{
    printf("C------\r\n");
}

class D : public A, public C
{
};

int n = 100;

int main()
{
    int *i = (int *)malloc(sizeof(int)*5);

    int n = 5;
    int z = 7;

    class B b;
    b.max(&n,&z);
    printf("n=%d, z=%d\r\n", n, z);

    char str[100] = "string-----";
    char *tempstr = (char *)malloc(sizeof(char) * 200);

    strcpy(tempstr, str);
    cout << "cout<<=" << tempstr << "\r\n";
    cout << ::n << "\r\n";
    cout << n << "\r\n";
    printf("n=%d\r\n", ::n);
        return 0;
}

