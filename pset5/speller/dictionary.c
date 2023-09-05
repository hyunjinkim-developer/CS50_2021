// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Prototype
void print(void);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;

// Hash table
node *table[N];

// Number of words in dictionary
unsigned int SIZE = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int hashidx = hash(word);
    node *cursor = table[hashidx];
    
    // traversing linked list
    while (cursor != NULL)
    {
        if (strcasecmp(cursor -> word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor -> next;
        }
    }

    return false;
}

// Hashes word to a number
// This code is used for calculating hash codes of Strings in JAVA
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // treat uppercase same as lowercase
        int convert = word[i];
        if (65 <= word[i] && word[i] <= 90)
        {
            convert += 32;
        }
        hash = 31 * hash + convert;
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

    // Read strings from file
    char *dword = calloc(LENGTH + 1, sizeof(char));
    while (fscanf(dict, "%s", dword) != EOF)
    {
        node *new = calloc(1, sizeof(node));
        if (new == NULL)
        {
            printf("Could not read new word form dictionary.\n");
            return false;
        }
        
        //copy word into node, ends with '\0'
        int i = 0;
        while (dword[i] != '\0')
        {
            new -> word[i] = dword[i];
            i += 1;
        }

        // make hash table with words in dictionary
        unsigned int hashidx = hash(dword);
        new -> next = table[hashidx];
        table[hashidx] = new;
        // count number of words in dictionary that are loaded
        SIZE += 1;

        memset(dword, LENGTH + 1, sizeof(char));
    } // end of while loop
    free(dword);
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return SIZE;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    int tableidx = 0;
    int count = 0;
    while (tableidx < N)
    {
        node *cursor = table[tableidx];
        node *temp = NULL;
        while (cursor != NULL)
        {
            temp = cursor;
            cursor = cursor -> next;
            free(temp);
            count += 1;
        }
        tableidx += 1;
    }
    if (count == SIZE)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Print hash table
void print(void)
{
    int cur = 0, sum = 0;
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        
        cur = 0;
        printf("table %d\n", i);
        while (cursor != NULL)
        {
            printf("%s ", cursor -> word);
            cur += 1;
            cursor = cursor -> next;
        }
        sum += cur;
        printf("\n");
    }

    if (sum == SIZE)
    {
        printf("All words in dictionary are loaded\n");
    }
}
