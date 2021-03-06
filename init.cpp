#include "main.h"

void getName(char *pname)
{    
    system("cls");
    printf("\n");
    printfCenter("Enter your name");
    printf("\n\n> ");
    fgetString(stdin, pname, MAX_PNAME);
}

int fgetString(FILE *stream, char *str, int length)
{
    int i = 0;
    char letter;

    for (i = 0; i < length - 1; i++)
    {
        if (fscanf(stream, "%c", &letter) == EOF)
            return EOF;

        if (letter == '\n')
            break;

        if (stream != stdin && isdigit(letter))
        {
            i--;
            fseek(stream, -2, SEEK_CUR);
            break;
        }
        
        str[i] = letter;
    }
    str[i] = '\0';

    return i;
}

Scores * initScores(char playerName[])
{
    Scores *scPtr = (Scores *)malloc(sizeof(Scores));
    scPtr->curr = (Score *)malloc(sizeof(Score));
    scPtr->prev = (Score *)malloc(sizeof(Score));
    
    strcpy(scPtr->curr->name, playerName);
    scPtr->curr->shots = 0;
    scPtr->curr->hits = 0;

    scPtr->prev->shots = 0;
    scPtr->prev->hits = 0;

    return scPtr;
}

Grids * initGrids(void)
{
    Grids * gridsPtr = (Grids *)malloc(sizeof(Grids));
    
    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
        {
            gridsPtr->mainGrid[i][j] = EMPTY_CELL;
            gridsPtr->auxGrid[i][j] = EMPTY_CELL;
            gridsPtr->computer[i][j] = EMPTY_CELL;
        }

    return gridsPtr;
}

FILE * getFilePath(char fname[])
{
    FILE *fp;
    char cmd;

    system("cls");
    printfCenter("Do you want to use file for statistics? (y/n)");
    do
        cmd = _getch();
    while (cmd != 'y' && cmd != 'n');

    if (cmd == 'n')
        return NULL;

    system("cls");
    printfCenter("1. New file   \n");
    printfCenter("2. Existing file");
    do
        cmd = _getch();
    while (cmd < '1' || cmd > '2');

    do
    {
        system("cls");
        printfCenter("Loading previous results\n\n");

        if (cmd == '1')
            printfCenter("Enter new file name")
        else
            printfCenter("Enter file name")

            printf("\n\n> ");
        scanf("%s", fname);

        if (cmd == '1')
            fp = fopen(fname, "wb+");
        else
            fp = fopen(fname, "rb");
    } while (!fp);

    return fp;
}

bool getPrevScore(FILE *fp, char *targetName, Score *prevScore)
{
    char nameFromFile[MAX_PNAME];
    Score scoreBuffer;

    while (fread(&scoreBuffer, sizeof(Score), 1, fp))
        if (strcmp(scoreBuffer.name, targetName) == 0)
        {
            *prevScore = scoreBuffer;
            fclose(fp);
            return true;
        }
    fclose(fp);

    return false;
}

char getPlacementMode(void)
{
    char cmd;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;

    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
    
    system("cls");
    while (true)
    {
        printfCenter("Select placement mode\n");
        printf("\n");
        printfCenter("1. Manual\n");
        printfCenter("2. Random\n");

        do
            cmd = _getch();
        while (cmd < '1' || cmd > '2');

        return cmd;
    }
}

Ships * initShipsOnField(void)
{
    Ships *shipsPtr = (Ships *)malloc(sizeof(Ships));

    for (int i = 0; i < MAX_SHIP_TYPES; i++)
    {
        shipsPtr->player[i] = 0;
        shipsPtr->computer[i] = 0;
    }
    
    return shipsPtr;
}

int getShip(int *shipsOnField)
{
    int shipNum;

    printf("\n\nSelect ship to place by its number\n");
    do
        shipNum = _getch() - '1' + 1;
    while (shipNum < 1 || shipNum > 4);
    
    return shipNum;
}

void initSubGrid(char grid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
            grid[i][j] = EMPTY_CELL;
}

void placeFormByDefault(int shipDecks, char grid[][MAX_GRID])
{
    for (int i = 0; i < shipDecks; i++)
        grid[MAX_GRID / 2][MAX_GRID / 2 - shipDecks / 2 + i] = PLACING_SHIP_CELL;
}