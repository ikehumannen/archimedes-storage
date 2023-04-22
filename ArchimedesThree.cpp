/*
3
*/

#include <iostream>
#include <string>               //needed for food string
#include <cmath>                //needed for ceil function
#include <cstdlib>              //needed for pow function

using namespace std;
                            //calories per serving of each food
int const CHOCBAR = 230;    //Chocolate Bar
int const GPEAS = 100;      //Green Peas
int const OMELET = 94;      //Egg Omelet
int const DBURGER = 1000;   //Double Cheeseburger

int bmrCal;                 //results of getBMRCal are set equal to this
int physicalCal;            //results of getPhysicalCal are set equal to this
int weight, intensity, minutes;         //needed to get the daily caloric requirement
int foodSelect;                         //takes the input of the user selected food
int totalCal;                       //equals result of getPhysicalCal + getBMRCal
int foodCal;                        //equals value of chosen food's calories per serving amount
int dailyServ;                      //results from getCalReq; line 102
string food;                        //gets input from getFood function to read at end of second calculation
char restart, additionalRes;        //used for repeat of main loop and to start second calculation respectively

int getBMR (int weight);                                    //lines 117-120
int getPhysicalCal (int weight, int intensity, int minutes); //lines 122-128
string getFood (int foodSelect);                             //lines 127-145
int setCal (int foodSelect);                                 //lines 147-165
double getCalReq (int totalCal, int foodCal);               //lines 167-172
int userError(int error);                                   //lines 174-179

int main()
{
cout << "This program will calculate the daily calories needed to maintain "            //brief intro to what program does
     << "your weight based on how many pounds you are and the minutes spent "
     << "an activity of determinable intensity. You can also optionally choose "
     << "to see how many servings of a select food it would take to reach this calorie goal.";
char restart = 'y';             // allows main loop to begin
do                              //end at line 111
{

    int weight = 0;                 //resetting values
    int intensity = 0;
    int minutes = 0;
    int foodSelect = 0;

    do
    {
        cout << "\n\nHow many pounds do you weigh? (max 1000): \n";
        if (!(cin >> weight)) userError(weight);                //prevent non-integer answer, see lines 174-179
    }while ((weight <= 4) || (weight >= 1001));                //checks that weight is five or more and 1000 or less

    cout << "--------------------------------------------------" << endl;
    cout << "           Activity            |       Intensity  " << endl;
    cout << "Running at 10 mph              |          17      " << endl;
    cout << "Running at 6 mph               |          10      " << endl;
    cout << "Basketball                     |          8       " << endl;
    cout << "Running at 1 mph               |          1       " << endl;
    cout << "--------------------------------------------------" << endl;

    do{
        cout << "\nFrom 1-17 what would best describe the intensity of the most physical activity you do in a day?: ";      //user activity intensity min 1, max 17
        if (!(cin >> intensity)) userError(intensity);                                      //prevent non-integer answer, see lines 174-179
    }while ((intensity <= 0) || (intensity >= 18));                                         //value of 1-17 entered

    do
    {
        cout << "\nHow many minutes do you partake in the activity?(max 180): ";             //user activity time min 1, max 180
        if (!(cin >> minutes)) userError(minutes);                                         //prevent non-integer answer, see lines 174-179
    }while (minutes <= 0 || minutes >= 181);                                              //proper input given

    bmrCal = getBMR(weight);                                        //calls function to calculate daily basal metabolic rate calory usage, see lines 117-120
    physicalCal = getPhysicalCal(weight,intensity, minutes);        //calls function to get physical activity calory usage, see lines 122-128
    totalCal = bmrCal + physicalCal;                                //calls function to get total calories needed for the day


    cout << "\n\nYou need " << totalCal << " calories a day to maintain your current weight.";
    cout << "\nWould you like to know how many servings of a selected food this is? (y for yes):";
    cin >> additionalRes;                                                       //used for second loop to run calculation if chosen
    while (additionalRes == 'y' || additionalRes == 'Y')                        //anything other than Y or y will bring you to line 111 and ask for complete program repeat
    {
        cout << "------------------------------------------------------" << endl;
        cout << "           Food            | Serving |      Calories  " << endl;
        cout << "1. Chocolate Bar           |  each   |        230     " << endl;
        cout << "2. Green Peas              |  cup    |        100     " << endl;
        cout << "3. Egg Omelet              | per egg |        94      " << endl;
        cout << "4. Double Cheeseburger     |  each   |       1000     " << endl;
        cout << "------------------------------------------------------" << endl;

        do                                                                                      //user selects food from list 1-4
        {
            cout << "\nWhich of these food items do you wish to select?";
            if (!(cin >> foodSelect)) userError(foodSelect);                                    //prevent non-integer answer, see lines 174-179
        }while ((foodSelect <= 0) || (foodSelect >= 5));                                        //value of 1-4 entered

        string food = getFood(foodSelect);                                                      //call function to set string so that it will be read in the end statement, see lines 127-145
        foodCal = setCal(foodSelect);                                                           //sets caloric value of food serving chosen, see lines 147-165
        dailyServ = getCalReq(totalCal, foodCal);                                                //gets number of the food selected needed to maintain weight, see lines 167-172

        cout << "\n\nEating only " << food << " You would need to eat roughly "
             << dailyServ << " serving(s) to maintain your current weight";
        additionalRes = 'n';                                                                    //changes value of additionalRes, ends loop because it needs to be y or Y to restart
    }
    cout << "\n\nWould you like to repeat the program?(y for yes): ";
    cin >> restart;
}while (restart == 'y'|| restart == 'Y');                                               //Anything other than Y or y ends main loop

}

int getBMR(int weight)                                          //calculates calories needed to maintain BMR of current weight
{
    return(70 * pow(weight / 2.2,0.756));
}

int getPhysicalCal(int weight, int intensity, int minutes)      //calculates calories needed to maintain weight after daily activity
{
    return(0.0386 * intensity * weight * minutes);
}

string getFood(int foodSelect)                                  //sets value of food string to be read out at end of second calculation
{
    if (foodSelect == 1)                                            //user selected chocolate bar
        {
        return "(a) chocolate bar(s)";
        }
    else if (foodSelect == 2)                                      //user selected green peas
        {
        return "green peas";
        }
    else if (foodSelect == 3)                                     //user selected egg omelet
        {
        return "(a) egg omelet(s)";
        }
    else if (foodSelect == 4)                                    //user selected double cheeseburger
        {
        return "(a) double cheeseburger(s)";
        }
}

int setCal(int foodSelect)                                    //sets value of foodCal needed for getCalReq
{
    if (foodSelect == 1)                                         //user selected chocolate bar
        {
        return CHOCBAR;                                          //returns 230 calories
        }
    else if (foodSelect == 2)                                   //user selected green peas
        {
        return GPEAS;                                           //returns 100 calories
        }
    else if (foodSelect == 3)                                   //user selected omelet
        {
        return OMELET;                                          //returns 94 calories
        }
    else if (foodSelect == 4)                                   //user selected double cheeseburger
        {
        return DBURGER;                                         //returns 1000 calories
        }
}

double getCalReq(int totalCal, int foodCal)                 //is double so that the value can be rounded, but is set equal to an integer above so the following number gets cut off for consistency
{
    totalCal = ceil(totalCal / foodCal);
    totalCal -= totalCal * .1;              //accounting for 10% spent on digestion
    return (totalCal);                        //ceil so that if there is a less than 1 serving it will be rounded up
}

int userError(int error)
{
    cin.clear();                            //clear error flag
    cin.ignore(100, '\n');                  //ignore line input
    cout << "Please enter an integer.";
}
