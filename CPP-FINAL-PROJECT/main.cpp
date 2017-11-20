//
//  main.cpp
//  CPP-FINAL-PROJECT
//
//  Created by Keith Smith on 11/18/17.
//  Copyright © 2017 Keith Smith. All rights reserved.
//
//  Battleship

#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <cctype>

using namespace std;

const int INT_BOARD_SIZE = 12;
int intPlayerHits = 0;
int intComputerHits = 0;

int randomInt(int, int);
void initializeBoard(char [][INT_BOARD_SIZE]);
void drawBoard(char [][INT_BOARD_SIZE], char [][INT_BOARD_SIZE]);
void spawnShips(char [][INT_BOARD_SIZE]);
void playerTurn(char [][INT_BOARD_SIZE], char [][INT_BOARD_SIZE]);
void computerTurn(char [][INT_BOARD_SIZE]);
bool *searchForHits(char [][INT_BOARD_SIZE], int *, int *);

bool *validateMove(string *, char[][INT_BOARD_SIZE]);
bool *validateMove(int *, int *, char[][INT_BOARD_SIZE]);
int convertMoveLetter(char);
int convertMoveNumber(char);

int main()
{
    char chrBoardPlayer[INT_BOARD_SIZE][INT_BOARD_SIZE];
    char chrBoardComputerVisible[INT_BOARD_SIZE][INT_BOARD_SIZE];
    char chrBoardComputerNonVisible[INT_BOARD_SIZE][INT_BOARD_SIZE];
    
    initializeBoard(chrBoardPlayer);
    initializeBoard(chrBoardComputerVisible);
    initializeBoard(chrBoardComputerNonVisible);
    
    spawnShips(chrBoardPlayer);
    spawnShips(chrBoardComputerNonVisible);
    
    drawBoard(chrBoardPlayer, chrBoardComputerVisible);

    while (intPlayerHits < 15 && intComputerHits < 15)
    {
        playerTurn(chrBoardComputerVisible, chrBoardComputerNonVisible);
        drawBoard(chrBoardPlayer, chrBoardComputerVisible);
        
        if (intPlayerHits != 15)
        {
            computerTurn(chrBoardPlayer);
            drawBoard(chrBoardPlayer, chrBoardComputerVisible);
        }
    }
    
    if (intPlayerHits == 15)
    {
        cout << endl << "Player wins!";
    }
    else if (intComputerHits == 15)
    {
        cout << endl << "Computer wins!";
    }
    
    cout << endl << endl << "GAME OVER" << endl << endl;
    
    return 0;
}

int randomInt(int minValue, int maxValue)
{
    int random;
    
    // Use random_device to generate seed
    random_device rd;
    
    // Use seed to set Mersenne twister engine
    mt19937 engine(rd());
    
    // Define distribution and type
    uniform_int_distribution<> distribution(minValue, maxValue);
    
    // Generate a result in range of distribution with MTE engine
    random = distribution(engine);
    
    return random;
}

void initializeBoard(char board[][INT_BOARD_SIZE])
{
    for (int i = 0 ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = 0 ; j < INT_BOARD_SIZE ; j++)
        {
            board[i][j] = '*';
        }
    }
    
    for (int i = 0 ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = 1 ; j < 2 ; j++)
        {
            board[i][j] = '|';
        }
    }
    for (int i = 0 ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = (INT_BOARD_SIZE - 1) ; j < INT_BOARD_SIZE ; j++)
        {
            board[i][j] = '|';
        }
    }
    
    for (int i = 1 ; i < 2 ; i++)
    {
        for (int j = 0 ; j < INT_BOARD_SIZE ; j++)
        {
            board[i][j] = '-';
        }
    }
    for (int i = (INT_BOARD_SIZE - 1) ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = 0 ; j < INT_BOARD_SIZE ; j++)
        {
            board[i][j] = '-';
        }
    }
    
    board[0][0] = ' ';
    board[1][0] = ' ';
    board[0][1] = ' ';
    board[0][INT_BOARD_SIZE - 1] = ' ';
    board[INT_BOARD_SIZE - 1][0] = ' ';
    
    board[1][1] = '0';
    board[1][INT_BOARD_SIZE - 1] = '0';
    board[INT_BOARD_SIZE - 1][1] = '0';
    board[INT_BOARD_SIZE - 1][INT_BOARD_SIZE - 1] = '0';
    
    board[0][2] = '1';
    board[0][3] = '2';
    board[0][4] = '3';
    board[0][5] = '4';
    board[0][6] = '5';
    board[0][7] = '6';
    board[0][8] = '7';
    board[0][9] = '8';
    board[0][10] = '9';
    
    board[2][0] = 'A';
    board[3][0] = 'B';
    board[4][0] = 'C';
    board[5][0] = 'D';
    board[6][0] = 'E';
    board[7][0] = 'F';
    board[8][0] = 'G';
    board[9][0] = 'H';
    board[10][0] = 'I';
}

