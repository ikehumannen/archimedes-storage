/*
9
*/

#include <iostream>     // General cin/cout input/output library
#include <fstream>      // Library needed for file I/O
#include <string>       // String functions library
#include <cstring>      // C-String functions library
#include <cstdlib>      // Need for exit function
#include <iomanip>      // Library needed for formatting instructions
#include <vector>       // Library needed for vectors
#include <cmath>        // Math Library

using namespace std;   					//Standard C++ vocabulary

const char DATA_FILE[]="DegreeRanking.txt";     // Name of Data File
const int MAX_DISPLAY (25);     			// Maximum number of records to display before a pause
const string R_DELIM ("\t");   			// deliminater in record            //FINAL USES COMMA
const int HI_MENU(4), LOW_MENU(0);              // Range of allowable menu responses

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

// The next THREE functions are used to PARSE and format a record from the database
// based on delimiter provided in the argument

int getInt(string record, int &pos1, string delim)  // Convert the next substring from the
{                                                   // string record into a integer number
    int pos2;                                       // index of end of substring
    string temp;                                    // where the substring is placed

    pos2=record.find_first_of(delim, pos1);         // Locate delimiter
    if(pos2<0) pos2=record.find('\0');              // No delimiter, so at end of string
    temp=record.substr(pos1, pos2-pos1);            // Get variable into substring temp

//    cout<<temp<<endl; // TEST CODE

    pos1=pos2+1;                                // Locate next comma delimiter, CBR

    return (atoi(temp.c_str()));                // Convert string to integer and return
}	// end of getInt

string getString(string record, int &pos1, string delim)    // Convert the next substring from the
{                                                           // string record into a string variable
    int pos2;                                               // end of the substring
    string temp;                                            // substring

    pos2=record.find_first_of(delim, pos1);     // Locate delimiter
    if(pos2<0) pos2=record.find('\0');          // No delimiter, so must be at end of string
    temp=record.substr(pos1, pos2-pos1);        // Find string variable

    pos1=pos2+1;                                // Locate next delimiter

    return (temp);                              // return the string
}	// end of getString


double getFloat(string record, int &pos1, string delim) // Convert the next substring from the
{                                                       // string record into a float number
    int pos2;                                           // end of the substring
    string temp;                                        // is the substring

    pos2=record.find_first_of(delim, pos1);     // Locate delimiter
    if(pos2<0) pos2=record.find('\0');          // No delimiter, so at end of string
    temp=record.substr(pos1, pos2-pos1);        // Get variable in substring temp

    pos1=pos2+1;                                // Locate next delimiter

    return (atof(temp.c_str()));                // returns string as float
}  	// end of getFloat

// cFUNCTION to convert a string containing
// dollar sign and number with commas into an integer
int convDollarStrtoInt(string dollStr)           // string containing $ and ' into integer
{
    int pos;                                    // string position index

    dollStr.erase(0,1);                         // Remove $ from beginning of string

    do                                          // Loop if multiple commas
    {
        pos=dollStr.find_first_of(",", 0);      // Locate comma
        if(pos>=0) dollStr.erase(pos,1);        // Remove comma
    } while(pos>0);

    return (atoi(dollStr.c_str()));                // returns string as integer
}


// cFUNCTION to convert a string containing
// percent sign into a double
int convUnRateStrToDouble(string unRateStr)
{
    int pos;                                  // string position index

    pos=unRateStr.find_first_of("%", 0);      // Locate percent symbol
    if(pos>=0) unRateStr.erase(pos,1);        // Remove remove percent symbol

    return (stod(unRateStr.c_str()));         // returns string as double
}

// FUNCTION to convert a string to an integer
int convStrToInt(string sInt, string delim)
{
    int pos1(0), pos2(1);

    while(pos2>0)   // remove delimiter
    {
        pos2=sInt.find_first_of(delim, pos1);
        if(pos2>0) sInt.erase(pos2,1);
    }

    return(atoi(sInt.c_str()));
}   // end of convStrToInt

// FUNCTION to convert a three digit integer into a string
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

// FUNCTION inserts a comma into the integer n and returns a string
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

//  FUNCTION to obtain an integer response from user at console
//  between lowerBound and upperBound, inclusive
int getUserResponse (int lowerBound, int upperBound)
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
        else if (userInput<lowerBound) cout<<"\nFL41: ERROR. Enter a larger integer: ";
        else if (userInput>upperBound) cout<<"\nFL42: ERROR. Enter a smaller integer: ";
        else loop=false;                // End loop as user entered a valid number
    } //end loop

    return(userInput);
}   //end of getUserResponse

