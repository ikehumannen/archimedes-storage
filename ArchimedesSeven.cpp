/*
7
*/

#include <iostream>     // General input/output library for cin/cout
#include <cmath>        // Math Library
#include <fstream>      // Need for file I/O
#include <cstdlib>      // Need for exit function
#include <iomanip>      // Library needed for format manipulator instructions
#include <vector>       // Need for vectors

using namespace std;    //Standard C++ vocabulary

// STANDARD CONSTANTS

const int MAX_DISPLAY(25);      // Max number of records to display without a pause

// Range of acceptable user menu entries
const int LOW_MENU (0),
            HI_MENU(7);

// Standard Error Return Codes
const int A_SIZE_ERR  (-1);   // Array Size Error Code
const int F_DATA_ERR  (-2);   // File Data Error Code
const int F_OPEN_ERR  (-3);   // File Open Error Code
const int EOF_FOUND   (-4);   // EOF Encountered Code
const int NOT_FOUND   (-5);   // Value Not Found Code

// Display Selection Flag
const bool MALE   (0);
const bool FEMALE (1);

// Current Sort Status Flag
const int UNKWN  (0);       // Sort status unknown
const int M_RANK (1);       // Male array sorted by Rank
const int F_RANK (2);       // Female array sorted by Rank
const int M_NAME (3);       // Male array sorted by Name
const int F_NAME (4);       // Female array sorted by Name

// DECLARE STRUCTURE
struct Names        // Declare a structure for holding names and ranks
{
public:                   // Note use of UC
    string name;
    int rank;
};

//FUNCTION DECLARATIONS AND DEFINITIONS

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
} // end of terminateErrorMsg

void fixEntry(int lowValue, int highValue)  // Function to correct incorrect user menu entry
{
    cout<<"\tPlease Enter a Number between "<<lowValue<<" and "<<highValue<<":  ";
    cin.clear();
    cin.ignore(100,'\n');
} // end of fixEntry

//FUNCTION to load an integer array from a file
//  Pre:    Open file inFile, two arrays of struct Name of size aMax
//  Post:   File data in a[] up to location loopCount
void getNamesFromFile (ifstream &inFile, vector<Names> &mNames, vector<Names> &fNames)
{
    int recordCount(0);         // Counter to track number of records read from file
    int fileRank;               // Rank data from file
    string male, female;        // Names from file
/*
    while(inFile>>fileRank>>male>>female)
    {
        mNames[recordCount].rank = fileRank;      // Insert data into array of structures, mNames
        mNames[recordCount].name = male;
        fNames[recordCount].rank = fileRank;      // Insert data into array of structures, fNames
        fNames[recordCount].name = female;

        recordCount++;
        if (inFile.eof());                        // If no more data in file, do nothing, else expand vector
        else
        {   mNames.resize(recordCount+1);         // Size is one more than index to vector
            fNames.resize(recordCount+1);
        }
    }   // end of while loop, file read completed
    return;         //Good file reads, return number of records
*/
}   //end of getNamesFromFile


// FUNCTION to swap two CBR individual instances of structures of Names
void swapVal(Names &v1, Names &v2)
{
    Names temp = v1;
    v1 = v2;
    v2 = temp;
}

// FUNCTION to AlphaNumerically bubble sort a vector of structure Names
// Entire structure is moved to the right based on member variable rank
void bSortRank(vector<Names> &v)
{
    int vSize = v.size();

    for (int i = vSize-1; i >= 0; i--)           //Traverse vector from right to left
        for (int j = 0; j < i; j++)              //Traverse vector from left to right
            if (v[j].rank > v[j+1].rank)         //Find larger of two adjacent values
            {
                swapVal(v[j], v[j+1]);          //Swap values, move larger to the right
            }
}

// FUNCTION to Rank bubble sort a vector of structure Names
// Entire structure is moved to the right based on member variable name
void bSortName(vector<Names> &v)
{
    int vSize = v.size();

    for (int i = vSize-1; i >= 0; i--)           //Traverse vector from right to left
        for (int j = 0; j < i; j++)              //Traverse vector from left to right
            if (v[j].name > v[j+1].name)         //Find larger of two adjacent values
            {
                swapVal(v[j], v[j+1]);          //Swap values, move larger to the right
            }
}

// FUNCTION for user to enter a name.
string getName()
{
    string name;
    cout << "\nEnter the name that you wish to find the ranking of: ";
    cin >> name;
   // for (int i = 0; i <strlen(name); i++)             //Check for numbers in name, might scrap as it limits freedom and you are able to name someone with a number
   //     if(isdigit(name[i]))
    return (name);
}

