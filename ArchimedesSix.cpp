/*
6
*/
#include <iostream>     //needed for cout and cin
#include <fstream>      //needed for file manipulation
#include <cstdlib>      //needed to get average
#include <iomanip>      //needed for output manipulation

using namespace std;

const int PATIENTS(4); //max number of patients
const int TEMPS(8); //temperatures gathered per patient

const int L_MENU(0);    //bounds for lowest possible menu entry
const int H_MENU(4);    //bounds for highest possible menu entry


//STANDARD ERROR CODES
const int A_SIZE_ERR  (-1);   // Array Size Error
const int F_DATA_ERR  (-2);   // File Data Error
const int F_OPEN_ERR  (-3);   // File Open Error
const int EOF_FOUND   (-4);   // EOF Encountered

//FUNCTION DECLARATIONS

//FUNCTION gets the patient user wishes to have data for based on menu input
int selectPatient();

//FUNCTION gets file based on input in selectPatient()
//Reads name and assigns it to patName[]
//calls readIntoA2D() to get number of loops
//calculates average temperature will removing those that werent read
//menuSelect is equal to the user entry in selectPatient()
void getTemp(int patTemp[][PATIENTS], string patName[], int numTemp[], double aveTemp[], int menuSelect, bool allTemp, int noReads[]);

//FUNCTION called within getTemp()
//writes data into patTemp based on column number given
//checks different EOF conditions
//sets noReads[column] to zero and adds one each 0 temperature encountered
int readIntoA2D(ifstream &inFile, int patTemp[][PATIENTS], int column, int noReads[]);

//outputs message informing user on how to read output table
void titleOutput(ostream &out);

void outputData(ostream &out, int patTemp[][PATIENTS], string patName[], int numTemp[], double aveTemp[], int noReads[], int choice);
//FUNCTION used for incorrect user entry
void entryError();

//FUNCTION outputs file handling error messages and exits program
void errorMsg(int errorRep);

//END OF FUNCTION DELCARATIONS

int main()
{
    int choice;                 //menu number of patient being accessed, chosen in selectPatient()

    bool endLoop(true);                //repeat do loop in main function
    bool allTemp;                   //true when getting data for all patients

    int patTemp[TEMPS][PATIENTS];   //contains temperatures for each patient

    int numTemp[PATIENTS];          //holds the number of temperatures per patient
    int noReads[PATIENTS];          //holds the number of no read temperatures per patient
    double aveTemp[PATIENTS];       //holds the average temperature for each patient (not including no reads)
    string patName[PATIENTS];       //holds the name of each patient

    ofstream outFile;               //assign object to output file

    cout << "\nThree patients are suspected of contracting the coronavirus. Their names, along with eight temperature readings for a 24-hour period"
         << "can be acessed below based on the users choice when prompted\n";
    do
    {
        choice = selectPatient();                   //retrieve user input

        if (choice == 0) endLoop = 0;               //user decides to exit program

        else if(choice == 4)                        //user decides to access all data
        {
            allTemp = true;                         //all patients selected

            for (int i = 0; i < PATIENTS; i++)                                          //each iteration assigns temperatures to patTemp
                getTemp (patTemp, patName, numTemp, aveTemp, i, allTemp, noReads);

            outFile.open("results.txt");                                                //output file
            titleOutput(outFile);                                                       //output titles to file

            outputData(outFile, patTemp, patName, numTemp, aveTemp, noReads, choice);   //output results to file
            outFile.close();                                                            //close output file

            titleOutput(cout);                                                          //output titles to console
            outputData(cout, patTemp, patName, numTemp, aveTemp, noReads, choice);      //output results to console
        }

        else
        {
            allTemp = false;                                                            //not reading all files
            getTemp(patTemp, patName, numTemp, aveTemp, choice, allTemp, noReads);      //get tempature values from specified file

            outFile.open("results.txt");                                                //output file
            titleOutput(outFile);                                                       //output titles to file

            outputData(outFile, patTemp, patName, numTemp, aveTemp, noReads, choice);   //output results to file
            outFile.close();                                                            //close output file


            titleOutput(cout);                                                          //output titles to console
            outputData(cout, patTemp, patName, numTemp, aveTemp, noReads, choice);      //output results to console
        }
    cout << "Output can be viewed in a file under the name results.txt.";
    system("pause");         //windows pause    //give user time to read console
    system("cls");            //windows clear   //clear console output
    //system("clear");          //linux clear

    }while(endLoop);        //exit main loop
    return (0);
}

