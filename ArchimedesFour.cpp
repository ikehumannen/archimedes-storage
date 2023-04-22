/*
4
*/
#include <iostream>     //needed for cin and cout
#include <fstream>      //needed for file I/O


using namespace std;

const int NUM_FILES(3);                 //number of files being read, prevents for loop at lines 55-88 from going past limit

//function declarations

//function gets the name, high score, low score, and calculates average from provided file. Located on lines 112-139
int getStudentData(ifstream &inputData, string &name, int &highScore, int &lowScore, double &avg);

//prints introduction to program and table of categories that determine scholarship elegibility to the console. Located on lines 142-147
void outputIntro();

//prints introduction to program and table of categories that determine scholarship elegibility to the output file. Located on lines 150-155
void outputIntroFile(ofstream &outputData);

//prints student name, high score, low score, average, and number of disqualifications under the table to the console. Located on lines 158-161
void outputScores(string name, int highScore, int lowScore, double avg, int disq);

//prints student name, high score, low score, average, and number of disqualifications under the table to the output file. Located on lines 164-167
void outputScoresFile (string name, int highScore, int lowScore, double avg, int disq, ofstream &outputData);

//end of function declarations

int main()
{

string name;                            //name of current student
string winner;                          //name of winning student

int highScore;                          //high score of current student
int lowScore;                           //lowest score of current student
int numDisq;                            //number of disqualified scores for current student

double avgScore;                        //average score for current student
double winnerAvg;                       //average score of winning student

ifstream inputData;                     //represents the current file being read
ofstream outputData;                    //where final output will be put

outputData.open("StudentWinner.txt");   //creating and opening output file

cout.setf(ios::fixed);                  //set decimal place for read out of averages
cout.setf(ios::showpoint);
cout.precision(2);


outputIntro();                          //prints intro to console
outputIntroFile(outputData);            //prints intro to output file

    for(int x=1; x <= NUM_FILES; x++)   //loops until x=4, each iteration it changes input file
    {
        switch(x)
        {
        case 1:
            inputData.open("student_1.txt");                //grades for BECKY_THATCHER
            break;
        case 2:
            inputData.open("student_2.txt");                //grades for SAWYER_TOM
            break;
        case 3:
            inputData.open("student_3.txt");                //grades for FINN_HUCK
            break;
        }

        if(inputData.fail())                                //user is missing files or incorrectly named them
        {
            cout << "Missing one or all of requested files.";
            exit (22);
        }

        numDisq = getStudentData(inputData, name, highScore, lowScore, avgScore);   //Gets name, high score, low score, and average
                                                                                    //from current file in addition to number of disqualified grades
        inputData.close();                                                          //Current input file closed

            if (numDisq < 2 && avgScore > winnerAvg)                                //Student cannot win if they didnt submit more than 2 projects
            {                                                                       //If student has not missed more than 2 projects and has a higher average than
                winner = name;                                                      //the current over all high they are declared the winner
                winnerAvg = avgScore;
            }

        outputScores(name, highScore, lowScore, avgScore, numDisq);                //prints results of each file read by computer to screen
        outputScoresFile(name, highScore, lowScore, avgScore, numDisq, outputData);    //prints results of each read to output file StudentWinner.txt
    }   //end of for loop, all data on files read

    cout << "\n The winning student is " << winner << " with an average score of " << winnerAvg <<".\n" << endl;           //output winner to console
    outputData << "\n The winning student is " << winner << " with an average score of " << winnerAvg <<".\n" << endl;     //output winner to StudentWinner.txt
    outputData.close();                                                                                                    //close output file StudentWinner.txt

    cout << "\nScholarship results can be found in the file, StudentWinner.txt\n" << endl;
    system("pause");                                                                             //gives user chance to read console output
    cout << endl;

    return 0;
}

//function definitions

//gets data from a student file, made call by reference so changes are known in main
int getStudentData(ifstream &inputData, string &name, int &highScore, int &lowScore, double &avg)
{
    int currentScore;                       //value of current score read from file
    int validScores (0);                    //number of non zero scores from current file
    int totalScore (0);                     //total of all non zero scores
    int numDisq (0);                        //number of zero grades

    highScore = 0;                          //values being reset
    lowScore = 1000;
    avg = 0;

    inputData>>name;                       //sets name of whose scores are being read
    while(inputData>>currentScore)         //loops until EOF
    {
        if(currentScore == 0) numDisq += 1;         //project with a grade of zero
        else                                        //non zero grade
        {
            validScores += 1;
            totalScore += currentScore;

            if (currentScore>highScore) highScore = currentScore;       //determines personal high score
            if (currentScore<lowScore) lowScore = currentScore;         //determines personal low score
        }
    }// exit while loop, all data read
    avg = static_cast<double>(totalScore)/validScores;                  //gets precise average

    return(numDisq);            //needed to determine if can be winner by checking zero grades
}

//prints introduction to what program does to console
void outputIntro()
{
    cout << "The table below lists student scholarship entries. To win a student " <<endl
         << "must complete at least five of seven projects with the highest average score.\n" << endl
         << "   STUDENT        HI_SCORE   LOW_SCORE   AVG_SCORE   DISQUALIFIED \n" << endl;
}

//prints introduction to what program does to output file
void outputIntroFile(ofstream &outputData)
{
    outputData  << "The table below lists student scholarship entries. To win a student " <<endl
                << "must complete at least five of seven projects with the highest average score.\n" << endl
                << "   STUDENT        HI_SCORE   LOW_SCORE   AVG_SCORE   DISQUALIFIED \n" << endl;
}

//prints results for each student to console
void outputScores(string name, int highScore, int lowScore, double avg, int disq)
{
    cout << name << "\t     " << highScore << "\t\t" << lowScore<< "\t   " << avg << "\t   " << disq << endl;
}

//prints results for each student to output file
void outputScoresFile (string name, int highScore, int lowScore, double avg, int disq, ofstream& outputData)
{
    outputData << name << "\t     " << highScore << "\t    " << lowScore << "\t   " << avg << "\t   " << disq << endl;
}

//end of function definitions