// END OF UTILITY FUNCTIONS

// DEFINE the class for holding name data obtained from the database
class Ranking
{
public:
// Declare Member Functions for the class Ranking
    void putOneRanking(ostream &out);                // Output a formatted object
    Ranking copyRanking();                           // Copy one object into another

// Member Variables of the class Names
    int rank;                   //rank of degree
    string name;                //name of degree
    string income;              //average income of degree holder
    string advDegree;           //percent with advanced degree
    string unRate;              //unemployment rate by percent

private:                            // Don't have private functions/variables
};

// DEFINE the MEMBER FUNCTIONS for the class Ranking

void Ranking::putOneRanking(ostream &out)    // Write one formatted object class to the stream out
{
    out <<setw(20)<<name                //employee name
        <<setw(20) <<rank                //job rank
        <<setw(10)<<income              //job income
        <<setw(10)<<unRate           //percent with advanced degree
        <<setw(10) <<advDegree;             //unemplyment rater
}

Ranking Ranking::copyRanking()    // Copy an object of the class Ranking into another object
{
    Ranking r;

    r.rank          = rank;
    r.name          = name;
    r.income        = income;
    r.unRate        = unRate;
    r.advDegree     = advDegree;

    return (r);
}
// END of class Names Member Function Definitions
// END of class Names DEFINITION

// DEFINE REGULAR FUNCTIONS SPECIFIC TO THIS PROGRAM

// FUNCTION to output titles to the stream out
void putTitles(ostream &out)
{
    out <<endl
        <<setw(20)
       //<<setw(40)<<"ADVANCE_"<<endl
        <<"ADVANCE_DEGREE_NAME         "
        <<setw(20) <<"RANK"
        <<setw(10)<<"INCOME"
        <<setw(10)<<"UNEMPLOY"
        <<setw(10)<<"DEGREE"
        <<endl<<endl;
}

Ranking parseOneRecord (string record)      // Parses record and returns class Ranking
{
    int position(0);    // set initial record position to beginning of array

    Ranking oneDegree;  // placeholder to return class

    oneDegree.rank      = getInt(record, position, R_DELIM);
    oneDegree.name      = getString(record, position, R_DELIM);
    oneDegree.income    = getString(record, position, R_DELIM);
    oneDegree.unRate    = getString(record, position, R_DELIM);
    oneDegree.advDegree = getString(record, position, R_DELIM);

    return (oneDegree);
}

// Function outputs all records to out
void putAllRecords (ostream &out, vector<Ranking> degree)
{
    int screenLoop(0);

    putTitles(out);

    for (int i=0; i<degree.size(); i++)        // loop to display the entire vector
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

        degree[i].putOneRanking(out);        // output one female name record
        out<<endl;
    }
}	// end of putAllRecords

// FUNCTION: Swap two CBR Ranking values, but preserves other member variables
void swapVal(Ranking &v1, Ranking &v2)
{
    Ranking temporary = v1;
    v1 = v2;
    v2 = temporary;
}

//  FUNCTION to bubble sort a vector of class Ranking
//  based on integer Rank
void bSortRank(vector<Ranking> &a)
{
    for (int i = a.size()-1; i>= 0; i--)                           // Traverse vector/array from right to left
        for (int j = 0; j < i; j++)                             // Traverse vector/array from left to right
            if (a[j].rank > a[j+1].rank) swapVal(a[j+1], a[j]);       // Move larger value to the right (low to high)
}

//  FUNCTION to bubble sort a vector of class Ranking
//  based on string income, converted to integer
void bSortIncomeD(vector<Ranking> &a)
{
    int incomeL, incomeR;                                       // variable to hold integer value of two adjacent income
    for (int i = a.size()-1; i>= 0; i--)                        // Traverse vector/array from right to left
        for (int j = 0; j < i; j++)                             // Traverse vector/array from left to right
        {
            incomeL = convDollarStrtoInt(a[j].income);
            incomeR = convDollarStrtoInt(a[j+1].income);
            if (incomeL < incomeR) swapVal(a[j+1], a[j]);       // Move larger value to the right (High to low)
        }
}

