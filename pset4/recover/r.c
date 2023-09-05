#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // remind the user of correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    // open memory card file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("This memory card cannot opened for reading.\n");
        return 1;
    }

    int fnum = 0; // file number
    char filename[8]; // name of new image file
    FILE *img = NULL; // new image file
    int flag = 1; // save 0 while reading in the  middle of JPEG file otherwise save 1 
    BYTE *buffer = calloc(512, sizeof(BYTE));

    // read memory card to EOF 
    // read 512 Bytes from memory card
    while(fread(buffer, sizeof(BYTE), 512, file))
    {
        // first 4 Bytes of buffer indicate JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            flag = 0;
            // close previous file
            if(fnum > 0)
            {
                fclose(img);
            }
            
            printf("%d\n", fnum);
            // using memory allocation for sprintf
            // open new image file
            sprintf(filename, "%03i.jpg", fnum);
            img = fopen(filename, "w");
            if(img == NULL)
            {
                printf("Something wrong with making new image file\n"); 
                return 1;
            }
            fnum = fnum + 1; 
            
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
        // while reading int the middle of JPEG
        else if(flag == 0)
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
        // haven't met JPEG contents yet
        // read next 512 Bytes
        else if (flag == 1)
        {
            continue;
        }

    } // end of while loop 

    free(buffer);
    fclose(file);
    fclose(img);

    return 0;
}

