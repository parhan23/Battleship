#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>

#define SCREEN_WIDTH 50

#define EMPTY_CELL        '\xB0'
#define PLACING_SHIP_CELL '\xB1'
#define IMPOSED_SHIP_CELL '\xB2'
#define SHIP_CELL         '\xDB'
#define HIT_CELL          '\xFD'
#define MISS_CELL         '*'

#define MAX_PNAME      100
#define MAX_FNAME      20
#define MAX_GRID       10
#define MAX_SHIP_TYPES 4
#define MANUAL_PL      '1'

#define PLACEMENT      0
#define BATTLE         1

#define PLAYER_TURN    0
#define COMPUTER_TURN  1

#define PLAYER_WINS    0
#define COMPUTER_WINS  1

#define eatline\
    while (getchar() != '\n')\
        continue

#define printfCenter(S, ...)\
{\
    printf("%*s", ((SCREEN_WIDTH - strlen(S)) / 2), "");\
    printf(S, __VA_ARGS__);\
}

struct Score
{
    char name[MAX_PNAME];
    int shots, hits;
    int kills, deaths;
};

struct Scores
{
    Score *prev;
    Score *curr;
};

struct Grids
{
    //Player's grids
    char mainGrid[MAX_GRID][MAX_GRID];
    char auxGrid[MAX_GRID][MAX_GRID];

    //Computer's grid
    char computer[MAX_GRID][MAX_GRID];
};

struct Ships
{
    int player[MAX_SHIP_TYPES];
    int computer[MAX_SHIP_TYPES];
};

struct Coords
{
    int x, y;
};

enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

void printGreet(void);
bool getName(char *);

Scores * initScores(void);
Grids * initGrids(void);

FILE * getFilePath(char[]);
bool getPrevScore(FILE *, char *, Score *);
void printPrevScore(Score *);
char getPlacementMode(void);

void performManualPlacement(char[][MAX_GRID], int[]);
Ships * initShipsOnField(void);
void printMainGrid(char [][MAX_GRID], int [], int, Coords);
void printShipIcon(int, int [], int, int);
int getShip(int *);
bool isShipAval(int *, int);
void initSubGrid(char [][MAX_GRID]);
void placeFormByDefault(int, char [][MAX_GRID]);
void buildPlacementOutputGrid(char[][MAX_GRID], char[][MAX_GRID], char[][MAX_GRID]);

bool placeForm(int, char[][MAX_GRID], char[][MAX_GRID]);
void moveForward(char [][MAX_GRID]);
void moveLeft(char[][MAX_GRID]);
void moveBackward(char[][MAX_GRID]);
void moveRight(char[][MAX_GRID]);
void rotate(int, char[][MAX_GRID]);
void rtNormal(int, int, int, char[][MAX_GRID]);
void rtReverse(int, int, int, char[][MAX_GRID]);
bool applyPlacement(char[][MAX_GRID], char[][MAX_GRID]);
bool isAllShipsOnField(int []);

void performRandomPlacement(char[][MAX_GRID], int[]);
void randomPlacement(char[][MAX_GRID], int[]);
bool setForward(int, int, int, char[][MAX_GRID]);
bool setLeft(int, int, int, char[][MAX_GRID]);
bool setBackward(int, int, int, char[][MAX_GRID]);
bool setRight(int, int, int, char[][MAX_GRID]);
bool isShipAround(int, int, char[][MAX_GRID]);
bool isAllDirsTryed(bool[]);

bool playerTurn(Scores *, Grids *, Ships *);
void printAuxiliaryGrid(char[][MAX_GRID], Coords, int[]);
bool placePointer(char[][MAX_GRID], Coords *);
bool isShipDestroyed(char[][MAX_GRID], const Coords);
int markDestroyedShip(char[][MAX_GRID], const Coords);
void markDestroyedCell(char[][MAX_GRID], const Coords);
bool isAllShipsDestroyed(int[]);

bool computerTurn(Scores *, Grids *, Ships *);
Coords findAssumedCell(const Coords, char[][MAX_GRID], bool[]);

void printResult(int);
void saveResult(char[], Scores *, char[]);
void freeMem(Scores *, Grids *, Ships *);