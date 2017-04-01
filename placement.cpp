#include "main.h"

void performManualPlacement(char grid[][MAX_GRID], int shipsOnField[])
{
    int shipNum;
    //Coords shipCoords;
    char subGrid[MAX_GRID][MAX_GRID];     //Layer for one ship movements
    char outputGrid[MAX_GRID][MAX_GRID];  //Combined grid
    Coords notUsedPtr = {-1};

    do
    {
        system("cls");
        printMainGrid(grid, shipsOnField, PLACEMENT, notUsedPtr);
        shipNum = getShip(shipsOnField);
        initSubGrid(subGrid);
        placeFormByDefault(shipNum, subGrid);

        do
        {
            buildPlacementOutputGrid(outputGrid, grid, subGrid);
            system("cls");
            printMainGrid(outputGrid, shipsOnField, PLACEMENT, notUsedPtr);
            printf("\nSelected ship: #%d\n\n", shipNum);
            printf("Controls: W,A,S,D - move\n");
            printf("          R       - rotate\n");
            printf("          Enter   - place\n");
        } while (!placeForm(shipNum, subGrid, grid));

        shipsOnField[shipNum - 1]++;
    } while (!isAllShipsOnField(shipsOnField));
}

void performRandomPlacement(char grid[][MAX_GRID], int shipsOnField[])
{
    Coords notUsedPtr = {-1};
    
    eatline;
    do
    {
        for (int i = 0; i < MAX_GRID; i++)
            for (int j = 0; j < MAX_GRID; j++)
                grid[i][j] = EMPTY_CELL;

        for (int i = 0; i < MAX_SHIP_TYPES; i++)
            shipsOnField[i] = 0;
        
        randomPlacement(grid, shipsOnField);
        system("cls");
        printMainGrid(grid, shipsOnField, PLACEMENT, notUsedPtr);
        printf("\n[Any key] - re-placement\n");
        printf("[Enter]   - apply placement");
    } while (_getch() != '\r');
}

void randomPlacement(char grid[][MAX_GRID], int shipsOnField[])
{
    int shipNum = 3;
    int x, y;        //Initial coordinates
    int dir;
    bool dirsTryed[4];
    bool isShipPlaced;
    static int srand_modificator = 0;
    
    srand((int)time(NULL) + srand_modificator);
    srand_modificator++;
    do
    {
        if (!isShipAval(shipsOnField, shipNum + 1))
            shipNum--;

        x = rand() % MAX_GRID;
        y = rand() % MAX_GRID;
        if (grid[y][x] == SHIP_CELL)
            continue;

        //Initialization
        for (int i = 0; i < 4; i++)
            dirsTryed[i] = false;
        isShipPlaced = false;

        do
        {
            dir = rand() % 4;

            if (dirsTryed[dir])
                continue;
            else
                dirsTryed[dir] = true;

            switch (dir)
            {
            case 0:
                isShipPlaced = setForward(shipNum, x, y, grid);
                break;
            case 1:
                isShipPlaced = setLeft(shipNum, x, y, grid);
                break;
            case 2:
                isShipPlaced = setBackward(shipNum, x, y, grid);
                break;
            case 3:
                isShipPlaced = setRight(shipNum, x, y, grid);
                break;
            }

        } while (!isAllDirsTryed(dirsTryed) && !isShipPlaced);
        
        if (isShipPlaced)
            shipsOnField[shipNum]++;

    } while (!isAllShipsOnField(shipsOnField));
}

bool isShipAval(int *shipsOnField, int shipNum)
{
    if (shipsOnField[shipNum - 1] < MAX_SHIP_TYPES - shipNum + 1)
        return true;
    else
        return false;
}

void buildPlacementOutputGrid(char outputGrid[][MAX_GRID], char grid[][MAX_GRID], char subGrid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
            if ((grid[i][j] == EMPTY_CELL) && (subGrid[i][j] == EMPTY_CELL))
                outputGrid[i][j] = EMPTY_CELL;

            else if ((grid[i][j] == SHIP_CELL) && (subGrid[i][j] == EMPTY_CELL))
                outputGrid[i][j] = SHIP_CELL;

            else if ((grid[i][j] == EMPTY_CELL) && (subGrid[i][j] == PLACING_SHIP_CELL))
                outputGrid[i][j] = PLACING_SHIP_CELL;

            else if ((grid[i][j] == SHIP_CELL) && (subGrid[i][j] == PLACING_SHIP_CELL))
                outputGrid[i][j] = IMPOSED_SHIP_CELL;
}

