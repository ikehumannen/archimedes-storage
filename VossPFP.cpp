/*
Jacob Voss
jacob.voss3@student.eastcentral.edu

Final Project

The purpose of this program is to view population data from 2010, 2018, and 2020 in a variety of ways.
The user can view the total population for the three years, see the growth from 2010 to 2018 or from 2018 to 2020.
he user can view all state data sorted either by rank, name, growth for 2020, or density in square miles.
The user can also compare two states by entering their abbreviations.
Data from: https://worldpopulationreview.com/states/
*/

#include <iostream>     // General cin/cout input/output library
#include <fstream>      // Library needed for file I/O
#include <string>       // String functions library
#include <iomanip>      // Library needed for formatting instructions
#include <vector>       // Library needed for vectors

using namespace std;   					//Standard C++ vocabulary

const char DATA_FILE[] = "PopulationByState.txt";   // Name of Data File
const int MAX_DISPLAY (25);     			        // Maximum number of records to display before a pause
const string R_DELIM (",");   			            // deliminater in record            //FINAL USES COMMA
const int HI_MENU(8), LOW_MENU(0);                  // Range of allowable menu responses

// Standard Error Return Codes
const int A_SIZE_ERR  (-1);   // Array Size Error Code
const int F_DATA_ERR  (-2);   // File Data Error Code
const int F_OPEN_ERR  (-3);   // File Open Error Code
const int EOF_FOUND   (-4);   // EOF Encountered Code
const int NOT_FOUND   (-5);   // Value Not Found Code

//  DECLARE and DEFINE REGULAR FUNCTIONS COPIED FROM OTHER PROGRAMS
//  FUNCTION to output standard error messages and terminate program
void terminateErrorMsg (int errorR)
    {
    switch(errorR)  //Switch on error value send by another function/code
    {
        case A_SIZE_ERR:
            cout<<"\nArray to small to read data from file. PROGRAM TERMINATED\n";
            break;
        case F_DATA_ERR:
            cout<<"\nInvalid data encountered during file read. PROGRAM TERMINATED\n";
            break;
        case F_OPEN_ERR:
            cout<<"\nFile opening error. PROGRAM TERMINATED.\n";
            break;
        case EOF_FOUND:
            cout<<"\nUnexpected End of File encountered. PROGRAM TERMINATED.\n";
            break;
        default:
            cout<<"\nOther Error. PROGRAM TERMINATED.\n";
    }
    exit(errorR);
}//end of terminateErrorMsg

//FUNCTION to convert the next substring from the
//string record into a integer number
int getInt(string record, int &pos1, string delim)
{
    int pos2;                                       // index of end of substring
    string temp;                                    // where the substring is placed

    pos2=record.find_first_of(delim, pos1);         // Locate delimiter
    if(pos2<0) pos2=record.find('\0');              // No delimiter, so at end of string
    temp=record.substr(pos1, pos2-pos1);            // Get variable into substring temp
    erase(temp, '"');                               // remove quotations
    pos1=pos2+1;                                    // Locate next comma delimiter, CBR

    return (atoi(temp.c_str()));                    // Convert string to integer and return
}	// end of getInt
//FUNCTION to convert the next substring from the
//string record into a string variable
string getString(string record, int &pos1, string delim)
{
    int pos2;                                               // end of the substring
    string temp;                                            // substring

    pos2=record.find_first_of(delim, pos1);     // Locate delimiter
    if(pos2<0) pos2=record.find('\0');          // No delimiter, so must be at end of string
    temp=record.substr(pos1, pos2-pos1);        // Find string variable
    erase(temp, '"');                           // remove quotations
    pos1=pos2+1;                                // Locate next delimiter

    return (temp);                              // return the string
}	// end of getStri

