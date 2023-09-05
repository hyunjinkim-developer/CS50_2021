#include <stdio.h>

int fibo(int n)
{
    printf("func call param %d\n", n);

    if (n == 1)
        return 0;
    else if (n == 2)
        return 1;
    else
        return fibo(n-1) + fibo(n-2);
}

int main(void)
{
    fibo(7);
    return 0;
}
