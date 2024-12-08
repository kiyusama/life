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
    int maxHp;
    // 0: 空白, 1: 残像, 2: 死体, 3: セル を表す
    int state;
    int movedFlag;
    int age;
};

struct Cell grid[HEIGHT][WIDTH];
int turnFlag = 1;

void spawnCell(int i, int j)
{
    grid[i][j].hp = 100 + rand() % 100;
    grid[i][j].maxHp = grid[i][j].hp;
    grid[i][j].state = 3;
    grid[i][j].movedFlag = turnFlag;
    grid[i][j].age = rand() % 10;
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
                grid[i][j].age = 0;
            }
        }
    }
}

void resetCell(int i, int j)
{
    grid[i][j].hp = 0;
    grid[i][j].state = 1;
    grid[i][j].movedFlag = 0;
    grid[i][j].age = 0;
}

void eatCell(int i, int j, int iNext, int jNext)
{
    grid[i][j].hp += grid[i][j].maxHp;
}

void attackCell(int i, int j, int iNext, int jNext)
{
    grid[iNext][jNext].hp -= grid[i][j].maxHp / 10;
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
        if (grid[iNext][jNext].state == 3)
        {
            attackCell(i, j, iNext, jNext);
        }
        else if (grid[iNext][jNext].state == 2)
        {
            eatCell(i, j, iNext, jNext);
        }
        grid[iNext][jNext] = grid[i][j];
        resetCell(i, j);
        grid[iNext][jNext].movedFlag *= -1;
    }
    else
    {
        grid[i][j].movedFlag *= -1;
    }
}

void splitCell(int i, int j)
{
    if (canMove(i, j - 1))
    {
        spawnCell(i, j - 1);
        grid[i][j].hp = grid[i][j].hp * 2 / 3;
        grid[i][j - 1].hp = grid[i][j - 1].hp * 2 / 3;
    }
}

void updateGrid()
{
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            grid[i][j].age++;

            if ((grid[i][j].hp <= 0 || grid[i][j].age > 100) && grid[i][j].state == 3) // 死亡判定
            {
                grid[i][j].state = 2;
                grid[i][j].hp = 0;
                grid[i][j].movedFlag = 0;
                grid[i][j].age = 0;
            }

            if (turnFlag == grid[i][j].movedFlag)
            {
                move(i, j);
            }
            else if (grid[i][j].state == 1) // 残像の場合は空白へ
            {
                grid[i][j].state = 0;
            }

            if (grid[i][j].age % 40 == 0 && grid[i][j].state == 3) // 生殖のテスト
            {
                splitCell(i, j);
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
            }
            printf("\n");
        }
        usleep(100000);
        updateGrid();
    }
    return 0;
}