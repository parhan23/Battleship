#include "main.h"

static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void printGreet(void)
{
    printfCenter("Battleship");
    printf("\n\n");
    printfCenter("Press [ENTER] to continue");
    getchar();
}

void printPrevScore(Score *prevSc)
{
    const int DIST = -10;                   //Distance between header and value
    CONSOLE_CURSOR_INFO structCursorInfo;

    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);

    eatline;
    system("cls");
    printfCenter("Your previous score");
    printf("\n\n");
    printfCenter("%*s %s\n", DIST, "Name:", prevSc->name);
    printfCenter("%*s %d\n", DIST, "Shots:", prevSc->shots);
    printfCenter("%*s %d\n", DIST, "Hits:", prevSc->hits);

    printf("\n");
    printfCenter("To continue press [ENTER]");
    getchar();
}

void printMainGrid(char grid[][MAX_GRID], int shipsOnField[], int mode, Coords lastShot)
{
    const int DIST = 10;  //Distance between grid and ships icons

    printf("\n");
    printf("  ");
    for (int i = 0; i < MAX_GRID; i++)
        printf("%c ", 'a' + i);
    printf("%*s", DIST, "");

    if (mode == PLACEMENT)
        printf("# Avaliable Ships\n");
    else
        printf("Your ships\n");

    for (int i = 0; i < MAX_GRID; i++)
    {
        printf("%2d", i + 1);
        for (int j = 0; j < MAX_GRID; j++)
        {
            if (mode == BATTLE && lastShot.x != -1 &&
                i == lastShot.y && j == lastShot.x)
            {
                SetConsoleTextAttribute(hConsole, (WORD)((LightRed << 4) | White));
                printf("%c%c", grid[i][j], grid[i][j]);
                SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
            }
            else if (grid[i][j] == HIT_CELL)
            {
                SetConsoleTextAttribute(hConsole, (WORD)((Black << 4) | White));
                printf("%c%c", grid[i][j], grid[i][j]);
                SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
            }
            else
                printf("%c%c", grid[i][j], grid[i][j]);
        }

        switch (i)
        {
        case 2:
            printShipIcon(4, shipsOnField, DIST, mode);
            break;
        case 4:
            printShipIcon(3, shipsOnField, DIST, mode);
            break;
        case 6:
            printShipIcon(2, shipsOnField, DIST, mode);
            break;
        case 8:
            printShipIcon(1, shipsOnField, DIST, mode);
            break;
        }
        printf("\n");
    }
}

void printAuxiliaryGrid(char grid[][MAX_GRID], Coords ptr, int shipsOnField[])
{
    const int DIST = 10;  //Distance between grid and ships icons

    printf("\n\n");
    printf("  ");
    for (int i = 0; i < MAX_GRID; i++)
        printf("%c ", 'a' + i);

    printf("%*s", DIST, "");
    printf("Enemy ships\n");

    for (int i = 0; i < MAX_GRID; i++)
    {
        printf("%2d", i + 1);
        for (int j = 0; j < MAX_GRID; j++)
        {
            if (i == ptr.y && j == ptr.x)
            {
                SetConsoleTextAttribute(hConsole, (WORD)((Green << 4) | White));
                printf("%c%c", grid[i][j], grid[i][j]);
                SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
            }
            else if (grid[i][j] == HIT_CELL)
            {
                SetConsoleTextAttribute(hConsole, (WORD)((Black << 4) | White));
                printf("%c%c", grid[i][j], grid[i][j]);
                SetConsoleTextAttribute(hConsole, (WORD)((White << 4) | Black));
            }
            else
                printf("%c%c", grid[i][j], grid[i][j]);
        }

        switch (i)
        {
        case 2:
            printShipIcon(4, shipsOnField, DIST, BATTLE);
            break;
        case 4:
            printShipIcon(3, shipsOnField, DIST, BATTLE);
            break;
        case 6:
            printShipIcon(2, shipsOnField, DIST, BATTLE);
            break;
        case 8:
            printShipIcon(1, shipsOnField, DIST, BATTLE);
            break;
        }
        printf("\n");
    }
}

void printShipIcon(int shipDecks, int shipsOnField[], int DIST, int mode)
{
    printf("%*s", DIST, "");

    if (mode == PLACEMENT)
        printf("%d ", shipDecks);

    for (int i = shipDecks - 1; i >= 0; i--)
        printf("%c%c", SHIP_CELL, SHIP_CELL);
    printf("%*s", 2 * (MAX_SHIP_TYPES - shipDecks), "");

    if (mode == PLACEMENT)
        printf(" x%d", MAX_SHIP_TYPES - shipsOnField[shipDecks - 1] - shipDecks + 1);
    else
        printf(" x%d", shipsOnField[shipDecks - 1]);
}

void printResult(int winner)
{
    system("cls");
    printf("\n\n");
    if (winner == PLAYER_WINS)
        printfCenter("YOU ARE WINNER!")
    else
        printfCenter("You lose");
    printf("\n\n");
}