#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    //get amount of change in dollar that I owed from the user
    float changeDollar = 0;
    do
    {
        changeDollar = get_float("Change owed: ");
    }
    while (changeDollar < 0);

    //convert dollar into cent
    int change = round(changeDollar * 100);

    //computing coins
    int numPenny = 0, numNickel = 0, numDime = 0, numQuarter = 0;

    numQuarter = change / 25;
    change -= numQuarter * 25;

    numDime = change / 10;
    change -= numDime * 10;

    numNickel = change / 5;
    change -= numNickel * 5;

    numPenny = change / 1;
    change -= numPenny * 1;

    //print the number of coins
    printf("%i\n", numQuarter + numDime + numNickel + numPenny);

}