void drawBoard(char boardPlayer[][INT_BOARD_SIZE], char boardComputer[][INT_BOARD_SIZE])
{
    
    cout << endl << endl << endl << endl << endl << endl;
    cout << endl << endl << endl << endl << endl << endl;
    cout << endl << endl << endl << endl << endl << endl;
    cout << endl << endl << endl << endl << endl << endl;
    
    cout << setw(16) << "COMPUTER" << endl;
    for (int i = 0 ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = 0 ; j < INT_BOARD_SIZE ; j++)
        {
            cout << boardComputer[i][j];
            cout << ' ';
        }
        cout << endl;
    }
    
    cout << endl;
    
    cout << setw(15) << "PLAYER" << endl;
    for (int i = 0 ; i < INT_BOARD_SIZE ; i++)
    {
        for (int j = 0 ; j < INT_BOARD_SIZE ; j++)
        {
            cout << boardPlayer[i][j];
            cout << ' ';
        }
        cout << endl;
    }
}

void spawnShips(char board[][INT_BOARD_SIZE])
{
    int intNumShips = 5;
    int intPointOne;
    int intPointTwo;
    
    int intDirection;
    
    bool booIsValid = false;
    
    while (intNumShips > 0)
    {
        while (!booIsValid)
        {
            intPointOne = randomInt(2, INT_BOARD_SIZE - 1);
            intPointTwo = randomInt(2, INT_BOARD_SIZE - 1);
            
            
            if (board[intPointOne][intPointTwo] != 'S')
            {
                intDirection = randomInt(1, 4);
                
                if (intDirection == 1)
                {
                    if (board[intPointOne][intPointTwo] == '*' && board[intPointOne - 1][intPointTwo] == '*' && board[intPointOne - 2][intPointTwo] == '*')
                    {
                        board[intPointOne][intPointTwo] = 'S';
                        board[intPointOne - 1][intPointTwo] = 'S';
                        board[intPointOne - 2][intPointTwo] = 'S';
                        booIsValid = true;
                    }
                }
                else if (intDirection == 2)
                {
                    if (board[intPointOne][intPointTwo] == '*' && board[intPointOne + 1][intPointTwo] == '*' && board[intPointOne + 2][intPointTwo] == '*')
                    {
                        board[intPointOne][intPointTwo] = 'S';
                        board[intPointOne + 1][intPointTwo] = 'S';
                        board[intPointOne + 2][intPointTwo] = 'S';
                        booIsValid = true;
                    }
                    
                }
                else if (intDirection == 3)
                {
                    if (board[intPointOne][intPointTwo] == '*' && board[intPointOne][intPointTwo - 1] == '*' && board[intPointOne][intPointTwo - 2] == '*')
                    {
                        board[intPointOne][intPointTwo] = 'S';
                        board[intPointOne][intPointTwo - 1] = 'S';
                        board[intPointOne][intPointTwo - 2] = 'S';
                        booIsValid = true;
                    }
                }
                else if (intDirection == 4)
                {
                    if (board[intPointOne][intPointTwo] == '*' && board[intPointOne][intPointTwo + 1] == '*' && board[intPointOne][intPointTwo + 2] == '*')
                    {
                        board[intPointOne][intPointTwo] = 'S';
                        board[intPointOne][intPointTwo + 1] = 'S';
                        board[intPointOne][intPointTwo + 2] = 'S';
                        booIsValid = true;
                    }
                }
            }
        }
        booIsValid = false;
        intNumShips--;
    }
}

