#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HEIGHT 31
#define WIDTH 99

int height = HEIGHT;
int width = WIDTH;

struct Cell
{
    int hp;
    int state;
};

struct Cell grid[HEIGHT][WIDTH];
// 0: 空白
// 1: セル
// 2: 残像
// 3: 移動済みセル　を表す

void spawnCell(int i, int j)
{
    grid[i][j].hp = 100 + rand() % 100;
    grid[i][j].state = 3;
}

int canMove(int i, int j)
{
    if (i < 0 || height - 1 < i || j < 0 || width - 1 < j)
    {
        return 0;
    }
    else if (grid[i][j].state > 0)
    {
        return 0;
    }
    return 1;
}

void setupGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == height / 2 && j == width / 2)
            {
                spawnCell(i, j);
            }
            else if (i % 10 == 0 && j % 10 == 0)
            {
                spawnCell(i, j);
            }
            else
            {
                grid[i][j].hp = 0;
                grid[i][j].state = 0;
            }
        }
    }
}

void resetCell(int i, int j)
{
    grid[i][j].hp = 0;
    grid[i][j].state = 2;
}

void move(int i, int j)
{
    grid[i][j].hp--;

    // fall through
    switch (rand() % 5)
    {
    case 0:
        if (canMove(i + 1, j))
        {
            grid[i + 1][j] = grid[i][j];
            grid[i + 1][j].state = 3;
            resetCell(i, j);
            break;
        }
    case 1:
        if (canMove(i - 1, j))
        {
            grid[i - 1][j] = grid[i][j];
            grid[i - 1][j].state = 3;
            resetCell(i, j);
            break;
        }
    case 2:
        if (canMove(i, j + 1))
        {
            grid[i][j + 1] = grid[i][j];
            grid[i][j + 1].state = 3;
            resetCell(i, j);
            break;
        }
    case 3:
        if (canMove(i, j - 1))
        {
            grid[i][j - 1] = grid[i][j];
            grid[i][j - 1].state = 3;
            resetCell(i, j);
            break;
        }
    default:
        grid[i][j].state = 3;
        break;
    }
}

void updateGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j].state == 2)
            {
                grid[i][j].state = 0;
            }
            else if (grid[i][j].state == 3)
            {
                grid[i][j].state = 1;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j].state == 1)
            {
                move(i, j);
            }
        }
    }
}

void printGrid(int state)
{
    switch (state)
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
    setupGrid();
    int i, j;
    while (1)
    {
        printf("\x1b[H"); // カーソルを左上端に
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                printGrid(grid[i][j].state);

                // 死亡のテスト
                if (grid[i][j].hp == 0)
                {
                    grid[i][j].state = 0;
                }
            }
            printf("\n");
        }
        usleep(100000);
        updateGrid();
    }
    return 0;
}