//FUNCTION to convert a digit to a string
string digitToStr(int x)
{
    const string num = "0123456789";
    int d;                                  // isolated least significant digit from x
    string s = "\0";                        // convert integer one digit at a time into this string

    if(x==0) s.insert(0,num.substr(0,1));   // Test for zero, insert

    while (x)                               // Loops from least significant digit until x = 0
    {
        d = x%10;                           // Isolate current least significant digit (extreme right)
        s.insert(0, num.substr(d,1));       // Insert converted digit at beginning of string
        x = x/10;                           // Get next digit to the left (throw away converted digit on right)
    }
    return (s);                             // Return integer as string s
}   // end of digitToStr

//FUNCTION inserts a comma into the integer n
//and returns n as a string
string insertComma(int n)
{
    string s="", sRemains;
    int x;

    if(n<0)                             // Check for negative number
    {
        s = "-" + insertComma(abs(n));  // Recursive call
        return (s);
    }
    else if (n>=1000)
    {
        x = n%1000;                     // Modulo division to isolate lower three digits
        sRemains = digitToStr(x);       // Convert lower three digits to string

        if (x==0)        s = insertComma(n/1000) + ",000";              // to_string doesn't pad zeros,
        else if (x<10)   s = insertComma(n/1000) + ",00" + sRemains;    //  so manually insert into string
        else if (x<100)  s = insertComma(n/1000) + ",0" + sRemains;     // using recursive call to insertComma
        else if (x<1000) s = insertComma(n/1000) + "," + sRemains;      // note use of integer division
        return (s);
    }
    else if (n<1000)                    // Small number (<3 digits), so no comma required
    {
        s = digitToStr(n);
        return (s);
    }
}   // end of insertComma

//FUNCTION to take the string from record and make it into a float
float getFloat(string record, int &pos1, string delim) // Convert the next substring from the
{                                                       // string record into a float number
    int pos2;                                           // end of the substring
    string temp;                                        // is the substring

    pos2=record.find_first_of(delim, pos1);     // Locate delimiter
    if(pos2<0) pos2=record.find('\0');          // No delimiter, so at end of string
    temp=record.substr(pos1, pos2-pos1);        // Get variable in substring temp
    erase(temp, '"');                           //remove quotations
    pos1=pos2+1;                                // Locate next delimiter

    return (atof(temp.c_str()));                // returns string as float
}  	// end of getFloat

//  FUNCTION to obtain an integer response from user at console
//  between lowerBound and upperBound, inclusive
int getUserResponse(int lowerBound, int upperBound)
{
    int userInput;       // Returned value from user
    bool loop=true;          // Control when the loop ends

    while(loop)          // Get an integer from user with error checking
    {
        if (!(cin>>userInput))
        {
            cin.clear();
            cin.ignore(100,'\n');
            cout<<"\nERROR. Enter an integer: ";    //User did not enter an integer
        }
        else if (lowerBound==upperBound) loop = false; // Entered value doesn't matter
        else if (userInput<lowerBound) cout<<"\nERROR. Enter a larger integer: ";
        else if (userInput>upperBound) cout<<"\nERROR. Enter a smaller integer: ";
        else loop=false;                // End loop as user entered a valid number
    } //end loop

    return(userInput);
}   //end of getUserResponse

//FUNCTION to clear user entry
//specifically in one scenario in the compareStates where the user could enter a special character
void clearInput(string input)
{
    cin.clear();        //clears flag when special character is entered
}

//FUNCTION to inform the user on what the program does and where the data comes from
void getContext()
{
    cout << "The purpose of this program is to allow you to view population data for the United States in 2010, 2018, and 2020.\n"
         << "Data sourced from https://worldpopulationreview.com/states/\n";
    system("pause");
}

//FUNCTION to show user what their menu options are
int getStateInfo(int lowMenu, int highMenu)
{
    cout << endl <<"                Total Population for 2010 (1)\n"
                 <<"                Total Population for 2018 (2)\n"
                 <<"                Total Population for 2020 (3)\n"
                 <<"                 Growth from 2010 to 2018 (4)\n"
                 <<"                 Growth from 2018 to 2020 (5)\n"
                 <<"          Average population density 2020 (6)\n"
                 <<"                      View all state data (7)\n"
                 <<"                       Compare two states (8)\n"
                 <<"                             Exit program (0)\n\n"
                 <<"        ENTER 1, 2, 3, 4, 5, 6, 7, 8, or 0: ";
    return(getUserResponse(lowMenu, highMenu));
}