void playerTurn(char computerVisible[][INT_BOARD_SIZE], char computerNonVisible[][INT_BOARD_SIZE])
{
    string *strSelection = nullptr;
    strSelection = new string;
    
    int *intPointOne = nullptr;
    intPointOne = new int;
    int *intPointTwo = nullptr;
    intPointTwo = new int;
    
    cout << "Please choose a tile: ";
    cin >> *strSelection;
    while (*validateMove(strSelection, computerVisible) == false)
    {
        cout << "Please enter a valid selection: ";
        cin.clear();
        cin.ignore();
        cin >> *strSelection;
    }
    
    *intPointOne = convertMoveLetter(strSelection->at(0));
    *intPointTwo = convertMoveNumber(strSelection->at(1));
    
    if (computerNonVisible[*intPointOne][*intPointTwo] == '*')
    {
        computerVisible[*intPointOne][*intPointTwo] = 'O';
        computerNonVisible[*intPointOne][*intPointTwo] = 'O';
    }
    else if (computerNonVisible[*intPointOne][*intPointTwo] == 'S')
    {
        computerVisible[*intPointOne][*intPointTwo] = 'X';
        computerNonVisible[*intPointOne][*intPointTwo] = 'X';
        intPlayerHits++;
    }
}

void computerTurn(char playerBoard[][INT_BOARD_SIZE])
{
    bool *isValid = nullptr;
    isValid = new bool;
    
    
    int *intPointOne = nullptr;
    intPointOne = new int;
    int *intPointTwo = nullptr;
    intPointTwo = new int;
    
    int pointOne;
    int pointTwo;
    
    isValid = searchForHits(playerBoard, &pointOne, &pointTwo);
    
    if (*isValid == true && validateMove(&pointOne, &pointTwo, playerBoard))
    {
        *intPointOne = pointOne;
        *intPointTwo = pointTwo;
    }
    else if (*isValid == true)
    {
        while (*validateMove(&pointOne, &pointTwo, playerBoard) != true)
        {
            *isValid = searchForHits(playerBoard, &pointOne, &pointTwo);
        }
    }
    
    while (*isValid != true)
    {
        *intPointOne = randomInt(2, INT_BOARD_SIZE - 1);
        *intPointTwo = randomInt(2, INT_BOARD_SIZE - 1);
        
        *isValid = *validateMove(intPointOne, intPointTwo, playerBoard);
    }
    
    if (playerBoard[*intPointOne][*intPointTwo] == '*')
    {
        playerBoard[*intPointOne][*intPointTwo] = 'O';
    }
    else if (playerBoard[*intPointOne][*intPointTwo] == 'S')
    {
        playerBoard[*intPointOne][*intPointTwo] = 'X';
        intComputerHits++;
    }
}

bool *searchForHits(char playerBoard[][INT_BOARD_SIZE], int *pointOne, int *pointTwo)
{
    bool *booIsValid = nullptr;
    booIsValid = new bool;
    *booIsValid = false;
    
    for (int i = 2 ; i < (INT_BOARD_SIZE - 1) ; i++)
    {
        for (int j = 2 ; j < (INT_BOARD_SIZE - 1) ; j++)
        {
            if (playerBoard[i][j] == 'X')
            {
                if (playerBoard[i - 1][j] == 'X')
                {
                    if (playerBoard[i - 2][j] == 'X')
                    {
                        *booIsValid = false;
                        continue;
                    }
                    else if (playerBoard[i - 2][j] == '*' || playerBoard[i - 2][j] == 'S')
                    {
                        *pointOne = i - 2;
                        *pointTwo = j;
                        *booIsValid = true;
                        break;
                    }
                    
                    *booIsValid = false;
                }
                else if (playerBoard[i - 1][j] == '*' || playerBoard[i - 1][j] == 'S')
                {
                    *pointOne = i - 1;
                    *pointTwo = j;
                    *booIsValid = true;
                    break;
                }
                
                if (playerBoard[i][j - 1] == 'X')
                {
                    if (playerBoard[i][j - 2] == 'X')
                    {
                        *booIsValid = false;
                        continue;
                    }
                    else if (playerBoard[i][j - 2] == '*' || playerBoard[i][j - 2] == 'S')
                    {
                        *pointOne = i;
                        *pointTwo = j - 2;
                        *booIsValid = true;
                        break;
                    }
                    
                    *booIsValid = false;
                }
                else if (playerBoard[i][j - 1] == '*' || playerBoard[i][j - 1] == 'S')
                {
                    *pointOne = i;
                    *pointTwo = j - 1;
                    *booIsValid = true;
                    break;
                }
                
                if (playerBoard[i][j + 1] == 'X')
                {
                    if (playerBoard[i][j + 2] == 'X')
                    {
                        *booIsValid = false;
                        continue;
                    }
                    else if (playerBoard[i][j + 2] == '*' || playerBoard[i][j + 2] == 'S')
                    {
                        *pointOne = i;
                        *pointTwo = j + 2;
                        *booIsValid = true;
                        break;
                    }
                    
                    *booIsValid = false;
                }
                else if (playerBoard[i][j + 1] == '*' || playerBoard[i][j + 1] == 'S')
                {
                    *pointOne = i;
                    *pointTwo = j + 1;
                    *booIsValid = true;
                    break;
                }
                
                if (playerBoard[i + 1][j] == 'X')
                {
                    if (playerBoard[i + 2][j] == 'X')
                    {
                        *booIsValid = false;
                        continue;
                    }
                    else if (playerBoard[i + 2][j] == '*' || playerBoard[i + 2][j] == 'S')
                    {
                        *pointOne = i + 2;
                        *pointTwo = j;
                        *booIsValid = true;
                        break;
                    }
                    
                    *booIsValid = false;
                }
                else if (playerBoard[i + 1][j] == '*' || playerBoard[i + 1][j] == 'S')
                {
                    *pointOne = i + 1;
                    *pointTwo = j;
                    *booIsValid = true;
                    break;
                }
            }
        }
    }
    
    return booIsValid;
}

