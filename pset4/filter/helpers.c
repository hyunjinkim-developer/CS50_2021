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

// reflect image horizontally 
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

// blur image 
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
                pos[3] = t[i][j + 1];
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
            WORD blurr = 0, blurg = 0, blurb = 0;
            
            //add rgbtRed, rgbtGreen, rgbtBlue seperately
            for (int posi = 0; posi < 9; posi++)
            {
                blurr += pos[posi].rgbtRed;
                blurg += pos[posi].rgbtGreen;
                blurb += pos[posi].rgbtBlue;
            }

            //calculate average seperately then round to the nearest integer value
            blurr = round(blurr / numpixel);
            blurg = round(blurg / numpixel);
            blurb = round(blurb / numpixel);
        
            //if converted value is bigger than 8bits, cap at 255 (otherwise, BYTE overflow)
            if (blurr > 255)
            {
                blurr = 255;
            }
            if (blurg > 255)
            {
                blurg = 255;
            }
            if (blurb > 255)
            {
                blurb = 255;
            }
            
            //save calculated value into original image array
            image[i][j].rgbtBlue = (BYTE) blurb;
            image[i][j].rgbtGreen = (BYTE) blurg;
            image[i][j].rgbtRed = (BYTE) blurr;

            free(pos);
        }
    }

    free(t);
    
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // save image pixels temporarily
    RGBTRIPLE(*t)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            t[i][j] = image[i][j];
        }
    }

    // sobal operator in right to left, top to bottom order
    int gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // save surrounding pixels position based on current location
            RGBTRIPLE(*pos) = calloc(9, sizeof(RGBTRIPLE));
            // check if each of position is occupied
            // for convenience when multiplying each pixel with cernel
            int occupy[9] = {0};
            if (i == 0 && j == 0)
            {
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];
                pos[7] = t[i + 1][j];
                pos[8] = t[i + 1][j + 1];

                occupy[4] = 1;
                occupy[5] = 1;
                occupy[7] = 1;
                occupy[8] = 1;
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

                occupy[3] = 1;
                occupy[4] = 1;
                occupy[5] = 1;
                occupy[6] = 1;
                occupy[7] = 1;
                occupy[8] = 1;
            }
            //top right corner
            else if (i == 0 && j == width - 1)
            {
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                pos[6] = t[i + 1][j - 1];
                pos[7] = t[i + 1][j];

                occupy[3] = 1;
                occupy[4] = 1;
                occupy[6] = 1;
                occupy[7] = 1;
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

                occupy[1] = 1;
                occupy[2] = 1;
                occupy[4] = 1;
                occupy[5] = 1;
                occupy[7] = 1;
                occupy[8] = 1;
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

                occupy[0] = 1;
                occupy[1] = 1;
                occupy[3] = 1;
                occupy[4] = 1;
                occupy[6] = 1;
                occupy[7] = 1;
            }
            //bottom left corner
            else if (i == height - 1 && j == 0)
            {
                pos[1] = t[i - 1][j];
                pos[2] = t[i - 1][j + 1];
                pos[4] = t[i][j];
                pos[5] = t[i][j + 1];

                occupy[1] = 1;
                occupy[2] = 1;
                occupy[4] = 1;
                occupy[5] = 1;
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

                occupy[0] = 1;
                occupy[1] = 1;
                occupy[2] = 1;
                occupy[3] = 1;
                occupy[4] = 1;
                occupy[5] = 1;
            }
            //bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                pos[0] = t[i - 1][j - 1];
                pos[1] = t[i - 1][j];
                pos[3] = t[i][j - 1];
                pos[4] = t[i][j];
                
                occupy[0] = 1;
                occupy[1] = 1;
                occupy[3] = 1;
                occupy[4] = 1;
            }
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

                occupy[0] = 1;
                occupy[1] = 1;
                occupy[2] = 1;
                occupy[3] = 1;
                occupy[4] = 1;
                occupy[5] = 1;
                occupy[6] = 1;
                occupy[7] = 1;
                occupy[8] = 1;
            } 

            // rgbtBlue, rgbtGreen, rgbtRed computed with Gx
            double gxB = 0, gxG = 0, gxR = 0;
            // rgbtBlue, rgbtGreen, rgbtRed computed with Gy
            double gyB = 0, gyG = 0, gyR = 0;
            // output using Sobel operator 
            int outB = 0, outG = 0, outR = 0;

            // calculate Sobel operator
            for (int idx = 0; idx < 9; idx++)
            {
                //if current location is in edges, 
                //pretend there is a 1 pixel solid black border around the edge of the image
                if (occupy[idx] == 0)
                {
                    continue;
                }

                // mulitply corresponding pixel Gx kernel
                gxB += pos[idx].rgbtBlue * gx[idx];
                gxG += pos[idx].rgbtGreen * gx[idx];
                gxR += pos[idx].rgbtRed * gx[idx];
                
                // mulitply corresponding pixel Gy kernel
                gyB += pos[idx].rgbtBlue * gy[idx];
                gyG += pos[idx].rgbtGreen * gy[idx];
                gyR += pos[idx].rgbtRed * gy[idx];
            }
        
            // calculate square root of Gx^2 + Gy^2
            outB = round(sqrt(pow(gxB, 2) + pow(gyB, 2)));
            outG = round(sqrt(pow(gxG, 2) + pow(gyG, 2)));
            outR = round(sqrt(pow(gxR, 2) + pow(gyR, 2)));

            // if calculated value exceeds 255 should be capped (otherwise BYTE overflow)
            if (outB > 255)
            {
                outB = 255;
            }
            if (outG > 255)
            {
                outG = 255;
            }
            if (outR > 255)
            {
                outR = 255;
            }
           
            //assign calculated value into original image array
            image[i][j].rgbtBlue = (BYTE) outB;
            image[i][j].rgbtGreen = (BYTE) outG;
            image[i][j].rgbtRed = (BYTE) outR;
       
            free(pos);
        }
    }

    free(t);
    return;
}