// class Population will hold state data obtained from the file "PopulationByState.txt"
class Population
{
public:
    Population();                       //default constructor
    void putOneState(ostream &out);     //output one state
    Population copyRanking();           //copy one object into another

    int rank;                           //state ranked by 2020 population, high to low
    string name;                        //name of the state
    int pop2020;                        //population of a state in 2020
    float growth;                       //population growth from 2018 to 2020
    int pop2018;                        //population of a state in 2018
    int pop2010;                        //population of a state in 2010
    float growthSince2010;              //population growth percent from 2010 to 2020
    float percent;                      //percent of total population state makes up in 2020
    float density;                      //average population density per square mile
private:
};

//DEFAULT CONSTRUCTOR to initialize values
Population::Population()
{
    rank = 0;
    name = "none";
    pop2020 = 0;
    growth = 0;
    pop2018 = 0;
    pop2010 = 0;
    growthSince2010 = 0;
    percent = 0;
    density = 0;
}

//MEMBER FUNCTION to output the full data for an individual state
void Population::putOneState(ostream &out)
{
    out << setw(6) << left << rank
        << setw(21) << name << right
        << setw(10) << insertComma(pop2020)
        << setw(9) << growth * 100 << "%"          //multiply by 100 to show number as a percent and follow it with the percent sign in output
        << setw(25) << insertComma(pop2018)
        << setw(15) << insertComma(pop2010)
        << setw(9) << growthSince2010 * 100 << "%"//multiply by 100 to show number as a percent and follow it with the percent sign in output
        << setw(17) << percent * 100 << "%"        //multiply by 100 to show number as a percent and follow it with the percent sign in output
        << setw(20) << density;
}

//MEMBER FUNCTION to copy one object to another
Population Population::copyRanking()
{
    Population p;

    p.rank              = rank;
    p.name              = name;
    p.pop2020           = pop2020;
    p.growth            = growth;
    p.pop2018           = pop2018;
    p.pop2010           = pop2010;
    p.growthSince2010   = growthSince2010;
    p.percent           = percent;
    p.density           = density;
    return(p);
}

// MEMBER FUNCTION to parse one record and returns class Population
Population parseOneRecord(string record)
{
    int position(0);    // set initial record position to beginning of class objectinto

    Population oneState;  // placeholder to return class

    oneState.rank             = getInt(record, position, R_DELIM);
    oneState.name             = getString(record, position, R_DELIM);
    oneState.pop2020          = getInt(record, position, R_DELIM);
    oneState.growth           = getFloat(record, position, R_DELIM);
    oneState.pop2018          = getInt(record, position, R_DELIM);
    oneState.pop2010          = getInt(record, position, R_DELIM);
    oneState.growthSince2010  = getFloat(record, position, R_DELIM);
    oneState.percent          = getFloat(record, position, R_DELIM);
    oneState.density          = getFloat(record, position, R_DELIM);

    return (oneState);
}

//END OF MEMBER FUNCTION DEFINITIONS

//FUNCTION to output titles to give context to the information
void putTitles(ostream &out)
{
    out << setw(2)  << "Rank"
        << setw(6)  << "Name"
        << setw(26) << "Pop. 2020"
        << setw(22) << "Growth 2018-2020"
        << setw(13) << "Pop. 2018"
        << setw(15) << "Pop. 2010"
        << setw(21) << "Growth 2010-2020"
        << setw(16) << "Percent of USA"
        << setw(11) << "Density"
        << endl;
}

