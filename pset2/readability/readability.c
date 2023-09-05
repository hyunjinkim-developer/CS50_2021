#include <cs50.h>
#include <math.h>
#include <stdio.h>

int countl(char *input);
int counts(char *input);
int countw(char *input);
void count(char *input, int *lsw);
float indexL(int *lsw);
float indexS(int *lsw);
float computeindex(int *lsw);
int convertgrade(float index);
void grade(int *lsw);

int main(void)
{
    //input
    string input = get_string("Text: ");
    
    /*lsw[0]: # of letters
    lsw[1]: # of sentences
    lsw[2]: # of words*/
    int lsw[3];
    for(int i = 0; i < 3; i++)
    {
        lsw[i] = 0;
    }
    
    //count number of letters/ sentneces/ words/
    count(input, lsw);

    //compute Coleman-Liau index
    computeindex(lsw);

    //calculate and print grade
    grade(lsw);

    return 0;
}
//count number of leters
int countl(char *input)
{
    int numletter = 0;

    int i = 0;
    while(input[i] != '\0')
    {
        //A ~ Z
        if(input[i] >= 65 && input[i] <= 90) 
        {
            numletter += 1;
        }
        //a ~ z
        else if(input[i] >= 97 && input[i] <= 122)
        {
            numletter += 1;
        }
        //else 
        //read next character
        
        i += 1;
    }

    return numletter;
}

//count numver of sentences
int counts(char *input)
{
    int numsentence = 0;
    
    int i = 0;
    while(input[i] != '\0')
    {
        //count as 1 sentence if ./ !/ ?/ appears
        //ASCII value: .(46)/ !(33)/ ?(63)
        if(input[i] == 46 || input[i] == 33 || input[i] == 63)
        {
            numsentence += 1;
        }
        //else
        //read next character
        i += 1;
    }

    return numsentence;
}

//count numver of word
int countw(char *input)
{
    int numword = 0;

    int i = 0;
    while(input[i] != '\0')
    {
        //sequence of characters before space(' ') is 1 word
        if(input[i] == ' ')
        {
            numword += 1;
        }
        //else
        //read next character
        i += 1;
    }
    //sequence of characters before nul('\0') is 1 word
    if(input[i] == '\0')
    {
        numword += 1;
    }

    return numword;
}

//count number of letters/ words/ sentences/
void count(char *input, int *lsw)
{
    lsw[0] = countl(input);
    lsw[1] = counts(input);
    lsw[2] = countw(input);
}

/*
//10 to the power of n
int power(int n)
{
    int pow = 1;
    
    for(int i = 0; i < n; i++)
    {
        pow *= 10;
    }

    return pow;
}

//round off to nth decimal places
float rounddec(float before, int nth)
{
    //calculate (n+1)th decimal place value
    int temp = before * power(nth + 1);
    //f it needs to be rounded up in (n+1)th decimal place 
    if((temp % 10) >= 5)
    {
        temp /= 10;
        temp += 1;
    }
    //else
    //just remove (n+1)th decimal digit
    else
    {
        temp /= 10;
    }
    //reverse into original decimal format
    float after = (float) temp / power(nth); 
    
    return after;
}
*/

//calculate L value in index formula
float indexL(int *lsw)
{
    float indexL = (float) lsw[0] / lsw[2] * 100;
    
    return indexL;
}

//calculate S value index formula
float indexS(int *lsw)
{
   float indexS = (float) lsw[1] / lsw[2] * 100;
   return indexS;
}

//compute Coleman-Liau index
float computeindex(int *lsw)
{
    float L = indexL(lsw);
    float S = indexS(lsw);
    float index = 0.0588 * L - 0.296 * S - 15.8;
    
    return index;
}

//convert index into grade round off index in fisrt decimal place
int convertgrade(float index)
{
    int grade = round(index);

    return grade;
}

//calculate and print grade
void grade(int *lsw)
{
    //caculate Coleman-Liau index
    float index = computeindex(lsw);
    //convert index into grade
    int grade = convertgrade(index);

    //print grade
    if(grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if(grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}
