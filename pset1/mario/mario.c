#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = 0;

    //prompt should be number between 1 to 8
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    //loop for every level
    for (int i = 0; i < height; i++)
    {
        //print ' '
        for (int l = 0; l < height - i - 1; l++)
        {
            printf(" ");
        }
        //print #
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        //print 2 spaces
        printf("  ");
        //print #
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        //move to nextline
        printf("\n");
    }

    return 0;
}
