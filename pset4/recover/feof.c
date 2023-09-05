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
    char filename[7]; // name of new image file
    FILE *img = NULL; // new image file
    int flag = 1; // save 0 while reading in the  middle of JPEG file otherwise save 1 
    
    BYTE *buffer = calloc(512, sizeof(BYTE));
    // read memory card to EOF 
    while(1)
    {
printf("1\n");
        // read 512 Bytes from memory card
        fread(buffer, sizeof(BYTE), 512, file);

printf("2\n");
        // first 4 Bytes of buffer indicate JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
printf("3\n");
            flag = 0;
            // close previous file
            if(fnum > 0)
            {
                fclose(img);
            }
            
            // open new image file
            sprintf(filename, "%03i.jpg", fnum);
printf("4\n");
            img = fopen(filename, "w");
            if(img == NULL)
            {
                printf("Something wrong with making new image file\n"); 
                return 1;
            }
            fnum += 1;
            
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
        //while reading int the middle of JPEG
        else if(flag == 0)
        {
printf("5\n");
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
        //haven't met JPEG contents yet
        //read next 512 Bytes

        if(feof(file) != 0)
        {
printf("7\n");
            break;
        }
printf("6\n");
    } 
printf("8\n");

    free(buffer);

    fclose(file);
    fclose(img);

    return 0;
}

