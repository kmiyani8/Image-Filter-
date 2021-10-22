#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00;

            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //Using similar logic to swap, as we're swapping pixels on the left with the pixels on the right and we're doing this using a temporary variable
            int tmpred = image[i][j].rgbtRed;
            int tmpgreen = image[i][j].rgbtGreen;
            int tmpblue = image[i][j].rgbtBlue;

            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            image[i][width - j - 1].rgbtRed = tmpred;
            image[i][width - j - 1].rgbtGreen = tmpgreen;
            image[i][width - j - 1].rgbtBlue = tmpblue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sumBlue = 0;
            float sumRed = 0;
            float sumGreen = 0;
            int counter = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        continue; // will move onto next iteration in the for loop
                    }

                    sumBlue += image[i + k][j + l].rgbtBlue;
                    sumRed += image[i + k][j + l].rgbtRed;
                    sumGreen += image[i + k][j + l].rgbtGreen;

                    counter++;
                }
            }

            temp[i][j].rgbtBlue = round(sumBlue / counter);
            temp[i][j].rgbtGreen = round(sumGreen / counter);
            temp[i][j].rgbtRed = round(sumRed / counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
        }
    }
    return;
}


// Detect edges
int cap(int value)
{
    return value < 255 ? value : 255;
}
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE temp[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int new_GxB, new_GxR, new_GxG, new_GyB, new_GyG, new_GyR;

            new_GxB = new_GxR = new_GxG = new_GyB = new_GyG = new_GyR = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }
                    int weightX = Gx[k + 1][l + 1];
                    new_GxB += image[i + k][j + l].rgbtBlue * weightX;
                    new_GxR += image[i + k][j + l].rgbtRed * weightX;
                    new_GxG += image[i + k][j + l].rgbtGreen * weightX;

                    int weightY = Gy[k + 1][l + 1];
                    new_GyB += image[i + k][j + l].rgbtBlue * weightY;
                    new_GyG += image[i + k][j + l].rgbtGreen * weightY;
                    new_GyR += image[i + k][j + l].rgbtRed * weightY;

                }
            }

            temp[i][j].rgbtRed = cap(round(sqrt((new_GxR * new_GxR) + (new_GyR * new_GyR))));

            temp[i][j].rgbtGreen = cap(round(sqrt((new_GxG * new_GxG) + (new_GyG * new_GyG))));

            temp[i][j].rgbtBlue = cap(round(sqrt((new_GxB * new_GxB) + (new_GyB * new_GyB))));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
        }
    }
    return;
}
