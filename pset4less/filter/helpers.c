#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            //average of red, green, blue
            BYTE grayscale = round((red + green + blue) / 3.0);

            image[i][j].rgbtBlue = grayscale;
            image[i][j].rgbtGreen = grayscale;
            image[i][j].rgbtRed = grayscale;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            //using sepia algorithm
            WORD sepiaRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            WORD sepiaGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            WORD sepiaBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);

            //if converted value is bigger than 8bits, cap at 255 (otherwise, byte overflow)
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtBlue = (BYTE) sepiaBlue;
            image[i][j].rgbtGreen = (BYTE) sepiaGreen;
            image[i][j].rgbtRed = (BYTE) sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE *temp = calloc(width, sizeof(RGBTRIPLE));

    for (int i = 0; i < height; i++)
    {
        //save image row length of width
        for (int j = 0; j < width; j++)
        {
            temp[j] = image[i][j];
        }

        //reflect horizontally
        for (int j = 0; j < width; j++)
        {
            int index = width - 1 - j;

            image[i][index].rgbtBlue = temp[j].rgbtBlue;
            image[i][index].rgbtGreen = temp[j].rgbtGreen;
            image[i][index].rgbtRed = temp[j].rgbtRed;
        }
    }

    free(temp);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*t)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    //copy original image before saving changes into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            t[i][j] = image[i][j];
        }
    }

    //important notice! i: height, j: width
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //array of pixel postion from left to right, top to bottom
            RGBTRIPLE(*pos) = calloc(9, sizeof(RGBTRIPLE));
            /* ul u ur   i-1,j-1    i-1, j   i-1, j+1
             *  l c  r   i, j-1     i, j     i, j+1
             * dl d dr   i+1, j-1   i+1, j   i+1, j+1
             */

            float numpixel = 0; //number of pixels added

            //center position:
            //top left corner
            if (i == 0 && j == 0)
            {
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];
                pos[7] = t[i + 1][j];
                pos[8] = t[i + 1][j + 1];

                numpixel = 4;
            }
            //top middle (not left or right corner)
            else if (i == 0 && j != 0 && j != width - 1)
            {
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];
                pos[6] = t[i + 1][j - 1];
                pos[7] = t[i + 1][j];
                pos[8] = t[i + 1][j + 1];

                numpixel = 6;
            }
            //top right corner
            else if (i == 0 && j == width - 1)
            {
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[6] = t[i + 1][j - 1];
                pos[7] = t[i + 1][j];

                numpixel = 4;
            }
            //left middle (not top or bottom corner)
            else if (i != 0 && i != height - 1 && j == 0)
            {
                pos[1] = t[i - 1][j];
                pos[2] = t[i - 1][j + 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];
                pos[7] = t[i + 1][j];
                pos[8] = t[i + 1][j + 1];

                numpixel = 6;
            }
            //right midle (not top or bottom corner)
            else if (i != 0 && i != height - 1 && j == width - 1)
            {
                pos[0] = t[i - 1][j - 1];
                pos[1] = t[i - 1][j];
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[6] = t[i + 1][j - 1];
                pos[7] = t[i + 1][j];

                numpixel = 6;
            }
            //bottom left corner
            else if (i == height - 1 && j == 0)
            {
                pos[1] = t[i - 1][j];
                pos[2] = t[i - 1][j + 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];

                numpixel = 4;
            }
            //bottom middle (not bottom left or right corner)
            else if (i == height - 1 && j != 0 && j != width - 1)
            {
                pos[0] = t[i - 1][j - 1];
                pos[1] = t[i - 1][j];
                pos[2] = t[i - 1][j + 1];
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];

                numpixel = 6;
            }
            //bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                pos[0] = t[i - 1][j - 1];
                pos[1] = t[i - 1][j];
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];

                numpixel = 4;
            }
            //center position is not in edges
            else
            {
                pos[0] = t[i - 1][j - 1];
                pos[1] = t[i - 1][j];
                pos[2] = t[i - 1][j + 1];
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];
                pos[6] = t[i + 1][j - 1];
                pos[7] = t[i + 1][j];
                pos[8] = t[i + 1][j + 1];

                numpixel = 9;
            }

            //calculate average of pixels around center
            WORD blurR = 0, blurG = 0, blurB = 0;

            //add rgbtRed, rgbtGreen, rgbtBlue seperately
            for (int posi = 0; posi < 9; posi++)
            {
                blurR += pos[posi].rgbtRed;
                blurG += pos[posi].rgbtGreen;
                blurB += pos[posi].rgbtBlue;
            }

            //calculate average seperately then round to the nearest integer value
            blurR = round(blurR / numpixel);
            blurG = round(blurG / numpixel);
            blurB = round(blurB / numpixel);

            //if converted value is bigger than 8bits, cap at 255 (otherwise, byte overflow)
            if (blurR > 255)
            {
                blurR = 255;
            }
            if (blurG > 255)
            {
                blurG = 255;
            }
            if (blurB > 255)
            {
                blurB = 255;
            }

            //save calculated value into original image array
            image[i][j].rgbtBlue = (BYTE) blurB;
            image[i][j].rgbtGreen = (BYTE) blurG;
            image[i][j].rgbtRed = (BYTE) blurR;

            free(pos);
        }
    }

    free(t);

    return;
}
