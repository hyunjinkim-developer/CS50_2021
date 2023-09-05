// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);
    
    // Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);
    
    // Read samples from input file and write updated data to output file
    int16_t *buffer = malloc(sizeof(int16_t) * 1);

    while (1)
    {
        fread(buffer, sizeof(int16_t), 1, input);
        //check if it is End of File 
        //if it is end of file, feof() return value that is not zero
        if (feof(input))
        {
            break;
        }
        (*buffer) = factor * (*buffer);
        fwrite(buffer, sizeof(int16_t), 1, output);
            
    }

    //free memory allocation
    free(buffer);

    // Close files
    fclose(input);
    fclose(output);
}
