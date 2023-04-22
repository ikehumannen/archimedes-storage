/*
5
*/
#include <iostream>     //needed for cin and cout

using namespace std;

const char NEW_BOARD[10] = {'0','1','2','3','4','5','6','7','8','9'};       //blank game board

char playerMark[3] = {'#','X','O'};            //pound symbol stands in for zero as 1(X) and 2(O) are the only ones being looked at

string winPos;      //set equal to the way the player won the game ie: on the top row

//FUNCTION DECLARATIONS

//calls drawBoard to display board, gets user input and verifies it, checks if game is still going and switches players
int playGame (char currentBoard[]);

//draws board when called with current marker placements for move consideration
void drawBoard(char currentBoard[]);

//checks if any winning conditions have been met, if all spaces have been used and if there is a draw.
int checkWin(char currentBoard[], string &winPos);

//clears bad inputs
void clearInput(char input);

//END OF FUNCTION DECLARATIONS

int main()
{          //board that players will be putting characters on
char yes('y');                                                              //giving value to char for contingency
char currentBoard[10] = {'0','1','2','3','4','5','6','7','8','9'};          //setting values of array where markers will be placed
//brief intro to Tic Tac Toe
cout << "Welcome to Tic Tac Toe." << endl;
cout << "In this game each player is assigned a symbol of X (player 1) and O (player 2)." << endl;
cout << "The first player to get three of their symbols in a row wins." << endl;
cout << "Press the enter key when you are ready to continue." << endl;

system("pause");
//cin.get(); //system("pause") doesnt work on linux so I have a solution that works on both windows and linux

    do
    {
        //system("clear");            //linux clear display
        system("cls");              //windows clear display
        for (int i = 0; i < 10; i++)currentBoard[i]=NEW_BOARD[i];               //reseting board from previous game

        playGame(currentBoard);                      //function that cointains bulk of the game

        cout << "\nDo you wish to play again?[y for yes]: ";
        if(!(cin >> yes)) clearInput(yes);                      //prevents answer from causing potential errors in program
    }while(yes == 'y' || yes == 'Y');         //if false then game will not be played again

}

//FUNCTION DEFINITIONS

//displays board for players by calling drawBoard, checks for bad input or spaces that already have markers there by comparing
//NEW_BOARD and currentBoard. After valid input is recieved it calls winCheck to check win conditions and afterwards if each
//space has been used, if so then a draw is declared and the loop exits
int playGame(char currentBoard[])
{
                                                      //resetting values
    string winPos("null");
    int player(1);                                    //represents whose turn it is between players 1 and 2
    int choice(0);                                    //takes input of where player wishes to play their marker
    int gameOver(0);                                      //status of the game, 0 means in progress, 1-8 are game winning positions, -1 is a draw.

    do
    {
        drawBoard(currentBoard);                        //display board for user

        cout << "Player " << player <<" (" << playerMark[player] << "), enter your move as a number:  ";

        while (!(cin >> choice) || choice<1 || choice>9 || NEW_BOARD[choice]!=currentBoard[choice])       //checking for incorrect input
        {
            clearInput(choice);                             //clears flag, ignores input
        }

        currentBoard[choice] = playerMark[player];              //sets player chosen position in array to their mark

        gameOver = checkWin(currentBoard, winPos);//if gameover==0 then the game continues, if 1 then someone won, if -1 then its a draw


        if      (gameOver==0 && player==2) player = 1;       //set to player 1 if game is still going and 2 was previous
        else if (gameOver==0 && player==1) player = 2;       //set to player 2 if game is still going and 1 was previous

    }while (gameOver == 0);                 //loop exits upon winner or draw

    if(gameOver == 1)                       //game won
    {
        drawBoard(currentBoard);            //updates board to show final marker placement
        cout << "\nPlayer " << player << " has won on the " << winPos;
    }
    else if(gameOver == -1)                 //game draw
    {
        cout << "\nThe game has ended in a draw, please try again.";
    }

}

//draws game board on screen when called
void drawBoard(char currentBoard[])
{
        //system("clear");        //display clear on linux
        system("cls");          //display clear on windows
        cout << "\n\t\t Tic Tac Toe\n";

        cout << "\tPlayer 1 (X)  -  Player 2 (O)" << endl << endl;

        cout << "\t        " << currentBoard[1] << "  |  " << currentBoard[2] << "  |  " << currentBoard[3] << endl;
        cout <<"\t     ------------------- " << endl;
        cout << "\t        " << currentBoard[4] << "  |  " << currentBoard[5] << "  |  " << currentBoard[6] << endl;
        cout <<"\t     ------------------- " << endl;
        cout << "\t        " << currentBoard[7] << "  |  " << currentBoard[8] << "  |  " << currentBoard[9] << endl;
}

//checks for victory at certain places, if there is a winner winPos is set to the location of victory and 1 is returned
//which exits primary playGame loop.
//if no winner is found it checks that all squares have been played, if there are none left a draw is called otherwise game continues
int checkWin(char currentBoard[], string &winPos)
{
        if(currentBoard[1] == currentBoard[2]&&currentBoard[2]==currentBoard[3])            //top row victory
            {
            winPos = "top row.";
            return 1;
            }
        else if(currentBoard[4] == currentBoard[5]&&currentBoard[5]==currentBoard[6])       //middle row victory
            {
            winPos = "middle row.";
            return 1;
            }
        else if(currentBoard[7] == currentBoard[8]&&currentBoard[8]==currentBoard[9])       //bottom row victory
            {
            winPos = "bottom row.";
            return 1;
            }
        else if(currentBoard[1] == currentBoard[4]&&currentBoard[4]==currentBoard[7])       //left column victory
            {
            winPos = "left column.";
            return 1;
            }
        else if(currentBoard[2] == currentBoard[5]&&currentBoard[5]==currentBoard[8])       //middle column victory
            {
            winPos = "middle column.";
            return 1;
            }
        else if(currentBoard[3] == currentBoard[6]&&currentBoard[6]==currentBoard[9])       //right column victory
            {
            winPos = "right column.";
            return 1;
            }
        else if(currentBoard[1] == currentBoard[5]&&currentBoard[5]==currentBoard[9])       //left diagonal victory
            {
            winPos = "left diagonal.";
            return 1;
            }
        else if(currentBoard[3] == currentBoard[5]&&currentBoard[5]==currentBoard[7])       //right diagonal victory
            {
            winPos = "right diagonal.";
            return 1;
            }
        else
            {
            for (int x=1;x<10;x++) if (currentBoard[x]==NEW_BOARD[x]) return 0;                      //unplayed square, game continues
            return -1;
            }                                                                      //all boxes played, return draw
}

//called when bad entry is input
void clearInput(char input)
{
    cin.clear();                    //clears flag
    cin.ignore(100,'\n');           //ignore input on line
    cout << "Please enter an expected or available value.";

}

//END OF FUNCTION DEFINITIONS
