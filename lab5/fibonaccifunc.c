#include <stdio.h>

int Fibo(int n)
{
        printf("%d\n", n);
    if (n == 1)
        return 0;
    else if (n == 2)
        return 1;
    else
    {
        // function is called
        // right after Fibo(n-1) is stated
        // not after the return statement ends
        return Fibo(n-1) + Fibo(n-2);
    }
    printf("!\n");
}

int main(void)
{
    printf("result: %d\n", Fibo(7));
    /*
    for (int i = 1; i < 15; i++)
    {
        printf("fibonaccifunc(%d) =", i);
        printf("= %d\n", Fibo(i));
    }
    */
}