//  FUNCTION to bubble sort a vector of class Ranking
//  based on string unRate, converted to double
void bSortUnRate(vector<Ranking> &a)
{
    double unRateL, unRateR;                                    //variable to hold double value of two adjacent unemployment rates
    for (int i = a.size()-1; i>= 0; i--)                        // Traverse vector/array from right to left
        for (int j = 0; j < i; j++)                             // Traverse vector/array from left to right
        {
            unRateL = convUnRateStrToDouble(a[j].unRate);
            unRateR = convUnRateStrToDouble(a[j+1].unRate);
             if (unRateL > unRateR) swapVal(a[j+1], a[j]);       // Move larger value to the left (Low to High)

        }
}
// FUNCTION to read dataset into a vector of class Rankings
// return size of vector
void getDataSet(ifstream &inFile, vector<Ranking> &degrees)
{
    Ranking oneDegree;
    string record;                              //holds one record from datafile

    getline(inFile, record, '\n');            //read and ignore dataset titles

    do
    {
        getline(inFile, record, '\n');          //get a record from the file
        oneDegree = parseOneRecord(record);     //Parse the record into the object degree
        degrees.push_back(oneDegree.copyRanking()); //insert parsed record into vector
    }while (!inFile.eof());
    return;
}

// FUNCTION to display menu options to user and prompt them for entry
// return user selection
int getDegreeInfo (int lowMenu, int highMenu)
{
    int selection;

    cout<<"\n\tWELCOME TO OUR COLLEGE DEGREE INFORMATION CENTER.\n\n"
        <<"The following information was compiled by www.bankrate.com. \n"
        <<"Please select the number corresponding to your request:\n\n"
        <<"               Sort Degrees by Rank (1) \n"
        <<"             Sort Degrees by income (2) \n"
        <<"  Sort Degrees by Unemployment Rate (3) \n"
        <<"                Compare two Degrees (4) \n"
        <<"                               EXIT (0) \n\n"
        <<"          ENTER 1, 2, 3, 4, or 0: ";

    selection = getUserResponse(lowMenu, highMenu);    // get user answer

    return(selection);
}

// END OF ALL FUNCTIONS

// MAIN PROGRAM FOLLOWS

int main ()
{
    vector <Ranking> degrees;  // vector to hold degree data from dataset

    int menuItem(0);           // user choice from menu
    int comp1(0), comp2(0);    // hold user entered rankings of two degrees to compare

    ifstream inFile;           // declare Object variables of Class ifstreem to hold name of input file
	inFile.open ("DegreeRanking.txt"); // open input file and assign to the object

	if(inFile.fail()) terminateErrorMsg(F_OPEN_ERR);        //bad file opening, message user and exit

    getDataSet(inFile, degrees);  //read data into vector

    inFile.close(); // THE DATABASE IS NOW IN THE VECTOR

	cout<<"\nL4xx: ENTIRE DATABASE of "<<degrees.size()<<" records loaded into vector.\n";

	do
    {
        menuItem = getDegreeInfo(LOW_MENU,HI_MENU);

        switch (menuItem)
        {
            case 0:                                         //user exits program
                break;
            case 1:

                bSortRank(degrees);                         //sort by rank (low to high, in this scenario lower is better)
                putAllRecords(cout, degrees);
                break;

            case 2:
                bSortIncomeD(degrees);                      //sort by income (high to low)
                putAllRecords(cout, degrees);
                break;
            case 3:
                bSortUnRate(degrees);                       //sort by unemployment rate (low to high, in this scenario lower is better)
                putAllRecords(cout, degrees);
                break;
            case 4:
                bSortRank(degrees);                         //sort by rank just incase the vector was previously sorted another way
                putAllRecords(cout, degrees);               //print degrees to console so user knows the ranks
                cout << "\n\nEnter the rankings of the degrees you wish to compare: ";
                comp1 = getUserResponse(1, degrees.size()) - 1;            //get first degree rank
                cout << endl << comp1 + 1 << " and ";           //one added so console value reflects what they entered

                do
                {
                    comp2 = getUserResponse(1, degrees.size()) - 1;
                    if (comp1 == comp2)                     //degree ranks are the same so prompt user to enter something else
                    {
                        cout << "\nPlease compare two different degrees.\n";
                        cout << comp1 + 1 << " and ";
                        comp2 = getUserResponse(1, degrees.size()) - 1;
                    }
                }while(comp1 == comp2);                     //exit when user enters two different degree ranks

                putTitles(cout);
                cout << "\nDegree 1:\n";
                degrees[comp1].putOneRanking(cout);         //output information of degree with corresponding first entered rank
                cout << "\n\nDegree 2:\n";
                degrees[comp2].putOneRanking(cout);         //output information of degree with corresponding first entered rank
                break;
        }

    } while(menuItem);      //exit when user enter's 0/switch case 0

    return 0;
}
