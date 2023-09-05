#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void swap(int before, int after);
bool cycle(int pairindex, int stop, int count);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

printf("ranks\n");
for (int l = 0; l < candidate_count; l++)
{
    printf("%d ", ranks[l]);
}
printf("\n");

        record_preferences(ranks);

printf("preferences\n");
for (int l = 0; l < candidate_count; l++)
{
    for(int m = 0; m < candidate_count; m++)
    {
        printf("%d ", preferences[l][m]);
    }
    printf("\n");
}

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for(int i = 0; i < candidate_count; i++)
    {
        if(strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    //invalid name
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for(int i = 0; i < candidate_count; i++)
    {
        for(int j = i + 1; j < candidate_count; j++)
        {
           preferences[ranks[i]][ranks[j]] += 1; 
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for(int i = 0; i < candidate_count; i++)
    {
        for(int j = i + 1; j < candidate_count; j++)
        {
            //more voters think i is better than j
            if(preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
            //more voters think j is better than i
            else if(preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
            //if preferences are similar don't record on array pairs
            else if(preferences[i][j] == preferences[j][i])
            {
                continue;
            }
        }
    }

for(int i = 0; i < pair_count; i++)
{
printf("pairs %d %d\n", pairs[i].winner, pairs[i].loser);
}

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int last = pair_count-1; //last place to search in bubble sort
    int swapcounter = 0; //count the number of swap
    do
    {
        swapcounter = 0;
        for (int i = 0; i <= last - 1; i++)
        {
            int diff1 = 0, diff2 = 0;
            diff1 = preferences[pairs[i].winner][pairs[i].loser];
            diff2 = preferences[pairs[i+1].winner][pairs[i+1].loser];
            if (diff1 < diff2)
            {
                swap(i, i+1);
                swapcounter += 1;
            }
            //if multiple pairs have the same strength of victory, order doesn't matter
            else
            {
                continue;
            }
        }

        last -= 1;
    }
    while (swapcounter != 0 || last <= -2);

printf("Sorted\n");
for(int i = 0; i < pair_count; i++)
{
    printf("pairs %d %d\n", pairs[i].winner, pairs[i].loser);
}
    return;
}

//swap elements in pairs array in decreasing order
void swap(int before, int after)
{
    //temporate value for swap
    int twinner = 0;
    int tloser = 0;

    twinner = pairs[before].winner;
    tloser = pairs[before].loser;

    //put after values in before
    pairs[before].winner = pairs[after].winner;
    pairs[before].loser = pairs[after].loser;

    //put before values in after
    pairs[after].winner = twinner;
    pairs[after].loser = tloser;

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int stopindex = -1; //stop index of pairs array
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        
 //       if(cycle() == true)
        {
            stopindex = i;
            break;
        }
    }
   
    //remove pairs[stopidex] node from locked graph

for (int i = 0; i < candidate_count; i++)
{
    for(int j = 0; j < candidate_count; j++)
    {
        printf("%d ", locked[i][j]);
    }
    printf("\n");
}
    return;
}

/*
//follow the arrows if it creates a cycle return true
int cycle(int curi, int stop, int count)
{
    //base case
    if(count == candidate_count && curj == stop)
    {
        return curi;
    }


    for(int j = 0; j < candidate_count; j++)
    {
        if (locked[curj][j] == true)
        {
            cycle(j, stop, count + 1);
        }
    }
    return -1;
    
}
*/

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

