#include <stdio.h>

int bsearch(int * arr, int first, int last, int target)
{
    if (first > last)
    {
        return -1;
    }

    int mid = (first + last) / 2;
    if (arr[mid] == target)
    {
        return mid;
    }
    else if(target < arr[mid])
    {
        return bsearch(arr, first, mid-1, target);
    }
    else 
    {
        return bsearch(arr, mid+1, last, target);
    }
}

int main(void)
{
    int arr[] = {1, 3, 5, 7, 9};
    int idx;
    idx = bsearch(arr, 0, sizeof(arr)/sizeof(int)-1, 7);
    if (idx == -1) 
    {
        printf("Not Found\n");
    }
    else
    {
        printf("Target's Location: %d\n", idx);
    }

    idx = bsearch(arr, 0, sizeof(arr)/sizeof(int)-1, 4);
    if(idx == -1)
    {
        printf("Not Found\n");
    }
    else
    {
        printf("Target's Location: %d\n", idx);
    }

    return 0;
}
