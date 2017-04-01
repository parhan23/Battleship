#include "main.h"

bool playerTurn(Scores * score, Grids * grids, Ships * shipsOnField)
{
    Coords ptr = {MAX_GRID / 2, MAX_GRID / 2};  //Pointer position
    Coords notUsedPtr = {-1};
    int shipNum;

    do
    {
        //Pointer movements loop
        do
        {
            system("cls");
            printMainGrid(grids->mainGrid, shipsOnField->player, BATTLE, notUsedPtr);
            printAuxiliaryGrid(grids->auxGrid, ptr, shipsOnField->computer);
            printf("\nControls: W,A,S,D - move cannons\n");
            printf("          [Space] - fire\n");
        } while (!placePointer(grids->auxGrid, &ptr));

        score->curr->shots++;
        if (grids->computer[ptr.y][ptr.x] == SHIP_CELL)
        {
            grids->auxGrid[ptr.y][ptr.x] = HIT_CELL;
            grids->computer[ptr.y][ptr.x] = HIT_CELL;
            score->curr->hits++;
            if (isShipDestroyed(grids->computer, ptr))
            {
                score->curr->kills++;
                shipNum = markDestroyedShip(grids->auxGrid, ptr);
                shipsOnField->computer[shipNum]--;
            }
        }
        else
            grids->auxGrid[ptr.y][ptr.x] = MISS_CELL;

        system("cls");
        printMainGrid(grids->mainGrid, shipsOnField->player, BATTLE, notUsedPtr);
        printAuxiliaryGrid(grids->auxGrid, ptr, shipsOnField->computer);

        if (isAllShipsDestroyed(shipsOnField->computer))
            return true;
    } while (grids->auxGrid[ptr.y][ptr.x] == HIT_CELL);

    return false;
}

bool computerTurn(Scores * score, Grids * grids, Ships * shipsOnField)
{
    static Coords prevHit = {-1, -1};
    static bool dirsTryed[4] = {false, false, false, false};
    Coords ptr;
    int shipNum;
    
    srand((int)time(NULL));
    do
    {
        if (prevHit.x == -1)
            do
            {
                ptr.x = rand() % MAX_GRID;
                ptr.y = rand() % MAX_GRID;
            } while (grids->mainGrid[ptr.y][ptr.x] == HIT_CELL ||
                grids->mainGrid[ptr.y][ptr.x] == MISS_CELL);
        else
            ptr = findAssumedCell(prevHit, grids->mainGrid, dirsTryed);

        if (grids->mainGrid[ptr.y][ptr.x] == SHIP_CELL)
        {
            grids->mainGrid[ptr.y][ptr.x] = HIT_CELL;
            prevHit = ptr;
            if (isShipDestroyed(grids->mainGrid, ptr))
            {
                score->curr->deaths++;
                shipNum = markDestroyedShip(grids->mainGrid, ptr);
                shipsOnField->player[shipNum]--;

                prevHit.x = -1;
                for (int i = 0; i < 4; i++)
                    dirsTryed[i] = false;
            }
        }
        else
            grids->mainGrid[ptr.y][ptr.x] = MISS_CELL;

        system("cls");
        printMainGrid(grids->mainGrid, shipsOnField->player, BATTLE, ptr);
        printAuxiliaryGrid(grids->auxGrid, ptr, shipsOnField->computer);
        printf("\nComputer strikes!");
        printf("\nPress [Enter] to continue...");
        while (_getch() != '\r')
            continue;

        if (isAllShipsDestroyed(shipsOnField->player))
            return true;
    } while (grids->mainGrid[ptr.y][ptr.x] == HIT_CELL);

    return false;
}

bool placePointer(char grid[][MAX_GRID], Coords *ptr)
{
    char cmd = _getch();

    switch (cmd)
    {
    case 'w':
        if (ptr->y - 1 >= 0)
            ptr->y--;
        break;
    case 'a':
        if (ptr->x - 1 >= 0)
            ptr->x--;
        break;
    case 's':
        if (ptr->y + 1 < MAX_GRID)
            ptr->y++;
        break;
    case 'd':
        if (ptr->x + 1 < MAX_GRID)
            ptr->x++;
        break;
    case ' ':  // == [Space]
        if (grid[ptr->y][ptr->x] == HIT_CELL ||
            grid[ptr->y][ptr->x] == MISS_CELL)
            return false;
        else
            return true;
    }

        return false;
}

bool isShipDestroyed(char grid[][MAX_GRID], const Coords aPtr)
{
    Coords ptr = aPtr;
    
    while (ptr.y - 1 >= 0 &&
          (grid[ptr.y - 1][ptr.x] != EMPTY_CELL &&
           grid[ptr.y - 1][ptr.x] != MISS_CELL))
    {
        ptr.y--;
        if (grid[ptr.y][ptr.x] == SHIP_CELL)
            return false;
    }

    ptr = aPtr;
    while (ptr.y + 1 < MAX_GRID &&
        (grid[ptr.y + 1][ptr.x] != EMPTY_CELL &&
         grid[ptr.y + 1][ptr.x] != MISS_CELL))
    {
        ptr.y++;
        if (grid[ptr.y][ptr.x] == SHIP_CELL)
            return false;
    }

    ptr = aPtr;
    while (ptr.x - 1 >= 0 &&
        (grid[ptr.y][ptr.x - 1] != EMPTY_CELL &&
            grid[ptr.y][ptr.x - 1] != MISS_CELL))
    {
        ptr.x--;
        if (grid[ptr.y][ptr.x] == SHIP_CELL)
            return false;
    }

    ptr = aPtr;
    while (ptr.x + 1 < MAX_GRID &&
        (grid[ptr.y][ptr.x + 1] != EMPTY_CELL &&
            grid[ptr.y][ptr.x + 1] != MISS_CELL))
    {
        ptr.x++;
        if (grid[ptr.y][ptr.x] == SHIP_CELL)
            return false;
    }

    return true;
}

