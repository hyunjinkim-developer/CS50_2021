#include <stdio.h>
#include <cs50.h>

int findlength(long cardNum);
long power(int digit);
long findstartnum(long cardNum, int length);
int checkvalid(long cardNum, int length);
int finddigit(long cardNum,int nth);
int evenchecksum(long cardNum, int length);
int oddchecksum(long cardNum, int length);
int checksumvalidity(long cardNum, int length);
void valid(long cardNum, int length);

int main(void)
{
    //get creditcard number from user
    long cardNum = 0;
    cardNum = get_long("Number: ");

    //find number of digit(length) of creditcard number
    int length = findlength(cardNum);

    //check card type and validity
    //print the type of card if it is valid
    valid(cardNum, length);

}

//make 10 to the power of digit
long power(int digit)
{
    long result = 1;

    while(digit != 0)
    {
        result *= 10;
        digit -= 1;
    }

    return result;
}

//find number of digit(length) of creditcard number
int findlength(long cardNum)
{
    int maxLength = 16;
    while(maxLength != 0)
    {
        if(cardNum/power(maxLength-1) != 0)
        {
            break;
        }
        maxLength--;
    }

    return maxLength;
}

//find two digit from the left of creditcard number
long findstartnum(long cardNum, int length)
{
    //if cardNum less than 100 then print INVALID
    if(cardNum < 10)
    {
        return cardNum;
    }
    else
    {
        return cardNum / power(length-2);
    }
}

//check whether cardNum is AMEX or MASTERCARD or VISA or INVAILD 
//return value 0:AMEX/ 1:MASTERCARD/ 2:VISA/ -1:INVALI
int checkvalid(long cardNum, int length)
{
    int cardId = (int) findstartnum(cardNum, length);

//?condition statement using switch?
    if(length == 13)
    {
        if(cardId/10 == 4)
        {
            //VISA: length is 13/ start with 4
            return 2;
        }
        else
        {
            return -1;
        }
    }
    else if(length == 15)
    {
        //AMEX: length is 15/ start with 34, 37
        if(cardId == 34 || cardId == 37)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else if(length == 16)
    {
        //MASTERCARD: length is 16/ start with 51, 52, 53,54,55
        if(cardId == 51 || cardId ==  52 || cardId ==  53 || cardId ==  54 || cardId ==  55)
        {
            return 1;
        }
        //VISA: length is 16/ start with 4
        else if(cardId/10 == 4)
        {
            return 2;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

//find nth digit 
int finddigit(long cardNum,int nth)
{
    int nthdigit = 0;
    long temp = 0;
    
    //remove left of nth digit
    temp = cardNum % power(nth);
    //remove right of nth digit
    nthdigit = temp / power(nth - 1);

    return nthdigit;
}

//multiply every other digits form second to last(every even digit starting from right) by 2 and then add those products(if multiplied value is bigger than 10, add both digits separately)
int evenchecksum(long cardNum, int length)
{
    int evenchecksum = 0;
    int index = 2;
    int nthdigit = 0;

    while(index <= length)
    {
        //nthdigit multiply by 2 
        nthdigit = finddigit(cardNum, index) *  2; 
        //if nthdigit is bigger than 10, add digits both separately
        if(nthdigit >= 10)
        {
            evenchecksum += (nthdigit / 10);
            evenchecksum += (nthdigit % 10);
        }
        else
        {
            evenchecksum += nthdigit;
        }
        index += 2;
    }
    return evenchecksum;
}

//add every other digit starting from first to last (every odd digit start from right)
int oddchecksum(long cardNum, int length)
{
    int oddchecksum = 0;
    int index = 1;
    int nthdigit = 0;

    while(index <= length)
    {
        nthdigit = finddigit(cardNum, index);
        oddchecksum += nthdigit; 
        index += 2;
    }
    return oddchecksum;
}

//check whether checksum value is valid
//if checksum value is valid return 0
//else return -1
int checksumvalidity(long cardNum, int length)
{
    int validity = 1; //default of return value is 1
    int checksum = oddchecksum(cardNum, length) + evenchecksum(cardNum, length); 
    
    //last digit of sum of checksum is 0: VALID
    if(checksum % 10 == 0)
    {
        validity = 0;
    }
    //else -1: INVALID
    else
    {
        validity = -1;
    }

    return validity;
}

//check card number is valid
//cardtype return value:
//-1 : INVALID
//0: AMEX
//1: MASTERCARD
//2: VISA
void valid(long cardNum, int length)
{
    int checksum = checksumvalidity(cardNum, length);
    int cardtype = checkvalid(cardNum, length);

    //ERROR handling
    if(cardtype > 2 || cardtype < 0 || checksum == 1)
    {
       printf("Something wrong with your code!\n");
    }
    //this card is INVALID
    if(checksum == -1 || cardtype == -1)
    {
        printf("INVALID\n");
    }
    //if checksum == 0: checksum is valid value
    else
    {
        if(cardtype == 0)
        {
            printf("AMEX\n");
        }
        else if(cardtype == 1)
        {
            printf("MASTERCARD\n");
        }
        else if(cardtype == 2)
        {
            printf("VISA\n");
        }
    }
}


