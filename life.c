#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int height = 32;
int width = 100;
char grid[32][100];
int frameCount = 0;

void makeGrid(char grid[height][width])
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == height / 2 && j == width / 2)
            {
                grid[i][j] = '@';
            }
            else
            {
                grid[i][j] = ' ';
            }
        }
    }
}

void move(int i, int j)
{
    switch (rand() % 5)
    {
    case 0:
        if (i == height - 1)
        {
            break;
        }
        grid[i][j] = ' ';
        grid[i + 1][j] = '@';
        break;
    case 1:
        if (i == 0)
        {
            break;
        }
        grid[i][j] = ' ';
        grid[i - 1][j] = '@';
        break;
    case 2:
        if (j == width - 1)
        {
            break;
        }
        grid[i][j] = ' ';
        grid[i][j + 1] = '@';
        break;
    case 3:
        if (j == 0)
        {
            break;
        }
        grid[i][j] = ' ';
        grid[i][j - 1] = '@';
        break;
    default:
        break;
    }
}

void updateGrid(char grid[height][width])
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j] == '@')
            {
                move(i, j);
            }
        }
    }
}

int main()
{
    printf("\x1b[2J"); // ターミナルをクリア
    makeGrid(grid);
    int i, j;
    while (1)
    {
        printf("\x1b[H"); // カーソルを左上端に
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                putchar(grid[i][j]);
            }
            printf("\n");
        }
        usleep(50000);
        updateGrid(grid);
        frameCount++;
    }
    return 0;
}