//Pre: runs at beginning of program, entry bounds set by constants L_MENU and H_MENU
//Post: returns the value selected by user
int selectPatient()
{
    int userInput;
    int endLoop = 1;          //control when the loop ends


    cout << "\nSelect the patient temperatures you wish to access.\n"
         << "\n\t\t1. \t\tPatient 1"
         << "\n\t\t2. \t\tPatient 2"
         << "\n\t\t3. \t\tPatient 3"
         << "\n\t\t4. \t\tAll Patient temperature records"
         << "\n\t\t0. \t\tExit Program"
         << "\nEnter the corresponding integer to select choice\n";

    while(endLoop)          //get an integer from user with error checking
    {
        if (!(cin >> userInput)) entryError();          //user entered a non-integer
        else if (L_MENU == H_MENU) endLoop = 0;   //grab any number as value doesn't matter
        else if (userInput < L_MENU) cout << "\nERROR. Enter a larger integer: ";
        else if (userInput > H_MENU) cout << "\nERROR. Enter a smaller integer: ";
        else endLoop = 0; //user entered a valid number
    } //end loop
    return(userInput);
}

//Pre: valid entry was given at selectPatient()
//Post: patName[], aveTemp[], noReads[], and numTemp[] given values from file that was read
void getTemp(int patTemp[][PATIENTS], string patName[], int numTemp[], double aveTemp[], int menuSelect, bool allTemp, int noReads[])
{
    int column;                 //the "column" where patient temps are being read into
    int total(0);               //sum of patient temperatures
    int readResult;
    ifstream inFile;            //Declare object that reads file data


    switch(menuSelect)                  //open file depending on value of menuSelect/choice
    {
        case 1:
            inFile.open("patient_1.txt");
            break;
        case 2:
            inFile.open("patient_2.txt");
            break;
        case 3:
            inFile.open("patient_3.txt");
            break;
        default:
            cout << "ERROR (of some sort)";
    }

    if(inFile.fail()) errorMsg(F_OPEN_ERR);     //problem occured while opening file, tell user and exit program

    if(allTemp) column = menuSelect - 1;        //fixes column format in second index when getting data from multiple files
    else column = 0;                            //single patient selected

    inFile >> patName[column];                                    //read in name from file
    readResult = readIntoA2D(inFile, patTemp, column, noReads);   //read in data from inFile opened at the switch statement
    inFile.close();                                               //file no longer needed

    if (readResult < 0) errorMsg(readResult);                     //issue with data found
    else numTemp[column] = readResult;                            //data was cleared

    for( int n = 0; n < numTemp[column]; n++)                   //get total temp for a column
        total=total+patTemp[n][column];

    aveTemp[column] = static_cast<double>(total)/(numTemp[column]-noReads[column]);       //average of total temperatures,
                                                                                            //no reads subtracted to remove from calc.
}//end of getTemp

