/*
1
*/
#include <iostream>

using namespace std;

int main()
{

 double length;                // input for material length, double for precise measurements.

 int matSelect;             // input for the material coeffecient and matName string changes

 double firstTemp, endTemp;    // input for first and final celcius temperatures, double for precise temp input

 double displacement, displacementPercent, coefficient;   //values use in end formulas
 double endLength;                          // Value of final length

 string material;                           //express the chosen material as a word in final calculation
 string direction;                          //express contraction or expansion based on final calculation

 const double wood = 5e-6;                  //coefficients of the materials the user can choose
 const double glass = 8.5e-6;
 const double concrete = 1.02e-5;
 const double steel = 1.2e-5;
 const double copper = 1.7e-5;
 const double aluminum = 2.31e-5;

 char restartCalc;                          //Used to restart the entire program when prompted
 bool repeatLoop;                           //Used to keep out bad answers the user may input

 cout << "This program will calculate the expansion or conraction (in meters)"
      << " of a selected material over an input temperature range (Celcius).\n";

    do          //do-while loop so user can do repeat calculations
    {
        repeatLoop = true;  //Loop until acceptable input is entered
        do                  //Loop for user to select material
        {
            cout << "Select one of the following materials.\n"
                 << "1. Wood \n"
                 << "2. Glass \n"
                 << "3. Concrete \n"
                 << "4. Steel \n"
                 << "5. Copper \n"
                 << "6. Aluminum \n"
                 << "Enter 1-6 to choose material: ";
                 cin >> matSelect;
                 if (matSelect <= 0 || matSelect >= 7) cout << "\nInput valid number.\n"; //Prevents user from selecting outside of pool
                 else repeatLoop = false;           //acceptable aswer given

        }while (repeatLoop == true); //exit loop after valid material chosen

        cout << "\nEnter initial temperature in Celsius: ";
        cin >> firstTemp;

        repeatLoop = true;      //Loop repeats until user puts value not equal to firstTemp
        do                      //Loop prevents user from entering same value in firstTemp and endTemp
        {
            cout << "\nEnter final temperature in Celcius: ";
            cin >> endTemp;
            if (endTemp == firstTemp) cout << "\nTemperature values cannot be the same.";
            else repeatLoop = false;        //user did not input value of firstTemp

        } while (repeatLoop == true);       //only exits if firstTemp and endTemp are different values

        repeatLoop = true; //repeats until user inputs valid number
        do                 // prevents number less than or equal to zero being input as length
        {
            cout << "\nEnter material length in meters: ";
            cin >> length;

            if (length <=0) cout << "\nEnter number greater than zero.\n";
            else repeatLoop = false; //Number greater than zero entered

        } while (repeatLoop == true); //exit loop after valid length input

        if (matSelect == 1)         //setting wood coefficient values for formula
        {
            material = "Wood";
            coefficient = wood;
        }
        else if (matSelect == 2)    //setting glass coefficient values for formula
        {
            material = "Glass";
            coefficient = glass;
        }
        else if (matSelect == 3)    //setting concrete coefficient values for formula
        {
            material = "Concrete";
            coefficient = concrete;
        }
        else if (matSelect == 4)    //setting steel coefficient values for formula
        {
            material = "Steel";
            coefficient = steel;
        }
        else if (matSelect == 5)    //setting copper coefficient values for formula
        {
            material = "Copper";
            coefficient = copper;
        }
        else if (matSelect == 6)    //setting aluminum coefficient values for formula
        {
            material = "Aluminum";
            coefficient = aluminum;
        }

        displacement = coefficient * length * (endTemp - firstTemp);    //calculate expansion or contraction
        endLength = length + displacement;                              //calculate final length of material
        displacementPercent = 100*(displacement / endLength);           //calculate percent expansion or contraction

        if(displacement < 0)        //prevent double negative for contraction output
        {
            direction = "contraction";
            displacement = displacement * -1;
            displacementPercent = displacementPercent * -1;
        }
        else direction = "expansion"; //output is positive making it expansion instead of contraction

        cout.setf(ios::fixed);          //Makes output more readable for user
        cout.setf(ios::showpoint);
        cout.precision(5);

        cout << "\n\nThe total " << direction << " for " << material << " is "
             << displacement << " and is " << displacementPercent
             << " percent of the final material lenght of ";
        cout.precision(2);                                      //better way to read the final length
        cout << endLength << " meters.\n";

        cout << "\nDo you want to run another calculation (y for yes): ";
        cin >> restartCalc;


    } while(restartCalc == 'y' || restartCalc == 'Y'); //ends program when made false

    return 0; //ends program after choses not to run program again

}