// FUNCTION to output all records to out
void putAllRecords (ostream &out, vector<Population> states)
{
    int screenLoop(0);

    putTitles(out);             //output titles

    for (int i=0; i<states.size(); i++)        // loop to display the entire vector
    {
        if(screenLoop<MAX_DISPLAY)     // Don't overwhelm user screen
        {
            screenLoop++;              // Track number of records outputted so far
        }
        else                            // pause and output new titles
        {
            system("pause");
            putTitles(out);
            screenLoop=1;
        }

        states[i].putOneState(out);        // output one state record
        out<<endl;
    }
}	// end of putAllRecords

//FUNCTION: Swap two CBR Population values, but preserves other member variables
void swapVal(Population &v1, Population &v2)
{
    Population temp = v1;
    v1 = v2;
    v2 = temp;
}

//FUNCTION to alphanumerically recursively bubble sort a vector of class Population
//entire structure is moved to the right based on member variable rank
void bSortRank(vector<Population> &states, int vSize)
{
    if(vSize <= 0) return;//base case

    for (int i = 0; i < vSize - 1; i++)// Traverse vector/array from left to right
    {
        if (states[i].rank > states[i+1].rank) swapVal(states[i+1], states[i]);// Move larger value to the right (low to high)
    }

    bSortRank(states, vSize - 1);   //recursive call function to sort
}

//FUNCTION to recursively bubble sort a vector of class Population
//based on string name member variable, sorts alphanumerically
void bSortName(vector<Population> &states, int vSize)
{
    if(vSize <= 0) return;//base case

    for (int i = 0; i < vSize - 1; i++)// Traverse vector/array from left to right
    {
        if (states[i].name > states[i+1].name) swapVal(states[i+1], states[i]);// Move larger value to the right (low to high)
    }

    bSortName(states, vSize - 1);   //recursive call function to sort
}

//FUNCTION to recursively bubble sort by population growth in 2020 by highest to lowest
void bSortGrowth(vector<Population> &states, int vSize)
{
   if(vSize <= 0) return;//base case

    for (int i = 0; i < vSize - 1; i++)// Traverse vector/array from left to right
    {
        if (states[i].growth < states[i+1].growth) swapVal(states[i+1], states[i]);// Move larger value to the left (high to low)
    }

    bSortGrowth(states, vSize - 1);   //recursive call function to sort
}

//FUNCTION to recursively bubble sort by population density in 2020 by highest to lowest
void bSortDensity(vector<Population> &states, int vSize)
{
    if(vSize <= 0) return;//base case

    for (int i = 0; i < vSize - 1; i++)// Traverse vector/array from left to right
    {
        if (states[i].density < states[i+1].density) swapVal(states[i+1], states[i]);// Move larger value to the left (high to low)
    }

    bSortDensity(states, vSize - 1);   //recursive call function to sort
}

//FUNCTION to ask user if they want menuItem switch cases 7 and 8 outputted to a file named "StateData.txt"
//returns 1 for yes and 0 for no
int queryFileOutput()
{
    cout << endl << "Do you want results outputted to a file?\n"
                 << "Data will be output in file named " << "\"StateData.txt\"\n"
                 << "1 for yes, 0 for no: ";
    return(getUserResponse(0, 1));
}

//FUNCTION to prompt user how they want state data to be sorted when it is output to the screen
int querySort(vector<Population> &states, int vSize)
{
    int select;
    cout << endl << "       How do you wish to sort the data?\n"
                 << "                                Rank (1)\n"
                 << "      Alphanumerically by state name (2)\n"
                 << "                   Population Growth (3)\n"
                 << "        Population density (sq. mi.) (4)\n"
                 << "            Enter 1, 2, 3, or 4: ";
    select = getUserResponse(1, 4);//user entering 1 doesn't matter as each time the main switch statement runs the program is sorted by rank
    if(select == 2) bSortName(states, vSize); //user selects sort by name
    else if(select == 3) bSortGrowth(states, vSize);//user selects sort by growth
    else if(select == 4)bSortDensity(states, vSize);//user selects sort by density
}