// FUNCTION to search a vector of structure rankNames
// of size Elements based on member variable name
// returns index for name or NOT_FOUND
int indexSearchV (vector<Names> &rankNames, string name)
{
    int size = rankNames.size();

    int startPoint(0), midPoint, oldMidPoint(0);
    int endPoint = size-1;                  // Don't out of bounds index array
    midPoint = (endPoint+startPoint)/2;     // Set initial midPoint value for comparison

    while (midPoint!=oldMidPoint)
    {
        cout << rankNames[midPoint].name;
        cout << rankNames[midPoint].rank << endl;
        if(name < rankNames[midPoint].name)        endPoint = midPoint;     //Search lower half next time
        else if(name > rankNames[midPoint].name)   startPoint = midPoint;   //Search upper half next time
        else if (name == rankNames[midPoint].name) return rankNames[midPoint].rank;
        oldMidPoint=midPoint;
        midPoint = (endPoint + startPoint) / 2;
    }   // end of loop

        if(name==rankNames[endPoint].name)        return endPoint;    // Fix for possible integer division boundary error
        else if(name==rankNames[startPoint].name) return startPoint;

    return NOT_FOUND;      // Name not found
}   //end index search

void putTitles(ostream &out, bool fm)   // out (console or file output), fm (Flag for female or male records)
{
    if (fm) out <<setw(15)<<"FEMALES"<<setw(5)<<" RANK"
                <<setw(19)<<"FEMALES"<<setw(5)<<" RANK"
                <<setw(19)<<"FEMALES"<<setw(5)<<" RANK"
                <<setw(19)<<"FEMALES"<<setw(5)<<" RANK"<<endl;
    else    out <<setw(15)<<"MALES"<<setw(5)<<" RANK"
                <<setw(19)<<"MALES"<<setw(5)<<" RANK"
                <<setw(19)<<"MALES"<<setw(5)<<" RANK"
                <<setw(19)<<"MALES"<<setw(5)<<" RANK"<<endl;
}

void putOneName(ostream &out, Names putName)    // out (console or file output),  putName (Structure of Names)
{                                               // NOTE: Only sending a single instance of structure, not a vector
    out<<setw(15)<<putName.name<<setw(5)<<putName.rank;
}

void putAllNames(ostream &out, vector<Names> putNames, bool fm)    // out (console or file output),
{                                   // putName (Structure of Names), sizeA (size of data array, fm (Female or Male Names)
  int sizeV = putNames.size();      // Get vector size
  int loop(-1);                     // Loop counter for screen display
  int vLoop(0);
  putTitles(cout, fm);

//cout<<endl<<putNames[vLoop].name<<endl;

  while(vLoop<sizeV)        // Output all valid data in vector
  {
      if(loop<MAX_DISPLAY-1)          // Output up to the maximum number of lines on screen MAX_DISPLAY
        {   putOneName(cout, putNames[vLoop]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+1]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+2]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+3]);
            cout<<endl;
            loop++;                             // increment row counter
            vLoop +=4;                          // Four items displayed
        }
        else        // Maximum lines on display screen is full, pause and reset
        {
            system("pause");
            putTitles(cout, fm);                // Display header titles
            putOneName(cout, putNames[vLoop]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+1]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+2]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop+3]);
            cout<<endl;
            loop=0;                             // Screen is at top, set counter to zero
            vLoop +=4;                          // Four items displayed
        }

        if      (vLoop+4<sizeV);                       // What if less than four items remaining in array?
        else if (vLoop+1==sizeV)
        {
            putOneName(cout, putNames[vLoop++]);
            cout<<endl;
        }
        else if (vLoop+2==sizeV)
        {   putOneName(cout, putNames[vLoop++]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop++]);
            cout<<endl;
        }
        else if (vLoop+3==sizeV)
        {   putOneName(cout, putNames[vLoop++]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop++]);
            cout<<"\t";
            putOneName(cout, putNames[vLoop++]);
            cout<<endl;
        }
  } // End while loop

} // End of putAllNames

// END OF FUNCTIONS