//Pre: inFile was opened successfuly
//Post: number of noReads[] acquired, temperatures for specific file written, and amount of temperatures returned
int readIntoA2D(ifstream &inFile, int patTemp[][PATIENTS], int column, int noReads[])
{
    noReads[column] = 0;                    //initialize number of no reads for this patient
    int loopCount(0);                       //keeps track of number of loops, will be used to calculate totals
    double currentValue(0);                 //value currently being read in
    while ((inFile>>currentValue) && (loopCount < TEMPS))
    {
        patTemp[loopCount][column]=currentValue;         //put in data at specific column
        if(currentValue == 0) noReads[column]++;         //temperature not recorded, increase noReads
        loopCount++;
    }
     if(inFile.eof() && loopCount <= TEMPS) return (loopCount);            //read successful
     else if (!inFile.eof() && loopCount >= TEMPS) return(A_SIZE_ERR);    //ERROR. More File data than Array can handle
     else if (!inFile.eof() && loopCount < TEMPS) return (F_DATA_ERR);    //ERROR. Non-Integer in File
     else return (0);                                                //ERROR. No data in file
}//end of readintoA2D

//Pre: EOF for infile found, all files read and written to patTemp[][] (if there was more than one)
//Post: message explaining table output to file and console
void titleOutput(ostream &out)
{
    //message put at the top of the output table.
    out << "\nTemperature (fahrenheit) shown under each respective patient name with their averages and number of missing temps listed at the bottom\n\n";
}
//Pre: title output to outfile and console
//Post: Patient Data written to outfile and console
void outputData(ostream &out, int patTemp[][PATIENTS], string patName[], int numTemp[], double aveTemp[], int noReads[], int choice)
{
    out.setf(ios::fixed);                   //format output
    out.setf(ios::showpoint);
    out.precision(1);
    int time(0);                            //time recorded in 3 hour segments
    out << "TIME    ";
    if (choice == 4)                                //all files selected
    {
        for (int i = 0; i < PATIENTS; i++) out << "\t" << patName[i];   //output patient names
        out << endl;
        for(int i = 0; i < TEMPS; i++)                          //outputs patient temperatures
        {
            time += 300;                            //time increases by 3 hours each loop
            out << time << "\t";
            for (int n = 0; n < PATIENTS-1; n++)    //temperatures for all 3 patients, each loop creates a new row
                out << "\t\t" << patTemp[i][n];     //row of temperatures outputted
            out << endl;
        }
        out << "AVERAGE\t";                         //outputs average temperature per patient
        for (int i = 0; i < PATIENTS-1; i++) out << aveTemp[i] << "\t\t\t";
        out << endl;
        out << "NO READINGS\t";                     //outputs number of 0 temperature readings
        for (int i = 0; i < PATIENTS-1; i++) out << noReads[i] << "\t\t";
        out << endl;
    }
    else                                                    //one file selected
    {
        out << patName[0];                                  //output patient name
        out << endl;

        for(int i = 0; i < TEMPS; i++)                      //outputs temperature for one patient
        {
            time += 300;                                    //time increases by 3 hours each loop
            out << setw(5) << time;
            out << "\t\t" << patTemp[i][0];
            out << endl;
        }
        out << "AVERAGE: \t";
        out << setw(5) << aveTemp[0] << "\t" << endl;
        out << "NO READINGS\t";
        out << setw(5) << noReads[0] << "\t" << endl;
    }
}
void entryError()
{
    cin.clear();
    cin.ignore(100,'\n');
    cout<<"\nERROR. Enter an integer: ";    //User did not enter an integer
}

void errorMsg (int errorRep)
{
    switch(errorRep)  //Switch on error value send by another function/code
    {
        case A_SIZE_ERR:
            cout<<"\nArray to small to read data from file. PROGRAM TERMINATED\n";
            break;
        case F_DATA_ERR:
            cout<<"\nNon-Integer encountered during file read. PROGRAM TERMINATED\n";
            break;
        case F_OPEN_ERR:
            cout<<"\nFile opening error. PROGRAM TERMINATED.\n";
            break;
        case EOF_FOUND:
            cout<<"\nUnexpected End of File Encountered. PROGRAM TERMINATED.\n";
            break;
        default:
            cout<<"\nOther Error. PROGRAM TERMINATED.\n";
    }
    exit(errorRep);
}