//FUNCTION to recursively check if abbr is anywhere in stateAbbr
//if abbr is in stateAbbr it returns the number of loops as a value of sizeV which evaluates to an equivalent location in the states vector declared in main
//returns -5 if the sent abbreviation cannot be located in stateAbbr
int searchState(string abbr, vector<string> stateAbbr, int vSize)
{
    if(vSize < -1) return(NOT_FOUND);
    if(stateAbbr[vSize] == abbr) return(vSize - 1);//if the abbreviation is found return how many places down it is in the list
    return searchState(abbr, stateAbbr, vSize - 1 );//recursive call
}

// FUNCTION to allow user to compare two specific states against each other
// presents user with alphanumerically sorted list of state names with their respective abbreviations after their names
// user can enter the name in upper or lower case it doesn't matter
//returns position of specific abbreviation in stateAbbr, this number is used to determine where the state is located in the states vector
void compareStates(vector<Population> &states, vector<int> &twoStates, vector<string> stateAbbr, int  vSize)
{
    int pos(0);                             //holds  value of numerical position of an element in stateAbbr
    int loopCount(0);                       //track how many times an abbreviation has been entered successfully (2 max)
    string abbr;                            //holds user entered state abbreviation
    bSortName(states, vSize);                      //sort by names so that state abbreviations will be next to the correct state
    for(int i = 0; i < vSize; i++)
    {
        cout << endl << states[i].name << " " << "(" << stateAbbr[i+1] << ")";//output state names with their abbreviations in parenthesis next to them
    }
    cout << endl << "Enter the abbreviation of the first state you wish to check: ";

    do
    {
        cin >> abbr;
        for (int s = 0; s < size(abbr); s++)        //check each character entered in abbr individually
            if (isalpha(abbr[s]) == 0)              //is false when abbr has a non-letter in it
                clearInput(abbr);                   //clear abbr

        for (int i = 0; i < size(abbr); i++) abbr[i] = toupper(abbr[i]);   //make all letters upper case to prevent problems
        pos = searchState(abbr, stateAbbr, vSize);                         //get numeric location of the element in index

        if(pos >= 0 && pos <= 51)//good if the index position as a number is between 0 and 51 to match with results of recursive function
        {
            twoStates.push_back(pos);                                      //put the position of the state index into twoStates
            loopCount++;                                                   //loopCount increases upon successful state entry
            if(loopCount == 1) cout << "\nEnter second state to compare: ";//successful first entry, prompt user for second state
        }
        else
        {
            cout << "\nPlease enter a valid state abbreviation: ";         //invalid user entry
        }                                                 //make safeEntry true for next run through
    }while(!(loopCount == 2));//exit loop if there were two successfult entries

    if(twoStates[0] == twoStates[1]) cout << "\nComparing the same state? Wonder how they will stack up to each other.\n";  //just for fun

} // end of function compare states

//FUNCTION to get data from a specific row from the input file each time its called
//returns the data inside a single member object
void getDataSet(ifstream &inFile, vector<Population> &states)
{
    Population oneState;
    string record;                             //holds one record from the data file

    getline(inFile, record, '\n');             //read and ignore dataset titles

    do
    {
        getline(inFile, record, '\n');         //get a record from the file
        oneState = parseOneRecord(record);     //Parse the record into the object degree
        if(oneState.rank != 0) states.push_back(oneState.copyRanking()); //insert parsed record into vector, file contains empty line if statement accounts for this
    }while (!inFile.eof());
    return;
}// end of getDataSet

//FUNCTION to get the growth between two specific years
//returns the value of growth using the standard growth formula
double getGrowth(double startPop, double endPop)
{
    return((endPop-startPop)/startPop);//population at the ending year divided by population at the starting year over start year population
}                                      //results in the amount of population growth as a percent over those years