bool placeForm(int shipNum, char subGrid[][MAX_GRID], char grid[][MAX_GRID])
{
    char cmd = _getch();

    switch (cmd)
    {
    case 'w':
        moveForward(subGrid);
        break;
    case 'a':
        moveLeft(subGrid);
        break;
    case 's':
        moveBackward(subGrid);
        break;
    case 'd':
        moveRight(subGrid);
        break;
    case 'r':
        rotate(shipNum, subGrid);
        break;
    case '\r':  // = [ENTER]
        return applyPlacement(grid, subGrid);
    }

    return false;
}

void moveForward(char grid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        if (grid[0][i] == PLACING_SHIP_CELL)
            return;

    for (int i = 0; i < MAX_GRID - 1; i++)
        for (int j = 0; j < MAX_GRID; j++)
            grid[i][j] = grid[i + 1][j];

    for (int i = 0; i < MAX_GRID; i++)
        grid[MAX_GRID - 1][i] = EMPTY_CELL;
}

void moveLeft(char grid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        if (grid[i][0] == PLACING_SHIP_CELL)
            return;

    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID - 1; j++)
            grid[i][j] = grid[i][j + 1];

    for (int i = 0; i < MAX_GRID; i++)
        grid[i][MAX_GRID - 1] = EMPTY_CELL;
}

void moveBackward(char grid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        if (grid[MAX_GRID - 1][i] == PLACING_SHIP_CELL)
            return;

    for (int i = MAX_GRID - 1; i >= 1; i--)
        for (int j = 0; j < MAX_GRID; j++)
            grid[i][j] = grid[i - 1][j];

    for (int i = 0; i < MAX_GRID; i++)
        grid[0][i] = EMPTY_CELL;
}

void moveRight(char grid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID; i++)
        if (grid[i][MAX_GRID - 1] == PLACING_SHIP_CELL)
            return;

    for (int i = 0; i < MAX_GRID; i++)
        for (int j = MAX_GRID - 1; j >= 1; j--)
            grid[i][j] = grid[i][j - 1];

    for (int i = 0; i < MAX_GRID; i++)
        grid[i][0] = EMPTY_CELL;
}

void rotate(int shipNum, char grid[][MAX_GRID])
{
    int deckOrder = 0;

    if (shipNum == 1)
        return;

    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
            if (grid[i][j] == PLACING_SHIP_CELL)
            {
                deckOrder++;

                if (deckOrder == 1 &&
                    i + shipNum - 1 < MAX_GRID &&
                    j + shipNum - 1 < MAX_GRID)
                {
                    rtNormal(i, j, shipNum, grid);
                    return;
                }

                if (deckOrder == shipNum)
                {
                    rtReverse(i, j, shipNum, grid);
                    return;
                }
            }
            
}

void rtNormal(int i, int j, int shipNum, char grid[][MAX_GRID])
{
    char tmp;

    for (int cellOrder = 1; cellOrder < shipNum; cellOrder++)
    {
        tmp = grid[i + cellOrder][j];
        grid[i + cellOrder][j] = grid[i][j + cellOrder];
        grid[i][j + cellOrder] = tmp;
    }
}

void rtReverse(int i, int j, int shipNum, char grid[][MAX_GRID])
{
    char tmp;

    for (int cellOrder = 1; cellOrder < shipNum; cellOrder++)
    {
        tmp = grid[i - cellOrder][j];
        grid[i - cellOrder][j] = grid[i][j - cellOrder];
        grid[i][j - cellOrder] = tmp;
    }
}

