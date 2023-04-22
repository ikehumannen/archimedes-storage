/*
8
*/

#include <iostream>     // General cin/cout input/output library
#include <fstream>      // Library needed for file I/O
#include <string>       // String functions library
#include <cstring>      // C-String functions library
#include <cstdlib>      // Need for exit function
#include <iomanip>      // Library needed for formatting instructions
#include <vector>       // Library needed for vectors
#include <cmath>        // Math Library
#include <cctype>       //need for toupper and tolower

using namespace std;   //Standard C++ vocabulary
const string R_DELIM ("\t");      // deliminator in record
const int LOW_MENU(0), HI_MENU(3);// Range of acceptable user menu entries

// Standard Error Return Codes
const int A_SIZE_ERR  (-1);   // Array Size Error Code
const int F_DATA_ERR  (-2);   // File Data Error Code
const int F_OPEN_ERR  (-3);   // File Open Error Code
const int EOF_FOUND   (-4);   // EOF Encountered Code
const int NOT_FOUND   (-5);   // Value Not Found Code


//  DECLARE and DEFINE REGULAR FUNCTIONS COPIED FROM OTHER PROGRAMS
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

void fixEntry(int lowValue, int highValue)  // Function to correct incorrect user menu entry
{
    cout<<"\tPlease Enter a Number between "<<lowValue<<" and "<<highValue<<":  ";
    cin.clear();
    cin.ignore(100,'\n');
} // end of fixEntry

// The next THREE functions are used to PARSE and format a record from the database
// based on delimiter provided in the argument

// Function to convert substring into an integer
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
}

// Function to convert substring into a string variable
string getString(string record, int &pos1, string delim)    // Convert the next substring from the
{                                                           // string record into a string variable
    int pos2;                                               // end of the substring
    string temp;                                            // substring

    pos2=record.find_first_of(delim, pos1);     // Locate delimiter
    if(pos2<0) pos2=record.find('\0');          // No delimiter, so must be at end of string
    temp=record.substr(pos1, pos2-pos1);        // Find string variable

    pos1=pos2+1;                                // Locate next delimiter

    return (temp);                              // return the string
}

// FUNCTION to convert a string to an integer
int convStrToInt (string sInt, string delim)
{
    int pos1(0), pos2(1);

    while(pos2>0)   // remove delimiter
    {
        pos2=sInt.find_first_of(delim, pos1);
        if(pos2>0) sInt.erase(pos2,1);
    }

    return(atoi(sInt.c_str()));
}
// END OF COPIED FUNCTIONS

// MAIN DATA STRUCTURE FOR PROJECT 08 and LAB 12

// DEFINE the class for holding name data obtained from the database
class Names
{
public:

void putOneObject(double total);     //display data at
Names copyName();                   //inserts data into current position of vector its called for

int rank;                           //holds rank of a row in file
string gender;                      //string for final output
string name;                        //string for name given in record
string count;                       //string for number of people given a specific name

private:                            // Don't have private functions/variables
};

// DEFINE the MEMBER FUNCTIONS for the class Names
void Names::putOneObject(double total)    // Write one formatted object class to the stream out
{

    cout.setf(ios::fixed);           //formatting console output for the percent of total
    cout.setf(ios::showpoint);

    cout << gender << " Data:"
         << "\nName: " << setw(12) << left << name
         << "\nRank: " << setw(12)  << left << rank
         << "\nCount: " << setw(12) << left << count
         << "\nTotal " << gender << " names: "<< setw(12) << left << static_cast<int>(total)    //cast total as int so its easier to read
         << "\n% of total: ";
         total =  100 *(stoi(count)/total);                                     //calculating total percent
         cout << setprecision(5) << setw(8) << left << total << "%\n\n";
}

Names Names::copyName()    // Copy an object of the class Names into another object
{
    Names n;

    n.rank        = rank;
    n.name        = name;
    n.count       = count;
    n.gender      = gender;

    return (n);
}
// END of class Names Member Function Definitions
// END of class Names DEFINITION


// DEFINE REGULAR FUNCTIONS SPECIFIC TO THIS PROGRAM

// FUNCTION to split a file record into two Objects
void parseOneRecord (string record, vector<Names> &mNames, vector<Names> &fNames, double &mTotal, double &fTotal)
{
    int position(0);

    Names oneMale, oneFemale;

    oneMale.rank    = getInt(record, position, R_DELIM);                //rank of one male record
    oneMale.name    = getString(record, position, R_DELIM);             //name in male record
    oneMale.count   = getString(record, position, R_DELIM);             //number of males given specific name
    oneMale.gender  = "Male";                                           //assigning male gender to this index
    oneFemale.rank  = oneMale.rank;                                     //names on same row so they have the same rank
    oneFemale.name  = getString(record, position, R_DELIM);             //name in female record
    oneFemale.count = getString(record, position, R_DELIM);             //number of females given specific name
    oneFemale.gender= "Female";                                         //assigning female gender to this index

    mTotal += convStrToInt(oneMale.count,",");                          // Add number of female names into sum of female names
    fTotal += convStrToInt(oneFemale.count, ",");                       // Add number of male names into sum of male names

    mNames.push_back(oneMale.copyName());   //put male data into vector
    fNames.push_back(oneFemale.copyName());   //put female data into vector
return;
}

