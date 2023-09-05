// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // Allocate memory for new person
    person *newp = malloc(sizeof(person));
    if (newp == NULL)
    {
        printf("Something went wrong!");
    }
    // Generation with parent data
    if (generations > 1)
    {
        // Recursively create blood type histories for parents
        person *par1 = create_family(generations - 1);
        person *par2 = create_family(generations - 1);
        if (par1 == NULL || par2 == NULL)
        {
            printf("Something went wrong!");
        }

        newp -> parents[0] = par1;
        newp -> parents[1] = par2;
        // Randomly assign child alleles based on parents
        int random1 = rand() % 2;
        int random2 = rand() % 2;
        newp -> alleles[0] = par1 -> alleles[random1];
        newp -> alleles[1] = par2 -> alleles[random2];
    }

    // Generation without parent data
    else
    {
        // Set parent pointers to NULL
        newp -> parents[0] = NULL;
        newp -> parents[1] = NULL;

        // Randomly assign alleles
        newp -> alleles[0] = random_allele();
        newp -> alleles[1] = random_allele();
    }

    // Return newly created person
    return newp;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // Handle base case
    if (p == NULL)
    {
        return ;
    }

    // Free parents
    person *child1 = p -> parents[0];
    person *child2 = p -> parents[1];
    free_family(child1);
    free_family(child2);

    // Free child
    free(p);
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
