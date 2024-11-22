#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int height = 32;
int width = 100;
int grid[32][100];
// 0: 空白
// 1: セル
// 2: 残像
// 3: 移動済みセル　を表す

void setupGrid(int grid[height][width])
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == height / 2 && j == width / 2)
            {
                grid[i][j] = 3;
            }
            else if (i % 10 == 0 && j % 10 == 0)
            {
                grid[i][j] = 3;
            }
            else
            {
                grid[i][j] = 0;
            }
        }
    }
}

// void attack(int i, int j)
// {
// }

void move(int i, int j)
{
    switch (rand() % 4)
    {
    case 0:
        if (i == height - 1)
        {
            break;
        }
        grid[i][j] = 2;
        grid[i + 1][j] = 3;
        break;
    case 1:
        if (i == 0)
        {
            break;
        }
        grid[i][j] = 2;
        grid[i - 1][j] = 3;
        break;
    case 2:
        if (j == width - 1)
        {
            break;
        }
        grid[i][j] = 2;
        grid[i][j + 1] = 3;
        break;
    case 3:
        if (j == 0)
        {
            break;
        }
        grid[i][j] = 2;
        grid[i][j - 1] = 3;
        break;
    default:
        break;
    }
}

void updateGrid(int grid[height][width])
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j] == 2)
            {
                grid[i][j] = 0;
            }
            else if (grid[i][j] == 3)
            {
                grid[i][j] = 1;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j] == 1)
            {
                move(i, j);
            }
        }
    }
}

void printGrid(int grid)
{
    switch (grid)
    {
    case 2:
        printf("\x1b[90m%c\x1b[0m", '@');
        break;
    case 3:
        putchar('@');
        break;
    default:
        putchar(' ');
        break;
    }
}

int main()
{
    printf("\x1b[2J"); // ターミナルをクリア
    setupGrid(grid);
    int i, j;
    while (1)
    {
        printf("\x1b[H"); // カーソルを左上端に
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                printGrid(grid[i][j]);
            }
            printf("\n");
        }
        usleep(100000);
        updateGrid(grid);
    }
    return 0;
}