//FUNCTION to get rank of name the user entered, I know linear search is far too simple but I spent hours trying to make binary
//search work and just couldn't
int indexSearchV (vector<Names> &rankNames, string name)
{
    for (int i = 0; i < rankNames.size(); i++)
        if (rankNames[i].name == name)
            return i;                               //name found
    return NOT_FOUND;                               //name not found, return -5

}//end index search

//FUNCTION to get name user wishes to search
string getName()
{
    string name;
    cout << "\n\nEnter the name that you wish to find the data for: ";
    cin >> name;
    for (int i = 0; i <= name.length(); i++) name[i] = tolower(name[i]);   //make all letters lower case to prevent problems
    name[0] = toupper(name[0]);                                            //make first letter uppercase as that is the standard in the record file
    return (name);
}

// MAIN PROGRAM FOLLOWS

int main ()
{


vector <Names> mNames, fNames;      //vectors for each gender

string record;                      //hold one record from file
string searchN;                     //user entered name that will be checked

int menuEntry;                      //input of user decision
int rResults(0);                    //rank of entered name
double fNameTotal(0), mNameTotal(0);   //total found for each gender

// Open datafile & test for open failure
ifstream inFile;
inFile.open ("RankNameCount1918.txt");

if(inFile.fail()) terminateErrorMsg(F_OPEN_ERR);        //bad file opening

    do
    {
        getline(inFile, record, '\n');                       // Read a record from datafile into string
        parseOneRecord(record, mNames, fNames, mNameTotal, fNameTotal);                     // Parse the record into a female and male object
    } while(!inFile.eof()); //end of loop to assign values to vectors

    inFile.close();         // Close datafile since THE DATABASE IS NOW IN THE ARRAY

cout << "This program takes a name entered and gets its popularity, how many were given it, "
     << "and the percent of total name assignments it achieved in the year 1918.";
    do          // Menu on while loop to get user response
    {
        searchN = getName(); //ask user what name they wish to search

        cout<<"SELECT NUMBER to search entered name against the list.\n"
            <<"                           Search Female Names [1]\n"
            <<"                             Search Male names [2]\n"
            <<"                           Search Both Genders [3]\n"
            <<"                                          EXIT [0]\n\n"
            <<"                   PLEASE ENTER YOUR SELECTION: ";
        while (!(cin>>menuEntry) || menuEntry<LOW_MENU || menuEntry>HI_MENU) fixEntry(LOW_MENU, HI_MENU);   //enter between 0 and 3
        cout<<endl;

    switch(menuEntry)
    {
        case 0:                                                 //user exits program
            break;
        case 1:                                                 //user wants to check entered name against female names

            rResults = indexSearchV(fNames, searchN);                              //Find rank of entered name if there is one
            if (rResults == NOT_FOUND) cout<< searchN << " not found in top 1000 FEMALE names from 1918\n\n";   //name not found
            else fNames[rResults].putOneObject(fNameTotal);                                                     //name found
            break;
        case 2:                                                  //user wants to check entered name against male names

            rResults = indexSearchV(mNames, searchN);                              //Find rank of entered name if there is one
            if (rResults == NOT_FOUND) cout<< searchN << " not found in top 1000 MALE names from 1918\n";       //name not found
            else mNames[rResults].putOneObject(mNameTotal);                                                     //name found
            break;
        case 3:                                                 //user wants to check entered name against both genders

            rResults = indexSearchV(fNames, searchN);                              //Find rank of entered name if there is one
            if (rResults == NOT_FOUND) cout<< searchN << " not found in top 1000 FEMALE names from 1918\n\n";  //name not found
            else fNames[rResults].putOneObject(fNameTotal);                                                     //name found

             rResults = indexSearchV(mNames, searchN);                             //Find rank of entered name if there is one
            if (rResults == NOT_FOUND) cout<< searchN << " not found in top 1000 MALE names from 1918\n";       //name not found
            else mNames[rResults].putOneObject(mNameTotal);                                                     //name found
            break;
        default:
            cout << "\nNo\n";                           //no idea how this could happen
    }


    }while(menuEntry!=0);       //user selected to end program
    return 0;
}//end of main