//FUNCTION to calculate the total size of the united states in square miles
//divdes a states population by its 2020 density and adds that number to the variable totalMiles
//totalMiles is returned as the size of the united states in 2020
double getMiles(vector<Population> states)
{
    double totalMiles(0);                   //holds the total miles for the united states.
    for(int i = 0; i < states.size(); i++)
    {
        totalMiles += states[i].pop2020 / states[i].density;    //divide a states population by its 2020 population density to get its size in miles
    }                                                           //add this number together each loop and you will have the total size for the united states
    return(totalMiles);
}

//FUNCTION to do numerous things
//gets the total population of the USA for 2010, 2018, and 2020
//calls getGrowth to calculate the population growth for a range of years and then returns the value
double getInfo(vector<Population> states, int menuItem)
{
int pop2010(0);   //holds population of entire country in 2010
int pop2018(0);   //holds population of entire country in 2018
int pop2020(0);   //holds population of entire country in 2020
    if(menuItem == 1||menuItem == 4)// if menuItem is 1 user only needs pop for 2010, if menuItem is 4 they need pop for growth calculation from 2010 to 2018
    {
        for(int i = 0; i < states.size(); i++)
        {
            pop2010 += states[i].pop2010;//add population of each state together to get total pop for the year
        }
        if(menuItem == 1) return(pop2010);//when menu item is 1 user only wants to see the population for 2010
    }
    if(menuItem == 2||menuItem == 4||menuItem == 5)// if menuItem is 2 user only needs pop for 2018, if menuItem is 4 or 5 they need pop for growth calculation
    {
        for(int i = 0; i < states.size(); i++)
        {
            pop2018 += states[i].pop2018;//add population of each state together to get total pop for the year
        }
        if(menuItem == 2) return(pop2018);//when menu item is 2 user only wants to see the population for 2018
    }
    if(menuItem == 3||menuItem == 5)// if menuItem is 3 user only needs pop for 2020, if menuItem is 5 they need pop for growth calculation
    {
        for(int i = 0; i < states.size(); i++)
        {
            pop2020 += states[i].pop2020;//add population of each state together to get total pop for the year
        }
        if(menuItem == 3) return(pop2020);//when menu item is 3 user only wants to see the population for 2020
    }
    if(menuItem == 4)                     //when menu item is 4 user wants growth from 2010 to 2018
    {
        return(getGrowth(pop2010, pop2018));//calculate pop growth from 2010 to 2018
    }
    if(menuItem == 5)                     //when menu item is 5 user wants growth from 2018 to 2020
    {
        return(getGrowth(pop2018, pop2020));//calulate pop growth from 2018 to 2020
    }
}// end of getInfo function

