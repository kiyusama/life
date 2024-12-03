#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// 画面サイズに合わせて変更
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
// 1: 残像
// 2: 死体
// 3: 通常セル
// 4: 移動後セル を表す

void spawnCell(int i, int j)
{
    grid[i][j].hp = 100 + rand() % 100;
    grid[i][j].state = 3;
}

void setupGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i % 10 == 0 && j % 10 == 0)
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
    grid[i][j].state = 1;
}

// 食べる
void eatCell(int i, int j)
{
    grid[i][j].state = 0;
}

int canMove(int iNext, int jNext)
{
    if (iNext < 0 || height - 1 < iNext || jNext < 0 || width - 1 < jNext)
    {
        return 0;
    }
    else if (grid[iNext][jNext].state >= 3)
    {
        return 0;
    }
    return 1;
}

void move(int i, int j)
{
    grid[i][j].hp--;

    int iNext, jNext;
    switch (rand() % 10)
    {
    case 0:
        iNext = i + 1;
        jNext = j;
        break;
    case 1:
        iNext = i - 1;
        jNext = j;
        break;
    case 2:
        iNext = i;
        jNext = j + 1;
        break;
    case 3:
        iNext = i;
        jNext = j - 1;
        break;
    default:
        iNext = i; // 移動しない場合はcanMoveがfalse
        jNext = j; // hpが減るだけ
        break;
    }
    if (canMove(iNext, jNext))
    {
        grid[iNext][jNext] = grid[i][j];
        grid[iNext][jNext].state = 4;
        resetCell(i, j);
    }
}

void updateGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j].state == 1) // 残像の場合は空白へ
            {
                grid[i][j].state = 0;
            }
            else if (grid[i][j].state == 4) // 移動後フラグを解除
            {
                grid[i][j].state = 3;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (grid[i][j].state == 3)
            {
                move(i, j);
            }
        }
    }
}

void printGrid(int i, int j)
{
    switch (grid[i][j].state)
    {
    case 1: // 残像
        printf("\x1b[90m%c\x1b[0m", '@');
        break;
    case 2: // 死体
        printf("\x1b[91m%c\x1b[0m", '@');
        break;
    case 3: // 通常セル
    case 4: // 移動後セル
        // putchar('@');
        printf("%i", grid[i][j].hp % 10);
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
                printGrid(i, j);
                // 死亡のテスト
                if (grid[i][j].hp == 0 && grid[i][j].state >= 3)
                {
                    grid[i][j].state = 2;
                }
            }
            printf("\n");
        }
        usleep(100000);
        updateGrid();
    }
    return 0;
}