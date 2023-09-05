#include <stdio.h>
#include <cs50.h>
#include <math.h>

int divide(int numCoin, int coinType, int change);
int computeChange(int change, int numCoin, int coinType);

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

    divide(numQuarter, 25, change);
    computeChange(change, numQuarter, 25);

    divide(numDime, 10, change);
    computeChange(change, numDime, 10);

    divide(numNickel, 5, change);
    computeChange(change, numNickel, 5);

    divide(numPenny, 1, change);
    computeChange(change, numPenny, 1);

    //print the number of coins
    printf("%i\n", numQuarter + numDime + numNickel + numPenny);

}

//compute how many coins should I give
int divide(int numCoin, int coinType, int change)
{
    numCoin = change / coinType;
    return numCoin;
}

//compute remaining change
int computeChange(int change, int numCoin, int coinType)
{
    change -= numCoin * coinType;
    return change;
}
