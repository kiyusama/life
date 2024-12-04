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
    int movedFlag;
};

struct Cell grid[HEIGHT][WIDTH];
int turnFlag = 1;

// 0: 空白
// 1: 残像
// 2: 死体
// 3: セル を表す

void spawnCell(int i, int j)
{
    grid[i][j].hp = 100 + rand() % 100;
    grid[i][j].state = 3;
    grid[i][j].movedFlag = turnFlag;
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
                grid[i][j].movedFlag = 0;
            }
        }
    }
}

void resetCell(int i, int j)
{
    grid[i][j].hp = 0;
    grid[i][j].state = 1;
    grid[i][j].movedFlag = 0;
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
    else if (grid[iNext][jNext].state == 3)
    {
        return 0;
    }
    return 1;
}

void move(int i, int j)
{
    grid[i][j].hp--;

    int iNext = i, jNext = j;
    switch (rand() % 10)
    {
    case 0:
        iNext++;
        break;
    case 1:
        iNext--;
        break;
    case 2:
        jNext++;
        break;
    case 3:
        jNext--;
        break;
    default:   // 移動しない場合はcanMoveがfalse
        break; // hpが減るだけ
    }
    if (canMove(iNext, jNext))
    {
        grid[iNext][jNext] = grid[i][j];
        resetCell(i, j);
        grid[iNext][jNext].movedFlag *= -1;
    }
    else
    {
        grid[i][j].movedFlag *= -1;
    }
}

void updateGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (turnFlag == grid[i][j].movedFlag)
            {
                move(i, j);
            }
            else if (grid[i][j].state == 1) // 残像の場合は空白へ
            {
                grid[i][j].state = 0;
            }
        }
    }
    turnFlag *= -1;
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
    case 3: // セル
        putchar('@');
        // printf("%i", grid[i][j].hp % 10);
        // printf("%i", grid[i][j].movedFlag % 10);
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
                if (grid[i][j].hp <= 0 && grid[i][j].state == 3)
                {
                    grid[i][j].state = 2;
                    grid[i][j].movedFlag = 0;
                }
            }
            printf("\n");
        }
        usleep(100000);
        updateGrid();
    }
    return 0;
}