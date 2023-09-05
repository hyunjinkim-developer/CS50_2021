#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
// prototype
void fname(FILE** img, int *fnum);

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

    int filenum = 0; // file number
    int *fnum = &filenum;
    char *filename = NULL; //name of new image file
    FILE *img = NULL;
    fname(&img, fnum); //current file
    
    // read memory card to EOF 
    do 
    {
        // read 512 Bytes from memory card
        BYTE *buffer = calloc(512, sizeof(BYTE));
        fread(buffer, sizeof(BYTE), 512, file);

printf("! \n");
        // check if it is first 4 Bytes of JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
printf("!! \n");
            // close previous file
            if(filenum > 0)
            {
                fclose(img);
            }
            
            // create new file
            fname(&img, fnum);
            fwrite(buffer, sizeof(BYTE), 512, img);
            // if it is not the start of new JPEG
            /*else
            {
                fwrite(buffer, sizeof(BYTE), 512, img);
            }*/
        }
        // if it is not JPEG, read next 512 Bytes

    free(buffer);
    }
    while(feof(file) != 0);

    fclose(file);
    fclose(img);

    return 0;
}

//make new image file
void fname(FILE** img, int *fnum)
{
    char filename[7];
    
printf("%d\n", *fnum);
    sprintf(filename, "%03i.jpg", ++(*fnum));
printf("%s\n", filename);
    *img = fopen(filename, "w");
    if(img == NULL)
    {
        printf("Something wrong with making new image file\n");
    }
printf("%d\n", *fnum);
}
