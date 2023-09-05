#include <cs50.h>
#include <stdio.h>
#include <string.h>

int checkargnum(int argc);
int numkey(char *argv);
void convertkey(int cap, int current, int index, int *key);
int alphabetkey(char *argv, int *key);
int checkkey(int argc, char *argv, int *key);
void cipher(char *plaintext, int *key);

int main(int argc, char **argv)
{
    int key[26] = {0};

    //get key using a single command-line argument
    if(checkkey(argc, argv[1], key) == 1)
    {
        return 1;
    }

    //get plaintext
    string plaintext = get_string("plaintext:  ");

    //convert palintext to ciphertext
    cipher(plaintext, key);

    //print ciphertext
    printf("ciphertext: %s\n", plaintext);

    return 0;
}

//check if command-line argument is valid
//return 0
//if not, return 1
int checkargnum(int argc)
{
    //if is not a single command-line argument
    if(argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else
        return 0;
}

//check nnumber of key, whether key contain 26 characters
//return 0
//if not, return 1
int numkey(char *argv)
{
    if(strlen(argv) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
        return 0;
}

//convert key to not case-sensitive
void convertkey(int cap, int current, int index, int *key)
{
    //if current alphabet is uppercase
    if(cap == 0)
    {
        key[index] = current - 65;
    }
    //if current alphabet is lowercase
    else if(cap == 1)
    {
        key[index] = current - 97;
    }
}

//check if key is alphabet and key contain each letter exactly once then convert key value to non case-sensitive in key array
//return 0
//if not, return 1
int alphabetkey(char *argv, int *key)
{
    for(int index = 0; index < 26; index++)
    {
        int cur = argv[index]; //current letter of argument key
        int cap = -1; //if current letter is uppercase: 0/ lowercase: 1

        //check if current letter is alphabet
        if(cur >= 65 && cur <= 90)
        {
            cap = 0;
        }
        else if(cur >= 97 && cur <= 122)
        {
            cap = 1;
        }
        //if current letter is not an alphabet
        else
        {
            printf("There is non alphabet letter in the key.\n");
            return 1;
        }

        //check if current alphabet is already exist
        for(int j = 0; j < index; j++)
        {
            if(cur == argv[j])
            {
                printf("Key must contain each letter exactly once.\n");
                return 1;
            }
        }
        
        //convert key to non case-sensitive
        convertkey(cap, cur, index, key);
    }

    return 0;
}

//check if key is valid and store key in array
//return 0
//if not, return 1
int checkkey(int argc, char *argv, int *key)
{
    if(checkargnum(argc) == 1)
    {
        return 1;
    }
    else
    {
        if(numkey(argv) == 1)
        {
            return 1;
        }
        else
        {
            if(alphabetkey(argv, key))
            {
                return 1;
            }
            else return 0;
        }
    }
}

//convert plaintext to ciphertext
void cipher(char *plaintext, int *key)
{
    for(int index = 0, length = strlen(plaintext); index < length; index++)
    {
        int cur = plaintext[index]; //current letter
        //current letter is an uppercase
        if(cur >= 65 && cur <= 90)
        {
            plaintext[index] = key[cur - 65] + 65;
        }
        //current letter is an lowercase
        else if(cur >= 97 && cur <= 122)
        {
            plaintext[index] = key[cur - 97] + 97;
        }
        //current letter is not an alphabet
        else
        {
            continue;
        }
    }
}
