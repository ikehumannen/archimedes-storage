/*
2
*/
#include <iostream>
#include <cstdlib> //included to seed the RNG
#include <ctime>

using namespace std;

int main()
{
int h_Pick;     //number of toothpicks the human inputs to take
int c_Pick;     // number of toothpicks the computer takes
char restart = 'y';     //variable to restart main loop
int toothP = 23;        //default number of toothpicks
int firstChoice;    //user inputs value and this determines who goes first; lines 45-57
bool playerTurn;    //determines which of the two major loops are in play
bool readPicks;     //true when user wants to have the remaining toothpicks revealed to them
char askToRead;     //takes user input on whether they want remaining toothpicks remaining;lines 63-77
bool gameEnd;       // determines if game is still going, when false neither computer or player will be able to input chocies
bool repeatLoop;    // general use boolean for various small loops

cout << "This program is designed to play between a human and the computer."
     << "\n23 is a game where two players take between 1 and 3 toothpicks out of a container each turn."
     << "\nWhoever removes the final toothpick loses.";

do{ //determines if player wants to restart, ends at line 154

gameEnd = false;        //variable set false for lines 81-148 to function
int firstChoice = 0;    //set to 0 so loop at lines
int toothP = 23;
playerTurn = true;  //default value and resets previous
readPicks = false;  //default value and resets previous
srand(time(0));         //RNG being seeded for line 122
char askToRead = 'n';   //the letter n doesnt matter, just random default value

repeatLoop = true; //allows for loop asking who goes first to repeat

while (repeatLoop == true) // exit loop if 1 or 2 entered
    {
    cout << "\nWho goes first? 1 for human, 2 for computer: ";
    cin >> firstChoice;
        if (firstChoice == 1)
        {
        playerTurn = true;      //human goes first
        repeatLoop = false;     //exit loop
        }
        else if (firstChoice == 2)
        {
        playerTurn = false;     //computer goes first
        repeatLoop = false;     //exit loop
        }
        else cout << "Input correct value.";    //received if neither 1 or 2 enetered.
    }
repeatLoop = true;
while (repeatLoop == true) //true when 'n', 'N', 'y' or, 'Y' entered
    {
    cout << "\nDo you wish to have the remaining toothpicks told to you every turn? "
         << "Y for yes, N for no: ";
     cin >> askToRead;
    if (askToRead == 'n'|| askToRead == 'N')
        {
        readPicks = false;      //user does not wish to have toothpick count revealed
        repeatLoop = false;     //exit loop
        }
    else if (askToRead == 'y'||askToRead == 'Y')
        {
        readPicks = true;       //user wishes to have toothpicks revealed
        repeatLoop = false;     //exit loop
        }
    else
        {
        cout << "Input valid character.";       //prevents bad data
        }
    }

    do //while gameEnd == false; ends at 149
    {

        while(playerTurn == true)       //players turn; ends at line 116
        {
            h_Pick = 0;     //removing previous data

            if (readPicks == true)      //only happens if user chose to have toothpicks remaining read to them
            {
            cout << "There are " << toothP << " toothpick(s) remaining";
            }
            cout << "\nEnter a number between 1 and 3 to remove that many toothpick(s)";
             cin >> h_Pick;
                if ((h_Pick >= 1 && h_Pick <= 3) && (toothP >= 4))      //occurs only when number chosen is greater than or equal to 1 and
                    {                                                   //less than or equal to 3 while there are 4 or more toothpicks remaining
                        {
                        toothP = toothP - h_Pick;           //setting the remaining toothpicks based on user input
                        playerTurn = false;                 //begins computers turn
                        }
                    }
                else if ((h_Pick >= 1 && h_Pick <= 3) && (toothP >= 1 && toothP <= 3))//only happens with correct number entry and if there are
                        {                                                             //between 1 and 3 toothpicks remaining
                        if ((h_Pick == toothP) && (toothP != 1))        //prevents user from ending their game until final toothpick remains
                            {
                            cout << "This move would lose you the game.";
                            }
                        else if ((h_Pick == 1) && (toothP == 1))    //happens when there is only one toothpick remaining
                            {
                            cout << "\nYou have lost the game.\n";
                            gameEnd = true; //game is over
                            break;          //exits loop and goes to line 152
                            }
                        }
                else                //prevents bad data from being entered in h_Pick
                    {
                    cout << "\nThis is an invalid input.\n";
                    }
        }

        while (playerTurn == false) //computers turn; ends at line 147
        {
        if (toothP == 23)       //happens if computer is chosen to make the first move
            {
            c_Pick = (rand() % 2) + 1;      //picking a random number between 1 and 3 for first move
            toothP = toothP - c_Pick;
            cout << "\nThe computer took " << c_Pick << " toothpick(s).\n";
            playerTurn = true; //players turn begins
            }

        else if (toothP > 4) //until there are 3 or less toothpicks the computer bases its moves off human input
            {
            c_Pick = 4 - h_Pick;
            toothP = toothP - c_Pick;
            cout << "\nThe computer took " << c_Pick << " toothpick(s).\n";
            playerTurn = true;  //players turn begins
            }
        else if (toothP <= 3 && toothP != 1) // if there are three or less remaining but more than 1
            {                                //computer will take all toothpicks but 1
            c_Pick = toothP - 1;
            toothP = toothP - c_Pick;
            cout << "\nThe computer took " << c_Pick << " toothpicks(s).\n";
            playerTurn = true;      //players turn begins
            }
        else if (toothP == 1)       //player victory
            {
            cout << "\nYou win!\n";
            gameEnd = true;     //game is over
            }
        }

    }while (gameEnd == false); //game is over when true


    cout << "\nDo you wish to play again? (Y for yes.)\n";
    cin >> restart;

}while (restart == 'y'|| restart == 'Y'); // if conditions are true then entire program will loop

return 0;       //exits code
}