int main ()
{
vector<Population> states;  //vector that will have all the data from the file read into it
int totalPop(0);            //int to hold yearly total population from getInfo in switch cases 1, 2, and 3
int menuItem(0);            //holds value entered by user for use of primary switch statement
vector<int> twoStates;      //vector to hold the numerical position of a specific state (position when sorted alphanumerically)
double growth(0);           //double to hold population growth between two specific years that will be expressed as a percent
double miles(0);            //total miles of the country for a 2020 the only year given a specific density for each state
bool file;                  //true if user wants data outputted to file

vector<string> stateAbbr =       {"N0","AL","AK","AZ","AR","CA","CO",   //not a constant because thats how I felt like doing it at the time
                                  "CT","DE","DC","FL","GA","HI","ID",
                                  "IL","IN","IA","KS","KY","LA","ME",
                                  "MD","MA","MI","MN","MS","MO","MT",
                                  "NE","NV","NH","NJ","NM","NY","NC",
                                  "ND","OH","OK","OR","PA","PR","RI",
                                  "SC","SD","TN","TX","UT","VT","VA",
                                  "WA","WV","WI","WY"}; //for state abbreviations, sort states by alphabetical order and then order the abbreviations by that output order

ifstream inFile;
ofstream outFile;
inFile.open("PopulationByState.txt");             //open datafile
if(inFile.fail()) terminateErrorMsg(F_OPEN_ERR);  //check for bad file read

getDataSet(inFile, states);                       //populate vector states with data

inFile.close();                                   //close inFile as its no longer needed

cout.setf(ios::fixed);                            //format output
cout.setf(ios::showpoint);
cout.precision(2);

int vSize = states.size();                        //size of states vector, needed for recursive functions

getContext();                                     //inform user what the purpose of the program is

do
{
    cout << endl;                                 //extra line for formatting
    bSortRank(states, vSize);                     //presort as a precaution for repeat runs
    menuItem = getStateInfo(LOW_MENU, HI_MENU);   //show the user a menu with their choices as numbers and returns said choice
    system("cls");
    switch(menuItem)
    {
        case 0:                                   //exit program
            break;

        case 1:                                   //total population for 2010
            {
                totalPop = getInfo(states, menuItem); //calculate total population for 2010
                cout << endl << "Total population for 2010 was " << insertComma(totalPop); //call insert comma to make the number easier to read
                break;
            }
        case 2:                                   //totoal population for 2018
            {
                totalPop = getInfo(states, menuItem); //calculate total population for 2018
                cout << endl << "Total population for 2018 was " << insertComma(totalPop); //call insert comma to make the number easier to read
                break;
            }
        case 3:                                   //total population for 2020
            {
                totalPop = getInfo(states, menuItem); //calculate total population for 2020
                cout << endl << "Total population for 2020 was " << insertComma(totalPop); //call insert comma to make the number easier to read
                break;
            }
        case 4:                                    //pop growth from 2010 to 2018
            {
                growth = getInfo(states, menuItem);    //calculate pop growth from 2010 to 2018
                cout << endl << "The population growth from 2010 to 2018 was " << growth * 100 << "%";
                break;
            }
        case 5:                                    //pop growth from 2018 to 2020
            {
                growth = getInfo(states, menuItem);    //calculate pop growth from 2018 to 2020
                cout << endl << "The population growth from 2018 to 2020 was " << growth * 100 << "%";
                break;
            }
        case 6:                                     //pop density 2020
            {
                totalPop = getInfo(states, 3);          //sending 3 will result in population calculation for 2020
                miles = getMiles(states);               //calculate the total size of us in square miles
                cout << endl  << "The average population density per square mile for 2020 was " << totalPop/miles;
                break;
            }
        case 7:                                         //output all state data
            {
                file = queryFileOutput();                   //ask user if they want data outputted to "StateData.txt" 1 is yes 0 is no
                if(file == 1) outFile.open("StateData.txt");//open outFile if user wants data outputted to "StateData.txt"
                querySort(states, vSize);                   //ask user how they want the data sorted (rank, name, growth, or density)
                putAllRecords(cout, states);                //output all state data to console
                if(file == 1)
                {
                    putAllRecords(outFile, states);         //output all state data to output file
                }
                break;
            }
        case 8:                                         //user wants to compare two states
            {
                file = queryFileOutput();                   //ask user if they want data outputted to "StateData.txt" 1 is yes 0 is no
                if(file == 1) outFile.open("StateData.txt");//open outFile if user wants data outputted to "StateData.txt"
                compareStates(states, twoStates, stateAbbr, vSize); //prompt user for the two states they want to compare
                int s1 = twoStates[0];//assign numeric index position to s1 so that it can be used to find that same state by name in states vector
                int s2 = twoStates[1];//assign numeric index position to s2 so that it can be used to find that same state by name in states vector
                putTitles(cout);
                states[s1].putOneState(cout);
                cout << endl << " Compared to: \n";
                states[s2].putOneState(cout);
                if(file == 1)               //put comparison info into output file
                {
                    putTitles(outFile);
                    states[s1].putOneState(outFile);
                    outFile << endl << " Compared to: \n";
                    states[s2].putOneState(outFile);
                }
                break;
            }
        default:
            cout << "\nImpossible\n"; //really just shouldn't happen

    }//end of switch statement
    outFile.close();        //close outFile in the event the user outputted the data to the file
    twoStates.clear();      //clear vector containing state index positions for next run
    system("pause");
}while(menuItem);
return 0;
}//end of main