int markDestroyedShip(char grid[][MAX_GRID], const Coords aPtr)
{
    Coords cell = aPtr;
    int shipNum = 0;

    markDestroyedCell(grid, cell);

    while (cell.y - 1 >= 0 && grid[cell.y - 1][cell.x] == HIT_CELL)
    {
        cell.y--;
        markDestroyedCell(grid, cell);
        shipNum++;
    }

    cell = aPtr;
    while (cell.y + 1 < MAX_GRID && grid[cell.y + 1][cell.x] == HIT_CELL)
    {
        cell.y++;
        markDestroyedCell(grid, cell);
        shipNum++;
    }

    cell = aPtr;
    while (cell.x - 1 >= 0 && grid[cell.y][cell.x - 1] == HIT_CELL)
    {
        cell.x--;
        markDestroyedCell(grid, cell);
        shipNum++;
    }

    cell = aPtr;
    while (cell.x + 1 < MAX_GRID && grid[cell.y][cell.x + 1] == HIT_CELL)
    {
        cell.x++;
        markDestroyedCell(grid, cell);
        shipNum++;
    }
    
    return shipNum;
}

void markDestroyedCell(char grid[][MAX_GRID], const Coords cell)
{
    if (cell.y - 1 >= 0 && grid[cell.y - 1][cell.x] != HIT_CELL)
        grid[cell.y - 1][cell.x] = MISS_CELL;

    if (cell.y + 1 < MAX_GRID && grid[cell.y + 1][cell.x] != HIT_CELL)
        grid[cell.y + 1][cell.x] = MISS_CELL;

    if (cell.x - 1 >= 0 && grid[cell.y][cell.x - 1] != HIT_CELL)
        grid[cell.y][cell.x - 1] = MISS_CELL;

    if (cell.x + 1 < MAX_GRID && grid[cell.y][cell.x + 1] != HIT_CELL)
        grid[cell.y][cell.x + 1] = MISS_CELL;

    
    if (cell.y - 1 >= 0 && cell.x - 1 >= 0)
        grid[cell.y - 1][cell.x - 1] = MISS_CELL;

    if (cell.y - 1 >= 0 && cell.x + 1 < MAX_GRID)
        grid[cell.y - 1][cell.x + 1] = MISS_CELL;

    if (cell.y + 1 < MAX_GRID && cell.x - 1 >= 0)
        grid[cell.y + 1][cell.x - 1] = MISS_CELL;

    if (cell.y + 1 < MAX_GRID && cell.x + 1 < MAX_GRID)
        grid[cell.y + 1][cell.x + 1] = MISS_CELL;
}

bool isAllShipsDestroyed(int shipsOnField[])
{
    for (int i = 0; i < MAX_SHIP_TYPES; i++)
        if (shipsOnField[i] > 0)
            return false;
    
    return true;
}

Coords findAssumedCell(const Coords prevHit, char grid[][MAX_GRID], bool dirsTryed[])
{
    Coords ptr;
    int dir;
    
    if (prevHit.y - 1 >= 0 && grid[prevHit.y - 1][prevHit.x] == HIT_CELL ||
        prevHit.y + 1 < MAX_GRID && grid[prevHit.y + 1][prevHit.x] == HIT_CELL)
        dirsTryed[1] = dirsTryed[3] = true;

    if (prevHit.x - 1 >= 0 && grid[prevHit.y][prevHit.x - 1] == HIT_CELL ||
        prevHit.x + 1 < MAX_GRID && grid[prevHit.y][prevHit.x + 1] == HIT_CELL)
        dirsTryed[0] = dirsTryed[2] = true;
    
    do
    {
        ptr = prevHit;
        do
            dir = rand() % 4;
        while (dirsTryed[dir]);

        switch (dir)
        {
        case 0:
            while (grid[ptr.y][ptr.x] == HIT_CELL && ptr.y - 1 >= 0)
                ptr.y--;
            break;
        case 1:
            while (grid[ptr.y][ptr.x] == HIT_CELL && ptr.x - 1 >= 0)
                ptr.x--;
            break;
        case 2:
            while (grid[ptr.y][ptr.x] == HIT_CELL && ptr.y + 1 < MAX_GRID)
                ptr.y++;
            break;
        case 3:
            while (grid[ptr.y][ptr.x] == HIT_CELL && ptr.x + 1 < MAX_GRID)
                ptr.x++;
            break;
        }

        if (grid[ptr.y][ptr.x] == HIT_CELL ||
            grid[ptr.y][ptr.x] == MISS_CELL)
            dirsTryed[dir] = true;
    } while (grid[ptr.y][ptr.x] != EMPTY_CELL &&
             grid[ptr.y][ptr.x] != SHIP_CELL);
    
    return ptr;
}