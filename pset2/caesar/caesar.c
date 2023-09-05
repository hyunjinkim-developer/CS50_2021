#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkdigit(char *argv);
int argcheck(int argc, char **argv);
int cipher(int character,  int key);
void rotate(char *plaintext, int key);


int main(int argc, char **argv)
{
    //check argument
    int validarg = argcheck(argc, argv);
    //if argument is not valid
    if(validarg == 1)
    {
        return 1;
    }
    
    //argument is valid
    //convert key value from string to int
    int key = atoi(argv[1]);

    //prompt for plaintext
    string plaintext = get_string("plaintext:  ");

    //cipher plain text to ciphertext
    rotate(plaintext, key);

    //print ciphertext
    printf("ciphertext: %s\n", plaintext);

    return 0;
}

//check if argv[1](key) is non-negative integer
//return 0/ if not, return 1
int checkdigit(char *argv) 
{

    for(int index = 0, n = strlen(argv); index < n; index++)
    {
        //check if character of argument is digit
        if(argv[index] >= 48 && argv[index] <= 57)
        {
            continue;
        }
        //if character of argument is not digit
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    return 0;
}

//check whether argument is valid
//if it is valid return 0
//if not, return 1
int argcheck(int argc, char **argv)
{
    //program run
    //with one comand-line argument 
    if(argc == 2)
    {
        //check if argv[1] is non-negative integer
        //if key is non-integer value, return 0
        //if not, return 1
        return checkdigit(argv[1]);
    }
    //program run
    //without any command-line argument
    //of with more than one command-line argument
    else 
    {
        //print ERROR message
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

//convert plaintext to ciphertext
int cipher(int character,  int key)
{
    return (character + key) % 26;
}

//read palintext and convert to ciphertext
void rotate(char *plaintext, int key)
{
    for(int index = 0, leng = strlen(plaintext); index < leng; index++)
    {
        int character = plaintext[index];

        //if character is uppercase
        if(character >= 65 && character <= 90)
        {
            character -= 65;
            plaintext[index] = cipher(character, key) + 65;
        }
        //if character is lowercase
        else if(character >= 97 && character <= 122)
        {
            character -= 97;
            plaintext[index] = cipher(character, key) + 97;
        }
        //if charcter is non-alphabetical characters
        else
        {
            continue;
        }
    }
}
