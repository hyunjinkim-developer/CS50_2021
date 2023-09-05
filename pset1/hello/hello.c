#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //get name from user
    string name = get_string("What is your name?\n");
    //print hello, (name)
    printf("hello, %s\n", name);
}
