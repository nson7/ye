
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class A
{
    public :
        void swap(int *i, int *j);
        void show();
        virtual void sum(int i, int j);
};

void A::swap(int *i, int *j)
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

void A::sum(int i, int j)
{
    printf("sum=%d\r\n", i-j);
}

class B
{
    public :
        void show();
        void sum(int i, int j);
};

void B::show()
{
    printf("B-----\r\n");
}

void B::sum(int i, int j)
{
    printf("sum=%d\r\n", i+j);
}

class C : public A, public B
{
    friend void callback();
    public :
        int common;
        void show();
        void sum(int i, int j);

    private :
        char c;
        int value;
};

void callback()
{
    C c;
    c.value = 100;
    c.c = 'n';
    printf("value=%d, c=%c\r\n", c.value, c.c);
}

void C::show()
{
    A::show();
}

void C::sum(int i, int j)
{
    A::sum(i, j);
}

int n = 100;

class D
{
    public :
        D(int i);
        ~D();
        void show();
    private :
        int value;

};

D::D(int i)
{
    value = i;
    printf("crate D ----\r\n");
}

D::~D()
{
    printf("delete D------\r\n");
}

void D::show()
{
    printf("value=%d\r\n", value);
}

int main()
{
    int n = 5;
    int z = 7;

    D *d = new D(11);
    d->show();
    delete d;

    C c;
    C *pc;
    c.show();
    c.swap(&n, &z);
    callback();
    c.common = 11;
    c.sum(3, 2);
    c.B::sum(3, 2);

    printf("n=%d, z=%d c=%d\r\n", n, z, c.common);

    return 0;
}