// In the MAIN, the code reads the database and dumps records to the display
int main()
{
    vector<Names> fNames(1), mNames(1);   // Declare a vector of structures

    string searchN;         // User Requested Name in Search
    int mSort, fSort;       // Sort Status of Array
    int loop;               // Count number of iterations of a loop
    int menuEntry;          // User menu selection
    int rResults;           // Value/Error return from function

    ifstream inFile;        // declare Object variables of Class ifstreem to hold name of input file

    inFile.open("babynames2018.txt");                   // open input file and assign to the object

    if(inFile.fail()) terminateErrorMsg(F_OPEN_ERR);    //Bad file opening, message user and exit

    getNamesFromFile(inFile, mNames, fNames);    //Read data from open file into two vectors

    inFile.close();     // Done with inFile, so close to avoid corruption
    mSort = UNKWN;      // Set Initial Sort to Unknown
    fSort = UNKWN;      // Set Initial Sort to Unknown

    do          // Menu on while loop to get user response
    {
        cout<<"SELECT NUMBER for DISPLAYING NAMES and POPULARITY RANKINGS from 2018.\n"
            <<"                    Female Names in Rank Order [1]\n"
            <<"                      Male Names in Rank Order [2]\n"
            <<"            Female Names in Alphabetical Order [3]\n"
            <<"              Male Names in Alphabetical Order [4]\n"
            <<"                      Search for a Female Name [5]\n"
            <<"                        Search for a Male Name [6]\n"
            <<"        Search for both a Female and Male Name [7]\n"
            <<"                                          EXIT [0]\n\n"
            <<"                   PLEASE ENTER YOUR SELECTION: ";
        while (!(cin>>menuEntry) || menuEntry<LOW_MENU || menuEntry>HI_MENU) fixEntry(LOW_MENU, HI_MENU);

        cout<<endl;

        switch(menuEntry)                           //Switch on User menu entry
        {
            case 0:
                break;                              // User wants to end the program
            case 1:                                 // Display All Female Names in Rank Order

                if (fSort != F_RANK) {bSortRank(fNames); fSort=F_RANK;}  // Call to Rank sort, if needed
                cout<<"POPULAR FEMALE NAMES FROM 2018 IN RANK ORDER \n\n";
                putAllNames(cout,fNames,FEMALE);

                break;
            case 2:                                 // Display All Male Names in Rank Order

                if (mSort != M_RANK) {bSortRank(mNames); mSort=M_RANK;}  // Call to Rank sort, if needed
                cout<<"POPULAR MALE NAMES FROM 2018 IN RANK ORDER \n\n";
                putAllNames(cout,mNames,MALE);

                break;
            case 3:                                // display All Female names in Alpha Order

                if (fSort != F_NAME) {bSortName(fNames); fSort=F_NAME;} // Call to Alpha Sort, if needed
                cout<<"POPULAR FEMALE NAMES FROM 2018 IN ALPHA ORDER \n\n";
                putAllNames(cout,fNames,FEMALE);

                break;
            case 4:                                 // Display All Male Names in Alpha Order

                if (mSort != M_NAME) {bSortName(mNames); mSort=M_NAME;} // Call to Alpha Sort, if needed
                cout<<"POPULAR MALE NAMES FROM 2018 IN ALPHA ORDER \n\n";
                putAllNames(cout,mNames,MALE);

                break;
            case 5:                             // Search for a Female Name

                if (fSort != F_NAME) {bSortName(fNames); fSort=F_NAME;} // Call to Alpha Sort, if needed
                searchN=getName();              // Call to get name from User
                rResults = indexSearchV (fNames, searchN);//get rank of name entered from female column
                cout<<endl<<searchN;
                if (rResults == NOT_FOUND) cout<<" not found in top 1000 FEMALE names from 2018\n";
                else cout<<" is "<< fNames[rResults].name <<" out of 1000 of the most popular FEMALE name in 2018\n";

                break;
            case 6:                     // Search for a Male Name

                if (mSort != M_NAME) {bSortName(mNames); mSort=M_NAME;} // Call to Alpha Sort, if needed
                searchN=getName();              // Call to get name from User
                rResults = indexSearchV (mNames, searchN);//get rank of name entered from male column
                cout<<endl<<searchN;
                if (rResults == NOT_FOUND) cout<<" not found in top 1000 MALE names from 2018\n";
                else cout<<" is "<< rResults <<" out of 1000 of the most popular MALE name in 2018\n";

                break;
            case 7:

                if (mSort != M_NAME) {bSortName(mNames); mSort=M_NAME;} // Call to Alpha Sort, if needed
                if (fSort != F_NAME) {bSortName(fNames); fSort=F_NAME;} // both genders need to have been sorted at some point
                searchN=getName();              // call to get name from User
                rResults = indexSearchV (mNames, searchN);//get rank of name entered from male column
                cout<<endl<<searchN;
                if (rResults == NOT_FOUND) cout<<" not found in top 1000 MALE names from 2018\n";
                else cout<<" is "<<rResults<<" out of 1000 of the most popular MALE name in 2018\n";

                rResults = indexSearchV (fNames, searchN);//get rank of name entered from female column
                cout<<endl<<searchN;
                if (rResults == NOT_FOUND) cout<<" not found in top 1000 FEMALE names from 2018\n";
                else cout<<" is "<<rResults<<" out of 1000 of the most popular FEMALE name in 2018\n";

                break;
            default:
                cout<<"\nIncorrect response. Try again\n"; // This should never happen
        }   // End Switch Statement

        cout<<endl<<endl;     // format line for next iteration<<endl;

    }while(menuEntry!=0);

    return 0;

}   // end of main()
