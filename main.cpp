#include "main.h"

int main(void)
{
    FILE *fp;
    Scores *scores;
    Grids *grids;
    Ships *shipsOnField;
    char fname[MAX_FNAME];
    char pname[MAX_PNAME];
    
    //Console size and color
    system("mode con cols=50 lines=30");
    system("color F0");
    setlocale(LC_ALL, "866");

    printGreet();
    getName(pname);
        
    grids = initGrids();
    scores = initScores(pname);
    shipsOnField = initShipsOnField();

    fp = getFilePath(fname);
    if (fp && getPrevScore(fp, pname, scores->prev))
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

    saveResult(fname, scores->curr);
    freeMem(scores, grids, shipsOnField);

    printfCenter("Thank you for playing! ;)\n");
    eatline;
    _getch();

    return 0;
}

void saveResult(char fname[], Score *currScore)
{
    FILE *file_in = fopen(fname, "rb");
    FILE *file_out = fopen(fname, "rb+");
    Score scoreBuffer;
    bool isScoreFound = false;

    if (!file_in)
        return;

    while (fread(&scoreBuffer, sizeof(Score), 1, file_in))
        if (strcmp(scoreBuffer.name, currScore->name) == 0)
        {
            scoreBuffer.shots = currScore->shots;
            scoreBuffer.hits = currScore->hits;

            fwrite(&scoreBuffer, sizeof(Score), 1, file_out);
            isScoreFound = true;
            break;
        }
        else
            fseek(file_out, sizeof(Score), SEEK_CUR);

    if (!isScoreFound)
        fwrite(currScore, sizeof(Score), 1, file_out);

    fclose(file_in);
    fclose(file_out);
}

void freeMem(Scores * scores, Grids * grids, Ships * shipsOnField)
{
    free(scores->prev);
    free(scores->curr);
    free(scores);
    free(grids);
    free(shipsOnField);
}