bool *validateMove(string *move, char computerVisible[][INT_BOARD_SIZE])
{
    bool *isValid = nullptr;
    isValid = new bool;
    
    int *intPointOne = nullptr;
    intPointOne = new int;
    int *intPointTwo = nullptr;
    intPointTwo = new int;
    
    if (move->size() == 2 && isalpha(move->at(0)) == true && isdigit(move->at(1)) == true)
    {
        move->at(0) = toupper(move->at(0));
        
        if (move->at(0) >= 'A' && move->at(0) <= 'I' && move->at(1) >= '1' && move->at(1) <= '9')
        {
            *intPointOne = convertMoveLetter(move->at(0));
            *intPointTwo = convertMoveNumber(move->at(1));
            
            if (computerVisible[*intPointOne][*intPointTwo] == '*')
            {
                *isValid = true;
            }
            else
            {
                *isValid = false;
            }
        }
        else
        {
            *isValid = false;
        }
    }
    else
    {
        *isValid = false;
    }
    
    return isValid;
}

bool *validateMove(int *pointOne, int *pointTwo, char playerBoard[][INT_BOARD_SIZE])
{
    bool *booIsValid = nullptr;
    booIsValid = new bool;
    *booIsValid = false;
    
    if (playerBoard[*pointOne][*pointTwo] == '*' || playerBoard[*pointOne][*pointTwo] == 'S')
    {
        *booIsValid = true;
    }
    else if (playerBoard[*pointOne][*pointTwo] == 'O' || playerBoard[*pointOne][*pointTwo] == 'X')
    {
        *booIsValid = false;
    }
    
    return booIsValid;
}

int convertMoveLetter(char letter)
{
    int intPoint;
    
    switch(letter)
    {
        case('A'):
            intPoint = 2;
            break;
        case('B'):
            intPoint = 3;
            break;
        case('C'):
            intPoint = 4;
            break;
        case('D'):
            intPoint = 5;
            break;
        case('E'):
            intPoint = 6;
            break;
        case('F'):
            intPoint = 7;
            break;
        case('G'):
            intPoint = 8;
            break;
        case('H'):
            intPoint = 9;
            break;
        case('I'):
            intPoint = 10;
            break;
        default:
            intPoint = 0;
            break;
    }
    
    return intPoint;
}

int convertMoveNumber(char number)
{
    int intPoint;
    
    switch(number)
    {
        case('1'):
            intPoint = 2;
            break;
        case('2'):
            intPoint = 3;
            break;
        case('3'):
            intPoint = 4;
            break;
        case('4'):
            intPoint = 5;
            break;
        case('5'):
            intPoint = 6;
            break;
        case('6'):
            intPoint = 7;
            break;
        case('7'):
            intPoint = 8;
            break;
        case('8'):
            intPoint = 9;
            break;
        case('9'):
            intPoint = 10;
            break;
        default:
            intPoint = 0;
            break;
    }
    
    return intPoint;
}
