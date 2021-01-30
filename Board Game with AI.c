#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//::::::::::::::CSE462 - INTRODUCTION TO ARTIFICIAL INTELLIGENCE:::::::::::::::::
//:::::::::::::::::::::: H U R K A N  U G U R  20160702051 ::::::::::::::::::::::
int MinimaxAlgorithm(char gameBoard[][8], int depth, int alpha, int beta, int isMaximising);
char baseScores[10] = "123456789"; //TO BE USED TO EVALUATE THE VALUE OF THE OPPONENTS
char safetyRate[11] = " 123456789"; //TO BE USED TO EVALUATE SAFETY RATE OF THE AI AND THE USER
int userTurn = 1, aiTurn = 2, currentTurn = 1; //REPRESENTS THE TURNS OF USER, AI AND THE GAME'S CURRENT TURN
int isGameOver = 0; //IF == 0, THAT MEANS GAME IS NOT OVER, ELSE GAME IS OVER
char indicators[2] = { 'X', 'O' }; // X == PLAYER1 , Y == PLAYER2
char fromIndex[3], toIndex[3]; //SELECTION COORDINATES THAT THE USER WILL PICK A STONE TO MOVE SOMEWHERE ELSE
int turnNumbers[2] = {100,100}; //TURN LIMITS OF THE USER AND THE AI WILL BE HELD IN HERE (INDEX[0] == PLAYER1'S TURN NUMBER, INDEX[1] == PLAYER2'S TURN NUMBER
int pieceNumber = 0; //THE NUMBER OF PIECE OF STONES ON THE BOARD
int safeUserScore = 0, safeAIScore = 0; //THESE WILL REPRESENT THE NUMBER OF MOVABLE SPACES FOR BOTH THE AI AND THE USER
int offensivesafeUserScore = 0, offensivesafeAIScore = 0; //THESE WILL REPRESENT THE NUMBER OF CAPTURED ENEMIES FOR BOTH THE AI AND THE USER
int prevXY[2] = {-1, -1}; //THIS ARRAY WILL HOLD THE AI'S STONE'S POSITION IN THE PREVIOUS STATE (IT IS LIKE HISTORY OF THE LATEST STONE THAT MOVED)
int victimCoordinates[2] = {-1,-1};
//::::::::THE FUNCTION THAT COMBINES ROWS AND COLUMNS AS COORDINATES::::::::
char* getCoordinates(int i, int j)
{
    char row[10] = "abcdefg";
    char column[10] = "1234567";

    char* coordinate = (char*)malloc(sizeof(char)*3);
    coordinate[0] = row[i-1];
    coordinate[1] = column[j-1];
    coordinate[2] = '\0';
    return coordinate;
}
//:::::::::::::THE FUNCTION THAT COUNTS MOVABLE ALTERNATIVES FOR BOTH USER AND AI::::::::::::::::::::::
//:::::::::::::::::::::::::IF ONE OF THEM IS 0 THAT MEANS THE GAME IS OVER:::::::::::::::::::::::::::::
void scoreEvaluationFunction(char gameBoard[][8])
{
    int i,j;
    safeAIScore = 0; safeUserScore = 0, offensivesafeAIScore = 0, offensivesafeUserScore = 0;

        //EVALUATE THE SAFETY SCORE FOR THE USER (EACH EMPTY SIDE IS +1 POINTS !!!)
        for(i = 1; i <= 7; i++)
            for(j = 1; j <= 7; j++)
                if(gameBoard[i][j] == indicators[userTurn-1]) //IF THE SELECTED STONE BELONGS TO THE USER, EXAMINE IT
                {
                    if(i+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i+1][j] != indicators[aiTurn-1] && gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != '@' && gameBoard[i+1][j] != '#')
                            safeUserScore++;
                    if(i-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i-1][j] != indicators[aiTurn-1] && gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != '@' && gameBoard[i-1][j] != '#')
                            safeUserScore++;
                    if(j+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j+1] != indicators[aiTurn-1] && gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != '@' && gameBoard[i][j+1] != '#')
                            safeUserScore++;
                    if(j-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j-1] != indicators[aiTurn-1] && gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != '@' && gameBoard[i][j-1] != '#')
                            safeUserScore++;
                }
        //EVALUATE THE SAFETY SCORE FOR THE AI (EACH EMPTY SIDE IS +1 POINTS !!!)
        for(i = 1; i <= 7; i++)
            for(j = 1; j <= 7; j++)
                if(gameBoard[i][j] == indicators[aiTurn-1] || gameBoard[i][j] == '@' || gameBoard[i][j] == '#')
                {
                    if(i+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i+1][j] != indicators[aiTurn-1] && gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != '@' && gameBoard[i+1][j] != '#')
                            safeAIScore++;
                    if(i-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i-1][j] != indicators[aiTurn-1] && gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != '@' && gameBoard[i-1][j] != '#')
                            safeAIScore++;
                    if(j+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j+1] != indicators[aiTurn-1] && gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != '@' && gameBoard[i][j+1] != '#')
                            safeAIScore++;
                    if(j-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j-1] != indicators[aiTurn-1] && gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != '@' && gameBoard[i][j-1] != '#')
                            safeAIScore++;
                }
        //EVALUATE THE OFFENSIVE SCORE FOR THE USER (EACH CAPTURED AI IS +1 POINTS !!!)
        for(i = 1; i <= 7; i++)
            for(j = 1; j <= 7; j++)
                if(gameBoard[i][j] == indicators[userTurn-1])
                {
                    if(i+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i+1][j] == indicators[aiTurn-1] || gameBoard[i+1][j] == '@' || gameBoard[i+1][j] == '#')
                            offensivesafeUserScore++;
                    if(i-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i-1][j] == indicators[aiTurn-1] || gameBoard[i-1][j] == '@' || gameBoard[i-1][j] == '#')
                            offensivesafeUserScore++;
                    if(j+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j+1] == indicators[aiTurn-1] || gameBoard[i][j+1] == '@' || gameBoard[i][j+1] == '#')
                            offensivesafeUserScore++;
                    if(j-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j-1] == indicators[aiTurn-1] || gameBoard[i][j-1] == '@' || gameBoard[i][j-1] == '#')
                            offensivesafeUserScore++;
                }
        //EVALUATE THE OFFENSIVE SCORE FOR THE USER (EACH CAPTURED USER IS +1 POINTS !!!)
        for(i = 1; i <= 7; i++)
            for(j = 1; j <= 7; j++)
                if(gameBoard[i][j] == indicators[aiTurn-1] || gameBoard[i][j] == '@' || gameBoard[i][j] == '#')
                {
                    if(i+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i+1][j] == indicators[userTurn-1])
                            offensivesafeAIScore++;
                    if(i-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i-1][j] == indicators[userTurn-1])
                            offensivesafeAIScore++;
                    if(j+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j+1] == indicators[userTurn-1])
                            offensivesafeAIScore++;
                    if(j-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j-1] == indicators[userTurn-1])
                            offensivesafeAIScore++;
                }
}
//:::::::::::::::::::::::::THE FUNCTION THAT DISPLAYS GAME SCREEN::::::::::::::::::::::::::::::::::::::
//:::::::::THIS FUNCTION ALSO DISPLAYS REMAINING MOVEMENT NUMBER AND MOVABLE NEIGHBOUR NUMBER::::::::::
void printGameBoard(char gameBoard[][8])
{
    int i,j;
    system("cls");
    printf("\n");
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
           if(i != 0)
               if( j == 0 || (j > 0 && (gameBoard[i][j] == indicators[userTurn-1] || gameBoard[i][j] == indicators[aiTurn-1])))
                    printf("\t %c |",gameBoard[i][j]);
               else if(gameBoard[i][j] == '@' || gameBoard[i][j] == '#')
                    printf("\t %c |",indicators[aiTurn-1]);
                else
                    printf("\t   |");
            else
                printf("\t %c  ",gameBoard[i][j]);
        }
        printf("\n\t   ---------------------------------------------------------\n");
    }
    scoreEvaluationFunction(gameBoard);
    printf("\n\t\t\t   AI remaining move: %d\n\t\t\t  User remaining move: %d\n", turnNumbers[aiTurn-1], turnNumbers[userTurn-1]);
    printf("\n\tAI have [%d] movement options\tUser have [%d] movement options\n", safeAIScore, safeUserScore);
    printf("\tAI total score [%d]\t\tUser total score [%d]\n\n", (safeAIScore+offensivesafeAIScore)-(safeUserScore+offensivesafeUserScore), (safeUserScore+offensivesafeUserScore)-(safeAIScore+offensivesafeAIScore));
}
//THE FUNCTION THAT DETERMINES A SINGLE TARGET TO ATTACK (IF A TARGET HAS THE LEAST MOVABLE AREA, THAT MEANS THAT TARGET IS MORE VALUABLE
void determineVictimTarget(char gameBoard[][8])
{
    int evaluate = 0,i,j;
    int bestEvaluation = 1000;

    //IF IT IS THE BEGINNING OF THE GAME, SINCE THERE IS NO DEFAULT VICTIM, SKIP THIS PART
    if(victimCoordinates[0] != -1)
    {
        //CHECK IF THE VICTIM HAS MOVABLE AREA
        if(victimCoordinates[0]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            if(gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] == ' ')
                evaluate = 1;
        if(victimCoordinates[0]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            if(gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] == ' ')
                evaluate = 1;
        if(victimCoordinates[1]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            if(gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] == ' ')
                evaluate = 1;
        if(victimCoordinates[1]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            if(gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] == ' ')
                evaluate = 1;

        //IF THE VICTIM HAS NO MOVABLE AREA LEFT THAT MEANS IT HAS BEEN CAPTURED, THEN MARK ITS SURROUNDING
        if(evaluate == 0)
        {
            if(victimCoordinates[0]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                if(gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] == indicators[aiTurn-1] || gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] == '@')
                    gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] = '#';
            if(victimCoordinates[0]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                if(gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] == indicators[aiTurn-1] || gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] == '@')
                    gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] = '#';
            if(victimCoordinates[1]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                if(gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] == indicators[aiTurn-1] || gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] == '@')
                    gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] = '#';
            if(victimCoordinates[1]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                if(gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] == indicators[aiTurn-1] || gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] == '@')
                    gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] = '#';
        }
    }
    //IT IS TIME TO FIND NEW VICTIM
    for(i = 1; i <= 7; i++){
        for(j = 1; j <= 7; j++){
            if(gameBoard[i][j] == '@')
                gameBoard[i][j] = indicators[aiTurn-1];
            else if(gameBoard[i][j] == indicators[userTurn-1]){
                evaluate = 0;
                if(i+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    if(gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != indicators[aiTurn-1]&& gameBoard[i+1][j] != '@' && gameBoard[i+1][j] != '#')
                        evaluate++;
                if(i-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    if(gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != indicators[aiTurn-1]&& gameBoard[i-1][j] != '@' && gameBoard[i-1][j] != '#')
                        evaluate++;
                if(j+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    if(gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != indicators[aiTurn-1]&& gameBoard[i][j+1] != '@' && gameBoard[i][j+1] != '#')
                        evaluate++;
                if(j-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    if(gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != indicators[aiTurn-1]&& gameBoard[i][j-1] != '@' && gameBoard[i][j-1] != '#')
                        evaluate++;
                 //THE OPPONENT WITH THE LEAST SPACE WILL BE THE TARGET
                if(evaluate > 0 && (evaluate < bestEvaluation))
                {
                    victimCoordinates[0] = i;
                    victimCoordinates[1] = j;
                    bestEvaluation = evaluate;
                }
            }
        }
    }
    //IF THE NEW VICTIM IS SURROUNDED BY ENEMY, MARK THESE ENEMIES SO THAT THEY PROTECT THEIR POSITION
    if(victimCoordinates[0]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] == indicators[aiTurn-1])
            gameBoard[victimCoordinates[0]+1][victimCoordinates[1]] = '@';
    if(victimCoordinates[0]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] == indicators[aiTurn-1])
            gameBoard[victimCoordinates[0]-1][victimCoordinates[1]] = '@';
    if(victimCoordinates[1]+1 <= 7)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] == indicators[aiTurn-1])
            gameBoard[victimCoordinates[0]][victimCoordinates[1]+1] = '@';
    if(victimCoordinates[1]-1 >= 1)//CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] == indicators[aiTurn-1])
            gameBoard[victimCoordinates[0]][victimCoordinates[1]-1] = '@';
}
//:::::::::::::::::::::::::THE FUNCTION THAT CHECKS AI/USER'S EFFICIENCY:::::::::::::::::::::::::::::::::::::::::::
//:::::::::::::::::::::::::HIGHER EVALUATION RESULT = MORE VALUABLE OPPONENT:::::::::::::::::::::::::::::::::::::::
//:::::::::::HIGHER EVALLUATION VALUE WILL PRIORITIZE THE SELECTED OPPONENT THAT MUST BE ATTACKED FIRST::::::::::::
char mapEvaluationFunction(char gameBoard[][8], char selfIndicator, int i, int j)
{
    int max = -1;
    int k, further;
    int block[] = {0,0,0,0};
    for(further = 1; further <= 9 && (max < 1); further++)
    {
        //CHECK DOWN
         //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(i+further <= 7)
        {
            if(further > 1 && max > 0)
                break;
            if(gameBoard[i+further][j] == indicators[aiTurn-1] ||gameBoard[i+further][j] == indicators[userTurn-1] || gameBoard[i+further][j] == ' '|| gameBoard[i+further][j] == '@'|| gameBoard[i+further][j] == '#')
                block[0] = 1;
            if((gameBoard[i+further][j] != selfIndicator) && (gameBoard[i+further][j] != ' ' && gameBoard[i+further][j] != '@') && block[0] == 0)
                for(k = 0; k < 9; k++)
                {
                    if(baseScores[k] == gameBoard[i+further][j])
                    {
                        if(max < k)
                        {
                            max = k;
                            if(further > 1 && k > 0)
                                gameBoard[i+1][j] = baseScores[k];
                        }
                        break;
                    }
                }
        }
        //CHECK RIGHT
        //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(j+further <= 7)
        {
            if(further > 1  && max > 0)
                break;
            if(gameBoard[i][j+further] == indicators[aiTurn-1] ||gameBoard[i][j+further] == indicators[userTurn-1] || gameBoard[i][j+further] == ' ' || gameBoard[i][j+further] == '@'|| gameBoard[i][j+further] == '#')
                block[1] = 1;
            if((gameBoard[i][j+further] != selfIndicator) && (gameBoard[i][j+further] != ' ' && gameBoard[i][j+further] != '@') && block[1]==0)
                    for(k = 0; k < 9; k++){
                        if(baseScores[k] == gameBoard[i][j+further])
                        {
                            if(max < k)
                            {
                                max = k;
                                if(further > 1 && k > 0)
                                    gameBoard[i][j+1] = baseScores[k];
                            }
                            break;
                        }
                    }
        }
        //CHECK LEFT
        //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(j-further >= 1)
        {
            if(further > 1 && max > 0)
                break;
            if(gameBoard[i][j-further] == indicators[aiTurn-1] || gameBoard[i][j-further] == indicators[userTurn-1]|| gameBoard[i][j-further] == ' '|| gameBoard[i][j-further] == '@' || gameBoard[i][j-further] == '#')
                block[2] = 1;
            else if((gameBoard[i][j-further] != indicators[aiTurn-1]) && (gameBoard[i][j-further] != ' ' && gameBoard[i][j-further] != '@' && gameBoard[i][j-further] != indicators[userTurn-1]) && block[2] == 0)
                    for(k = 0; k < 9; k++)
                    {
                        if(baseScores[k] == gameBoard[i][j-further])
                        {
                            if(max < k)
                            {
                                max = k;
                                if(further > 1 && k > 0)
                                    gameBoard[i][j-1] = baseScores[k];
                            }
                            break;
                        }
                    }
        }
        //CHECK UP
        //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        if(i-further >= 1)
        {
            if(further > 1 && max > 0)
                break;
            if(gameBoard[i-further][j] == indicators[aiTurn-1] ||gameBoard[i-further][j] == indicators[userTurn-1]  || gameBoard[i-further][j] == ' ' || gameBoard[i-further][j] == '@'|| gameBoard[i-further][j] == '#')
                block[3] = 1;
            else if((gameBoard[i-further][j] != selfIndicator) && (gameBoard[i-further][j] != ' ' && gameBoard[i-further][j] != '@' && gameBoard[i-further][j] != indicators[userTurn-1]) && block[3] == 0)
                    for(k = 0; k < 9; k++)
                    {
                        if(baseScores[k] == gameBoard[i-further][j])
                        {
                            if(max < k)
                            {
                                max = k;
                                if(further > 1 && k > 0)
                                    gameBoard[i-1][j] = baseScores[k];
                            }
                            break;
                        }
                    }
        }
        if(max > 0)
            break;
    }
    if(max == -1 || max == 0)
        return '1';
    else
        return baseScores[max];
}
//:::::::::::::::THE FUNCTION THAT EVALUATES ENEMY'S SURROUNDING:::::::::::::::::::::::::::::::::::::::
//:::::::::::::IF OPPONENT HAS A FREE SIDE, THIS SIDE WILL BE EVALUATED WITH A HIGHER SCORE::::::::::::
void enemyNeighbourEvaluationFunction(char gameBoard[][8], int i, int j, int maxPoint)
{
    int k,l,n;

    if(i+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
        for(k=i+1,l=maxPoint; (l>0)&&(k<=7); k++,l--)
            if((gameBoard[k][j] != indicators[userTurn-1]) && (gameBoard[k][j] != indicators[aiTurn-1] && gameBoard[k][j] != '@'&& gameBoard[k][j] != '#'))
            {
                if(gameBoard[k][j] == ' ')
                    gameBoard[k][j] = baseScores[l-1];
                else
                {
                    for(n=0;n<9;n++)
                        if(baseScores[n] == gameBoard[k][j])
                            if(n+1 < l){
                                gameBoard[k][j] = baseScores[l-1];
                                    break;
                            }
                }
            }else break;
        if(i-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            for(k=i-1,l=maxPoint; (l>0)&&(k>=1); k--,l--)
                if((gameBoard[k][j] != indicators[userTurn-1]) && (gameBoard[k][j] != indicators[aiTurn-1] && gameBoard[k][j] != '@'&& gameBoard[k][j] != '#'))
                {
                    if(gameBoard[k][j] == ' ')
                        gameBoard[k][j] = baseScores[l-1];
                    else
                    {
                        for(n=0;n<9;n++)
                            if(baseScores[n] == gameBoard[k][j])
                                if(n+1 < l){
                                    gameBoard[k][j] = baseScores[l-1];
                                    break;
                                }
                    }
                }else break;
        if(j+1 <= 7) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            for(k=j+1,l=maxPoint; (l>0)&&(k<=7); k++,l--)
                if((gameBoard[i][k] != indicators[userTurn-1]) && (gameBoard[i][k] != indicators[aiTurn-1] && gameBoard[i][k] != '@'&& gameBoard[i][k] != '#'))
                {
                    if(gameBoard[i][k] == ' ')
                        gameBoard[i][k] = baseScores[l-1];
                    else
                    {
                        for(n=0;n<9;n++)
                            if(baseScores[n] == gameBoard[i][k])
                                if(n+1 < l){
                                    gameBoard[i][k] = baseScores[l-1];
                                    break;
                                }
                    }
                }else break;
        if(j-1 >= 1) //CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
            for(k=j-1,l=maxPoint; (l>0)&&(k>=1); k--,l--)
                if((gameBoard[i][k] != indicators[userTurn-1]) && (gameBoard[i][k] != indicators[aiTurn-1] && gameBoard[i][k] != '@' && gameBoard[i][k] != '#'))
                {
                    if(gameBoard[i][k] == ' ')
                        gameBoard[i][k] = baseScores[l-1];
                    else
                    {
                        for(n=0;n<9;n++)
                            if(baseScores[n] == gameBoard[i][k])
                                if(n+1 < l){
                                    gameBoard[i][k] = baseScores[l-1];
                                    break;
                                }
                    }
                }else break;
}
//::::::::::THE FUNCTION THAT CLEARS THE MAP AND ALL THE EVALUATION VALUES OF BOTH USER AND AI:::::::::::::::::::::::::
//:::::::::::::::THIS FUNCTION WILL RE-EVALUATED EVERYTHING ON THE MAP ACCORDING TO THE NUMBER OF SPACES:::::::::::::::
void reEvaluationOfTheEnemyNeighbours(char gameBoard[][8], char enemyIndicator, int aiStucked)
{
    int i,j,k,n;
    for(i = 1; i <= 7; i++)
        for(j = 1; j <= 7; j++)
            if(gameBoard[i][j] != indicators[userTurn-1] && gameBoard[i][j] != indicators[aiTurn-1] && gameBoard[i][j] != '@' && gameBoard[i][j] != '#')
            {
                if(aiStucked == 0)
                    gameBoard[i][j] = ' ';
                else
                    gameBoard[i][j] = '1';
            }
    //IF AI NEEDS TO BE RESCUED, ITS NEIGHBOUR SURROUNDED BY ENEMIES MUST BE CHECKED FOR THE SAFETY OF THE AI
    if(aiStucked == 1)
    {
         enemyNeighbourEvaluationFunction(gameBoard,victimCoordinates[0],victimCoordinates[1], 9);//VICTIM

        for(k = 1; k <= 2; k++)
            for(i = 1; i <= 7; i++)
                for(j = 1; j <= 7; j++)
                    if((gameBoard[i][j] != indicators[userTurn-1]) && (gameBoard[i][j] != indicators[aiTurn-1]) && (gameBoard[i][j] != ' ') && gameBoard[i][j] != '@' && gameBoard[i][j] != '#')
                        for(n=0;n<9;n++)
                            if(baseScores[n] == gameBoard[i][j])
                            {
                                enemyNeighbourEvaluationFunction(gameBoard,i,j, n);
                                break;
                            }
    }
}
//::::::::::::::THIS FUNCTION IS AN EXTENSION OF THE "MINIMAX ALGORITHM" FUNCTION AND THIS IS A PART OF THE MINIMAX ALGORITHM::::::::::::::::::::::::::::::
//::::::::::::IT SELECTS HIGHEST VALUE FOR MAXIMISING PART AND LOWEST VALUE FOR MINIMISING PART OF THE EVALUATION VALUE (WHICH IS CALLED "SCORE")::::::::::
//::::::::::::::::::::::FURTHERMORE IT RE-EVALUATES THE gameBoard[i][j] != indicators[aiTurn-1]gameBoard[i][j] != indicators[aiTurn-1]gameBoard[i][j] != indicators[aiTurn-1]gameBoard[i][j] != indicators[aiTurn-1] BETA VALUE WHICH IS USED IN MINIMAX WITH ALPHA-BETA PRUNING::::::::::::::::::::::::::::::::::
void MinimaxSuccessorFunction(char nextBoard[][8], char currentBoard[][8], int *bestScore, int i_current, int j_current, int i_next, int j_next, int depth, int* alpha, int* beta, int isMaximising)
{
    int i,j;
    for(i = 0; i <= 7; i++)
        for(j = 0; j <= 7; j++)
            nextBoard[i][j] = currentBoard[i][j];

    if(isMaximising == 1) //EXTENSION OF MAXIMISING PROCESS WHICH WAS HELD IN "MINIMAX ALGORIHM" FUNCTION
    {
        nextBoard[i_next][j_next] = indicators[aiTurn-1]; //GENERATE THE NEXT STATE BY MOVING AI TO THE NEXT POSITION
        nextBoard[i_current][j_current] = ' '; //ERASE THE PREVIOUS POSITION
        scoreEvaluationFunction(nextBoard);
        isMaximising = 0; //THE STEP WILL BE MINIMISING PROCESS (IT WILL BE THE AI'S OPPONENT'S TURN)
        int score = MinimaxAlgorithm(nextBoard, depth+1, *alpha, *beta, isMaximising); //RETURN PLACE OF MINIMISING PART OF THE FUNCTION WILL BE HELD IN HERE
        if(score > *bestScore) //IF RETURNED VALUE IS BIGGER THAN BEST SCORE, MAKE IT THE NEW BEST SCORE
            *bestScore = score;
        if(*alpha > *bestScore) //IF ALPHA IS BIGGER THAN THE EVALUATION, UPDATE THE CURRENT ALPHA
            *alpha = score;
    }
    else  //EXTENSION OF MINIMISING PROCESS WHICH WAS HELD IN "MINIMAX ALGORIHM" FUNCTION
    {
        nextBoard[i_next][j_next] = indicators[userTurn-1]; //GENERATE THE NEXT STATE BY MOVING USER(AI) TO THE NEXT POSITION
        nextBoard[i_current][j_current] = ' '; //ERASE THE PREVIOUS POSITION
        scoreEvaluationFunction(nextBoard);
        isMaximising = 1;  //THE STEP WILL BE MAXIMISING PROCESS (IT WILL BE THE AI'S OPPONENT'S TURN)
        int score = MinimaxAlgorithm(nextBoard, depth+1, *alpha, *beta,  isMaximising);  //RETURN VALUE OF MINIMISING FUNCTION WILL BE HELD IN HERE
        if(score < *bestScore) //IF RETURNED VALUE IS SMALLER THAN BEST SCORE, MAKE IT THE NEW BEST SCORE
            *bestScore = score;
        if(*beta < *bestScore) //IF BETA IS SMALLER THAN THE EVALUATION, UPDATE THE CURRENT BETA
            *beta = score;
    }
}
//:::::::::::::::::::::::::::::::::THIS IS MY MAIN MINIMAX FUNCTION::::::::::::::::::::::::::::::::::::::::::::::::::::
//:::::::::::::::THIS FUNCTION BOTH CONTAINS MAXIMISING AND MINIMISING PROCESSES:::::::::::::::::::::::::::::::::::::::
//::::::::::::::::::ALSO THIS FUNCTION CALLS MINIMAXSUCCESSORFUNCTION FOR SOME CALCULATIONS & EVALUATIONS::::::::::::::
int MinimaxAlgorithm(char gameBoard[][8], int depth, int alpha, int beta, int isMaximising)
{
    scoreEvaluationFunction(gameBoard);
    if(depth == 6)
    {
        if(isMaximising == 0)
            return offensivesafeAIScore; //RETURNS EVAUATED OFFENSIVE AI SCORE AT THAT TIME
        else
            return offensivesafeUserScore; //RETURNS EVAUATED OFFENSIVE USER SCORE AT THAT TIME
    }
    if(isMaximising == 1) //MAXIMISING PROCESS WILL BE HELD IN HERE
    {
        int bestScore = offensivesafeAIScore; //THIS REPRESENTS -INFINITY
        char successorArray[8][8]; //THIS WILL HOLD THE NEXT STEP'S ENVIRONMENT AND STATE
        int i,j;
        for(i = 1; i <= 7; i++)
        {
            for(j = 1; j <= 7; j++)
            {
                if(gameBoard[i][j] == indicators[aiTurn-1]) //IF SELECTED STONE IS AI, APPLY MAXIMISING
                {
                    if(i+1 <= 7) //GOTO DOWN// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i+1, j, depth, &alpha, &beta, 1);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED ALPHA IS BIGGER THAN CURRENT BETA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(j+1 <= 7) //GOTO RIGHT// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i, j+1, depth, &alpha, &beta, 1);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED ALPHA IS BIGGER THAN CURRENT BETA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(j-1 >= 1) //GOTO LEFT// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i, j-1, depth, &alpha, &beta, 1);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED ALPHA IS BIGGER THAN CURRENT BETA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(i-1 >= 1) //GOTO UP// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i-1, j, depth, &alpha, &beta, 1);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED ALPHA IS BIGGER THAN CURRENT BETA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                }
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = offensivesafeUserScore; //THIS REPRESENTS +INFINITY
        char successorArray[8][8]; //THIS WILL HOLD THE NEXT STEP'S ENVIRONMENT
        int i,j;
        for(i = 1; i <= 7; i++)
        {
            for(j = 1; j <= 7; j++)
            {
                if(gameBoard[i][j] == indicators[userTurn-1]) //IF SELECTED STONE IS USER-AI, APPLY MINIMISING
                {
                    if(i+1 <= 7) //GOTO DOWN// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i+1, j, depth, &alpha, &beta, 0);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED BETA IS SMALLER THAN CURRENT ALPHA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(j+1 <= 7) //GOTO RIGHT// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i, j+1, depth, &alpha, &beta, 0);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED BETA IS SMALLER THAN CURRENT ALPHA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(j-1 >= 1) //GOTO LEFT// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i, j-1, depth, &alpha, &beta, 0);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED BETA IS SMALLER THAN CURRENT ALPHA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                    if(i-1 >= 1) //GOTO UP// AND ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                    {
                        if(gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != indicators[aiTurn-1]) //IF THE NEXT POSITION IS ALREADY OCCUPIED, SKIP
                        {
                            MinimaxSuccessorFunction(successorArray,gameBoard, &bestScore, i, j, i-1, j, depth, &alpha, &beta, 0);
                            if(beta <= alpha) //AT THE END OF THE FUNCTION CALL, IF EVALUATED BETA IS SMALLER THAN CURRENT ALPHA, SEARCHING PROCESS IS OVER, EXIT !!!
                                return bestScore;
                        }
                    }
                }
            }
        }
        return bestScore;
    }
}
//:::::::::::::::::::::THIS FUNCTION ENSURES THAT AI WILL NEVER GO BACK TO THE PREVIOUS STATE AFTER ONE STEP:::::::::::::::::::::::::::::
//::::::::::::::THANKS TO THIS FUNCTION, SOME INEFFICIENT PATHS ARE DISABLED AND IT WILL MINIMIZE THE EXECUTION PROCESS !!!!!::::::::::::
//::::::::::::::::::::::::LONG STORY SHORT, IT CLOSES THE PREVIOUS STATE TO INCREASE THE SPEED OF CALCULATION::::::::::::::::::::::::::::
int checkHistory(int i_next, int j_next)
{
    if(prevXY[0]==i_next && prevXY[1] == j_next)
        return 0;
    return 1;
}
//::::::::::::THIS IS THE FUNCTION WHERE AI CALLS MINIMAX ALGORITHM AND GENERATES THE FIRST NEXT NODES FOR EACH DIRECTION::::::::::::::::
void rootAISuccessorFunction(char nextBoard[][8], char currentBoard[][8], int move[], int* bestScore, int* aiStucked, int i_current, int j_current, int i_next, int j_next)
{
    int i,j;
    for(i = 0; i <= 7; i++)
        for(j = 0; j <= 7; j++)
            nextBoard[i][j] = currentBoard[i][j];
    //IF OUR AI IS NOT IN DANGER AND IT CAN MOVE FREELY, MINIMAX FUNCTION CAN BE CALLED WITHOUT ANY PROBLEM
    if(*aiStucked == 1)
    {
        nextBoard[i_next][j_next] = indicators[aiTurn-1]; //GENERATE THE NEXT STATE
        nextBoard[i_current][j_current] = ' ';
        scoreEvaluationFunction(nextBoard);

        int score = MinimaxAlgorithm(nextBoard, 1, offensivesafeAIScore, offensivesafeUserScore, 1); // RETURN VALUE OF MINIMAX FUNCTION WILL BE HELD IN HERE
        if(score > *bestScore) //IF RETURNED VALUE IS BIGGER THAN BEST SCORE, MAKE IT THE NEW BEST SCORE AND MAKE IT THE NEXT STATE CANDIDATE
        {
            *bestScore = score;
            *aiStucked = 0;
            move[0] = i_current; move[1] = j_current; //STORE THE CURRENT INDEX VALUE
            move[2] = i_next; move[3] = j_next; //STORE THE NEXT INDEX VALUE
        }
    }
    //IF OUR AI IS ABOUT THE STUCK, TAKE THE AI FOR THE SAFEST PLACE ACCORDING TO THE EVALUATION FUNCTION
    //AND THEN CALL THIS FUNCTION (rootAISuccessorFunction) AGAIN TO CALL MINIMAX ALGORITHM THE NEXT TIME
    else if(*aiStucked == -1)
    {
        int isThisStateSafe;
        for(i = 0; i < 10; i++)
            if(safetyRate[i] == nextBoard[i_next][j_next])
            {
                isThisStateSafe = i;
                break;
            }
        if(isThisStateSafe >= *bestScore) //IF IT IS SAFE, CONFIRM THAT THIS WILL CAN BE THE NEXT STATE FREELY
        {
            *bestScore = isThisStateSafe;
            move[0] = i_current; move[1] = j_current; //STORE THE CURRENT INDEX VALUE
            move[2] = i_next; move[3] = j_next; //STORE THE NEXT INDEX VALUE
        }
    }
}
void rootAIAttemptsToMove(char gameBoard[][8])
{
    scoreEvaluationFunction(gameBoard); //FUNCTION THAT EVALUATES CURRENT STATE'S SCORE FOR BOTH AI AND USER
    int bestScore = offensivesafeAIScore; //THIS IS MY -INFINITY
    int move[4] = {-1,-1,-1,-1}; //i_current, j_current, i_next, j_next
    char successorArray[8][8]; //THIS WILL HOLD THE NEXT STEP'S ENVIRONMENT
    int i,j;
    int aiStucked = 1;
    int checkSafety = 0;
    char mostPromisingPath = '\0';
    determineVictimTarget(gameBoard);
    for(i = 1; i <= 7; i++)
    {
        for(j = 1; j <= 7; j++)
        {
            if(gameBoard[i][j] == indicators[aiTurn-1]) //IF SELECTED STONE IS AI, THEN FIND THE MOST OPTIMAL STONE
            {
                mostPromisingPath = mapEvaluationFunction(gameBoard,indicators[aiTurn-1], i,j);
                if(mostPromisingPath != '\0')
                {
                    //GOTO DOWN (IF THE MINIMAX FUNCTION RETURNS THE HIGHEST SCORE)
                    //CHECK IF THE NEXT STATE IS OUR PREVIOUS STATE. IF SO, CLOSE THAT STATE TO REDUCE EXECUTION TIME
                    if((i+1 <= 7) && checkHistory(i+1,j)) //ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i+1][j] != indicators[userTurn-1] && gameBoard[i+1][j] != indicators[aiTurn-1] && gameBoard[i+1][j] != '@' && gameBoard[i+1][j] != '#')
                            rootAISuccessorFunction(successorArray,gameBoard, move, &bestScore, &aiStucked, i, j, i+1, j);
                    //GOTO RIGHT (IF THE MINIMAX FUNCTION RETURNS THE HIGHEST SCORE)
                    //CHECK IF THE NEXT STATE IS OUR PREVIOUS STATE. IF SO, CLOSE THAT STATE TO REDUCE EXECUTION TIME
                    if((j+1 <= 7) && checkHistory(i,j+1)) //ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j+1] != indicators[userTurn-1] && gameBoard[i][j+1] != indicators[aiTurn-1] && gameBoard[i][j+1] != '@' && gameBoard[i][j+1] != '#')
                            rootAISuccessorFunction(successorArray,gameBoard, move, &bestScore, &aiStucked, i, j, i, j+1);
                    //GOTO LEFT (IF THE MINIMAX FUNCTION RETURNS THE HIGHEST SCORE)
                    //CHECK IF THE NEXT STATE IS OUR PREVIOUS STATE. IF SO, CLOSE THAT STATE TO REDUCE EXECUTION TIME
                    if((j-1 >= 1) && checkHistory(i,j-1)) //ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i][j-1] != indicators[userTurn-1] && gameBoard[i][j-1] != indicators[aiTurn-1] && gameBoard[i][j-1] != '@' && gameBoard[i][j-1] != '#')
                            rootAISuccessorFunction(successorArray,gameBoard, move, &bestScore, &aiStucked, i, j, i, j-1);
                    //GOTO UP (IF THE MINIMAX FUNCTION RETURNS THE HIGHEST SCORE)
                    //CHECK IF THE NEXT STATE IS OUR PREVIOUS STATE. IF SO, CLOSE THAT STATE TO REDUCE EXECUTION TIME
                        if((i-1 >= 1) && checkHistory(i-1,j)) //ALSO CHECK IF IT IS THE LIMIT OF THE BOARD OR NOT
                        if(gameBoard[i-1][j] != indicators[userTurn-1] && gameBoard[i-1][j] != indicators[aiTurn-1] && gameBoard[i-1][j] != '@' && gameBoard[i-1][j] != '#')
                            rootAISuccessorFunction(successorArray,gameBoard, move, &bestScore, &aiStucked, i, j, i-1, j);
                }
                else
                    aiStucked = 1;
            }
        }
        if((aiStucked == 1 || checkSafety == 0) && i == 7) //IF THE AI IS ABOUT TO BE STUCK, YOU NEED TO SAVE IT BEFORE IT BEFORE IT IS CAPTURED
        {
            reEvaluationOfTheEnemyNeighbours(gameBoard, indicators[userTurn-1], 1); //DELETE ALL THE CALCULATED EVALUATIONS AND THEN RE-EVALUATE
            i = 0;
            checkSafety = 1;
            bestScore = 0;
            aiStucked = -1; //THIS IS THE SIGN OF THE AI NEEDS TO BE RESCUED
        }
    }
    if(move[0] == -1) //IF THERE IS NO MOVEMENT OPERATION THAT MEANS AI IS HOLDING ALL THE ENEMY STONES, THE GAME IS OVER
        isGameOver = 1;
    else //AFTER THE MINIMAX FUNCTION, GENERATED NEW STATE MUST BE ACTIVATED ON OUR MAIN GAMEBOARD
    {
        gameBoard[move[0]][move[1]] = ' '; //DELETE THE AI FROM THE PREVIOUS STATE
        gameBoard[move[2]][move[3]] = indicators[aiTurn-1]; //GENERATE THE AI ON THE NEXT STATE
        turnNumbers[aiTurn-1]--; //DECREASE THE NUMBER OF TURNS FOR THE AI
        prevXY[0] = move[0]; prevXY[1] = move[1]; //SAVE THE PREVIOUS POSITION'S COORDINATE SO THAT WE DON'T SELECT IT WHEN WE MAKE DECISIONS ON THE NEXT STATE
        printGameBoard(gameBoard);
        reEvaluationOfTheEnemyNeighbours(gameBoard,indicators[userTurn-1],0);
        printf("\t\t Computer moves the piece [%c] at %s to %s\n", indicators[aiTurn-1], getCoordinates(move[0],move[1]), getCoordinates(move[2],move[3]));
    }
}
//:::::::::THE FUNCTION THAT EXECUTES USER PLAYER'S MOVEMENTS AND CHECKS IF THE GIVEN COORDINATES ARE SUITABLE OR NOT::::::::::
int userAttemptsToMove(char gameBoard[][8])
{
    int i;
    int from_i,from_j,to_i, to_j;
    char row[8] = "abcdefg";
    char column[8] = "1234567";
    //EXTRACT THE CURRENT (X) COORDINATE
    for(i = 0; i < 8; i++)
        if(row[i] == fromIndex[0])
        {
            from_i = i+1;
            break;
        }
    //EXTRACT THE CURRENT (Y) COORDINATE
    for(i = 0; i < 8; i++)
        if(column[i] == fromIndex[1])
        {
            from_j = i+1;
            break;
        }
    //EXTRACT THE NEXT (X) COORDINATE
    for(i = 0; i < 8; i++)
        if(row[i] == toIndex[0])
        {
            to_i = i+1;
            break;
        }
    //EXTRACT THE NEXT (Y) COORDINATE
    for(i = 0; i < 8; i++)
        if(column[i] == toIndex[1])
        {
            to_j = i+1;
            break;
        }

    //IF THE SELECTED PATH IS OUT OF THE BOARD, RETURN FALSE
    if((to_i < 1 || to_i > 7) || (to_j < 1 || to_j > 7) || (from_i < 1 || from_i > 7) || (from_j < 1 || from_j > 7))
        return 0;
    //IF THE WRONG STONE IS PICKED OR THE NEXT POSITION IS ALREADY OCCUPIED, RETURN FALSE
    else if((gameBoard[from_i][from_j] != indicators[userTurn-1]) || (gameBoard[to_i][to_j] == indicators[userTurn-1] || gameBoard[to_i][to_j] == indicators[aiTurn-1]))
        return 0;
    //IF THE DISTANCE BETWEEN CURRENT AND NEXT STATE IS 1, THEN THE TRANSPORTATION IS ACCEPTABLE !!!
    else if(((from_i == to_i && (abs(from_j - to_j) == 1)) || (from_j == to_j && (abs(from_i - to_i) == 1))) && (gameBoard[to_i][to_j] != indicators[userTurn-1] || gameBoard[to_i][to_j] != indicators[aiTurn-1]))
    {
        gameBoard[to_i][to_j] = indicators[userTurn-1];
        gameBoard[from_i][from_j] = ' ';
        return 1;
    }
    //OR ELSE RETURN FALSE
    else
        return 0;
}
int main()
{
    system("color B1"); //ENVIRONMENT OF THE GAME COLOR IS SET TO BLUE
    printf("\n\n\t\t\t\tWanna be Player[1] or Player[2] (Type 1 or 2): ");
    scanf("%d", &userTurn);
    if(userTurn == 2)
        aiTurn = 1;
    printf("\n\n\t\t\t\t\t     Number of [Pieces]: ");
    scanf("%d", &pieceNumber);
    printf("\n\n\t\t\t\t\t     Number of [Turn Limits]: ");
    scanf("%d", &turnNumbers[0]); turnNumbers[1] = turnNumbers[0];

    //THIS IS OUR GAMEBOARD THAT WILL BE FILLED BY USING SYMMETRIC RANDOMIZATION (RELATIVE TO THE ORIGIN)
    char gameBoard[8][8] = {
        {' ','1', '2', '3', '4', '5', '6', '7'},
        {'a',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'b',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'c',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'d',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'e',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'f',' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'g',' ', ' ', ' ', ' ', ' ', ' ', ' '},
    };

    //FILL THE MAP WITH USER & AI STONES RANDOMLY AND EQUALLY (SYMMETRIC)
    srand(time(0));
    int i,i_rand, j_rand;
    for(i = 0; i < pieceNumber; i++)
    {
        i_rand = rand()%7+1;
        j_rand = rand()%7+1;
        if(gameBoard[i_rand][j_rand] == ' ')
        {
            gameBoard[i_rand][j_rand] = 'O';
            gameBoard[abs(i_rand - 8)][abs(j_rand - 8)] = 'X';
        }
        else
            i--;
    }

    printGameBoard(gameBoard);
    scoreEvaluationFunction(gameBoard);
    for(;isGameOver == 0;) //LET THE GAME CONTINUE UNTIL IT IS OVER
    {
        //IF ANY OF THE PLAYERS CANNOT MOVE THAT MEANS THE GAME IS OVER
        if(safeAIScore == 0 || safeUserScore == 0)
            isGameOver = 1;
        if(currentTurn == userTurn && isGameOver == 0) //IT IS USER'S TURN TO MAKE A CHOICE
        {
            if(turnNumbers[userTurn-1] > 0)
            {
                while(1)
                {
                    printf("\nChoose piece to move [%c]: ", indicators[userTurn-1]);
                    scanf("%s", fromIndex);
                    printf("Choose the new position for %s: ", fromIndex);
                    scanf("%s", toIndex);
                    if(userAttemptsToMove(gameBoard) == 1) //IF MOVEMENT COMMAND IS APPLIABLE THEN GO AHEAD
                    {
                        turnNumbers[userTurn-1]--;
                        printGameBoard(gameBoard);
                        printf("\t\t    Player moves the piece at %s to %s\n", fromIndex, toIndex);
                        break;
                    }
                    else
                        printf("\t\t\t Invalid movement !\n");
                }
            }
            currentTurn = aiTurn;
        }
        //IT IS AI'S TURN TO MAKE A CHOICE
        else if(currentTurn == aiTurn && turnNumbers[aiTurn-1] > 0 && isGameOver == 0)
        {
            if(turnNumbers[aiTurn-1] > 0)
                rootAIAttemptsToMove(gameBoard);
            currentTurn = userTurn;
        }
        //GAME OVER INFORMATION WILL BE DECLARED IN HERE
        if((turnNumbers[userTurn-1] == 0 && turnNumbers[aiTurn-1] == 0) || safeAIScore == 0 || safeUserScore == 0 || isGameOver == 1)
        {
            isGameOver = 1;
            if(safeUserScore > safeAIScore)
                printf("\n\t\t [Game Over]:  Player[%d]  < User >  wins !\n\n", userTurn);
            else if(safeUserScore < safeAIScore)
                printf("\n\t\t  [Game Over]:  Player[%d]  < AI >  wins !\n\n", aiTurn);
            else
                printf("\n\t\t\t[Game Over]: It is a draw !\n\n");
            break;
        }
    }
    system("pause");
    return 0;
}
