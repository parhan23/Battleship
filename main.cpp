#include "main.h"

int main(void)
{
    FILE *fp;
    Scores *scores;
    Grids *grids;
    Ships *shipsOnField;
    char fname[MAX_FNAME];
    char name[MAX_PNAME];
    int turn;
    
    //Console size and color
    system("mode con cols=50 lines=30");
    system("color F0");
    setlocale(LC_ALL, "866");

    printGreet();
    if (!getName(name))
        exit(EXIT_SUCCESS);
    else
    {
        grids = initGrids();
        scores = initScores();
        shipsOnField = initShipsOnField();
    }

    fp = getFilePath(fname);
    if (fp && getPrevScore(fp, name, scores->prev))
        printPrevScore(scores->prev);

    if (getPlacementMode() == MANUAL_PL)
        performManualPlacement(grids->mainGrid, shipsOnField->player);
    else
        performRandomPlacement(grids->mainGrid, shipsOnField->player);
    randomPlacement(grids->computer, shipsOnField->computer);
    
    while (true)
    {
        if (playerTurn(scores, grids, shipsOnField))
        {
            printResult(PLAYER_WINS);
            break;
        }

        if (computerTurn(scores, grids, shipsOnField))
        {
            printResult(COMPUTER_WINS);
            break;
        }
    }

    saveResult(fname, scores, name);
    freeMem(scores, grids, shipsOnField);

    return 0;
}

void saveResult(char fname[], Scores * scores, char targetName[])
{
    FILE *file_in = fopen(fname, "r");
    FILE *file_out = fopen(fname, "r+");
    char nameFromFile[MAX_PNAME];
    char ch;

    if (!file_in)
        return;
    
    while (fscanf(file_in, "%s", nameFromFile) != EOF)
    {
        if (strcmp(nameFromFile, targetName) == 0)
        {
            fseek(file_out, ftell(file_in), SEEK_SET);
            break;
        }
        while (((ch = fgetc(file_in)) != EOF) && (ch != '\n'))
            continue;
    }

    if (ch == EOF)
    {
        fseek(file_out, NULL, SEEK_END);
        fprintf(file_out, "\n%s", targetName);
    }
    fprintf(file_out, " %d", scores->curr->shots);
    fprintf(file_out, " %d", scores->curr->hits);
    fprintf(file_out, " %d", scores->curr->kills);
    fprintf(file_out, " %d", scores->curr->deaths);

    fclose(file_in);
    fclose(file_out);

    printf("Thank you for playing! ;)\n");
    eatline;
    _getch();
}

void freeMem(Scores * scores, Grids * grids, Ships * shipsOnField)
{
    free(scores->prev);
    free(scores->curr);
    free(scores);
    free(grids);
    free(shipsOnField);
}