bool applyPlacement(char grid[][MAX_GRID], char subGrid[][MAX_GRID])
{
    for (int i = 0; i < MAX_GRID - 1; i++)
        for (int j = 0; j < MAX_GRID - 1; j++)
        {
            if ((subGrid[i][j] == PLACING_SHIP_CELL) && (grid[i][j] == SHIP_CELL))
                return false;

            if ((subGrid[i][j] == PLACING_SHIP_CELL) &&
                ((grid[i][j + 1] == SHIP_CELL) ||
                (grid[i + 1][j] == SHIP_CELL) ||
                    (grid[i + 1][j + 1] == SHIP_CELL) ||
                    (grid[i - 1][j + 1] == SHIP_CELL)))
                return false;

            if ((grid[i][j] == SHIP_CELL) &&
                ((subGrid[i][j + 1] == PLACING_SHIP_CELL) ||
                (subGrid[i + 1][j] == PLACING_SHIP_CELL) ||
                    (subGrid[i + 1][j + 1] == PLACING_SHIP_CELL) ||
                    (subGrid[i - 1][j + 1] == PLACING_SHIP_CELL)))
                return false;
        }

    for (int i = 0; i < MAX_GRID; i++)
        if (((subGrid[i][MAX_GRID - 1] == PLACING_SHIP_CELL) &&
            (grid[i][MAX_GRID - 1] == SHIP_CELL)) ||

            ((subGrid[MAX_GRID - 1][i] == PLACING_SHIP_CELL) &&
            (grid[MAX_GRID - 1][i] == SHIP_CELL)))
            return false;

    for (int i = 0; i < MAX_GRID; i++)
        for (int j = 0; j < MAX_GRID; j++)
            if (subGrid[i][j] == PLACING_SHIP_CELL)
                grid[i][j] = SHIP_CELL;

    return true;
}

bool isAllShipsOnField(int shipsOnField[])
{
    for (int i = 0; i < MAX_SHIP_TYPES; i++)
        if (shipsOnField[i] < MAX_SHIP_TYPES - i)
            return false;

    return true;
}

bool setForward(int shipLength, int j_init, int i_init, char grid[][MAX_GRID])
{    
    if (i_init - shipLength < 0)
        return false;

    for (int deck = 0; deck <= shipLength; deck++)
        if (isShipAround(i_init - deck, j_init, grid))
            return false;

    for (int deck = 0; deck <= shipLength; deck++)
        grid[i_init - deck][j_init] = SHIP_CELL;
    
    return true;
}

bool setLeft(int shipLength, int j_init, int i_init, char grid[][MAX_GRID])
{
    if (j_init - shipLength < 0)
        return false;

    for (int deck = 0; deck <= shipLength; deck++)
        if (isShipAround(i_init, j_init - deck, grid))
            return false;

    for (int deck = 0; deck <= shipLength; deck++)
        grid[i_init][j_init - deck] = SHIP_CELL;

    return true;
}

bool setBackward(int shipLength, int j_init, int i_init, char grid[][MAX_GRID])
{
    if (i_init + shipLength >= MAX_GRID)
        return false;

    for (int deck = 0; deck <= shipLength; deck++)
        if (isShipAround(i_init + deck, j_init, grid))
            return false;

    for (int deck = 0; deck <= shipLength; deck++)
        grid[i_init + deck][j_init] = SHIP_CELL;

    return true;
}

bool setRight(int shipLength, int j_init, int i_init, char grid[][MAX_GRID])
{
    if (j_init + shipLength >= MAX_GRID)
        return false;

    for (int deck = 0; deck <= shipLength; deck++)
        if (isShipAround(i_init, j_init + deck, grid))
            return false;

    for (int deck = 0; deck <= shipLength; deck++)
        grid[i_init][j_init + deck] = SHIP_CELL;

    return true;
}

bool isShipAround(int i, int j, char grid[][MAX_GRID])
{   
    if (i - 1 >= 0 && grid[i - 1][j] == SHIP_CELL ||
        j - 1 >= 0 && grid[i][j - 1] == SHIP_CELL ||
        i + 1 < MAX_GRID && grid[i + 1][j] == SHIP_CELL ||
        j + 1 < MAX_GRID && grid[i][j + 1] == SHIP_CELL ||

        i - 1 >= 0 && j - 1 >= 0 && grid[i - 1][j - 1] == SHIP_CELL ||
        i - 1 >= 0 && j + 1 < MAX_GRID && grid[i - 1][j + 1] == SHIP_CELL ||
        i + 1 < MAX_GRID && j - 1 >= 0 && grid[i + 1][j - 1] == SHIP_CELL ||
        i + 1 < MAX_GRID && j + 1 < MAX_GRID && grid[i + 1][j + 1] == SHIP_CELL)
        return true;
    else
        return false;
}

bool isAllDirsTryed(bool dirsTryed[])
{
    for (int dir = 0; dir < 4; dir++)
        if (dirsTryed[dir] == false)
            return false;
    
    return true;
}