#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <windows.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

int numberOfTeams;
string path; // the path of the Input File
map<string, int>nameToId;

// this function to clear the console screen
void clearScreen()
{
    // Check if the operating system is Windows
    #ifdef _WIN32                  
        std::system("cls");         // If it is, execute the "cls" command to clear the console screen
    // If the operating system is not Windows (i.e., Unix or Linux-based systems)
    #else
        std::system("clear");       // Execute the "clear" command to clear the console screen
    #endif
}

// adjust the display of the program
void gotoxy(int x, int y) {
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// check if the file is CSV file or not
bool isCSVFile(char* filename) 
{
    // Get the position of the last dot in the filename
    size_t len = strlen(filename);
    size_t dotpos = len;
    for (size_t i = 0; i < len; i++) 
    {
        if (filename[i] == '.')dotpos = i;
    }

    // Check if the dot was found and if the extension is ".csv"
    if (dotpos != len && strcmp(&filename[dotpos], ".csv") == 0) 
    {
        path = string(filename); // if The File is already .csv then make path = the path of this file
        return true;
    } 
    else return false;
}

void get_file()
{   
    system("cls"); 
	gotoxy(10, 5);
	cout << "<< Uploud a Comma Separated File (.CSV) >>";
	gotoxy(20, 7);
	cout << "The file should represent matches' statistics";
    gotoxy(25, 8);
    cout << "of every game during the rounds of the English Premier League season";

    // Initialize variables for file selection
    OPENFILENAME ofn;           // common dialog box structure
    char szFile[260];           // buffer for file name
    HWND hwnd = NULL;           // owner window
    szFile[0] = '\0';

    // Initialize OPENFILENAME struct
    ZeroMemory(&ofn, sizeof(ofn));                      // Clear memory for struct
    ofn.lStructSize = sizeof(ofn);                      // Set size of struct
    ofn.hwndOwner = hwnd;                               // Set owner window (null in this case)
    ofn.lpstrFile = szFile;                             // Set buffer for file name
    ofn.nMaxFile = sizeof(szFile);                      // Set maximum length of file name
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";        // Set filter for file types
    ofn.nFilterIndex = 1;                               // Set default filter index
    ofn.lpstrFileTitle = NULL;                          // Set buffer for file title (not used in this case)
    ofn.nMaxFileTitle = 0;                              // Set maximum length of file title (not used in this case)
    ofn.lpstrInitialDir = NULL;                         // Set initial directory (not used in this case)
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  // Set options for file selection

    gotoxy(12, 12);
    cout << "Press 'u' to browse: ";

    while (true)
    {
        // Check if the key is pressed
        if (_getch() == 'u') 
        {
            // Display the Open dialog box.
            if (GetOpenFileName(&ofn)==TRUE) 
            {
                if (isCSVFile(szFile)) {
                    // return and go to Take That File
                    return;
                }
                else {
                    gotoxy(20, 14); 
                    cout << "The file should be a .CSV file";
                    gotoxy(12, 16); 
                    cout << "Press 'u' to browse: ";
                }
            }
        }
        else {
            gotoxy(12, 12); 
            cout << "Press 'u' to browse: ";
        }
    }
}

// This class represents a match between two teams
class Match
{
    private:
        int homeTeamId;         // The id of the home team
        int awayTeamId;         // The id of the away team
        int round;              // The round number of the match
        int day;                // The day of the match
        int month;              // The month of the match
        int year;               // The year of the match
        int goalsForHome;       // The number of goals scored by the home team
        int goalsForAway;       // The number of goals scored by the away team
        char winner;            // d = Draw, h = Home, a = Away

    public:
        // Constructor to initialize the object
        Match(int r, int d, int m, int y, int gh, int ga, char w)
        {
            this->round         = r;
            this->day           = d;
            this->month         = m;
            this->year          = y;
            this->goalsForHome  = gh;
            this->goalsForAway  = ga;
            this->winner        = w;
        }

        // Overloaded operator < to sort matches based on their round number in ascending order
        bool operator<(const Match& other)
        {
            return this->round < other.round;
        }

        // Static method to compare two matches based on their date
        static bool compare(const Match& a, const Match& b) 
        {
            if (a.year != b.year)
            {
                return a.year < b.year;
            }
            else if (a.month != b.month)
            {
                return a.month < b.month;
            }
            else
            {
                return a.day < b.day;
            }
        }
};

// This class represents a team in the league
class Team
{
    private:
        string Name;                // The name of the team
        int Id;                     // The id of the team
        int matchesPlayed;          // Number of matches played by the team
        int points;                 // Number of points earned by the team
        int win;                    // Number of matches won by the team
        int lose;                   // Number of matches lost by the team
        int draw;                   // Number of matches drawn by the team
        int goalsFor;               // Number of goals scored by the team
        int goalsAgainst;           // Number of goals conceded by the team

    public: 
        // Constructor to initialize the object
        Team(string name, int id, int mp = 0, int p = 0, int w = 0, int l = 0, int d = 0, int gf = 0, int ga = 0)
        {
            this->Name           = name;
            this->Id             = id;
            this->matchesPlayed  = mp;
            this->points         = p;
            this->win            = w;
            this->lose           = l;
            this->draw           = d;
            this->goalsFor       = gf;
            this->goalsAgainst   = ga;
        }

        // Method to reset all the values of the team
        void reset()
        {
            this->matchesPlayed  = 0;
            this->points         = 0;
            this->win            = 0;
            this->lose           = 0;
            this->draw           = 0;
            this->goalsFor       = 0;
            this->goalsAgainst   = 0;
        }

        // Method to print the details of the team
        void print()
        {
            // TODO: Implementation pending
        }

        // Overloaded operator < to compare two teams based on their points
        bool operator<(const Team& other)
        {
            return this->points > other.points;
        }
};

vector<Team>team;

// This class represents a league with teams and their matches schedule
class League
{
    private:
        bool isTeamsSorted;                     // flag to check if teams are sorted
        vector<vector<Match>> teamSchedule;     // adjacency list of the graph

    public: 
        // Constructor to initialize the object
        League()
        {
            isTeamsSorted = false;
            resetTeams();
        }

        // Method to reset all the teams
        void resetTeams()
        {
            for (int i = 0; i < team.size(); i++)
            {
                team[i].reset();
            }
        }

        // Method to add a match between two teams to the schedule
        // Parameters:
        // homeId - id of the home team
        // awayId - id of the away team
        // match - Match object representing the match
        void addMatch(int homeId, int awayId, Match match)
        {
            // Check if both teams exist in the schedule
            if (max(homeId, awayId) >= (int)team.size())
            {
                cout << "Raise Error: You Forget to push a Team in The team Vector\n";
                assert(0);
            }

            // Check if both teams exist in the teamSchedule
            if (max(homeId, awayId) >= (int)teamSchedule.size())
            {
                cout << "Raise Error: You Forget to push a Team in The teamSchedule Vector\n";
                assert(0);
            }

            // Add the match to the home team's schedule
            this->teamSchedule[homeId].push_back(match);

            // Mark that the teams are not sorted
            isTeamsSorted = false;
        }

        // Method to perform DFS on the rounds
        void DFS_Rounds()
        {
            // TODO: Implementation pending  =>  samar
        }

        // Method to perform DFS on the dates
        void DFS_Date()
        {
            // TODO: Implementation pending  =>  mod
        }

        // Method to sort the matches based on rounds
        void sortMatchesBasedOnRounds()
        {
            for (int i = 0; i < this->teamSchedule.size(); i++)
            {
                sort(teamSchedule[i].begin(), teamSchedule[i].end());
            }
        }

        // Method to sort the matches based on date
        void sortMatchesBasedOnDate()
        {
            for (int i = 0; i < this->teamSchedule.size(); i++)
            {
                sort(teamSchedule[i].begin(), teamSchedule[i].end(), Match::compare);
            }
        }

        // Method to sort the teams based on their standing
        void sortTeams()
        {
            // If teams are already sorted, return
            if (isTeamsSorted) 
            {
                return;
            }

            // Sort the teams
            sort(team.begin(), team.end());

            // Mark that the teams are sorted
            isTeamsSorted = true;
        }

        // Method to print the standing of the teams
        void printStanding()
        {
            // First, sort the teams
            sortTeams();

            // Print the standing of each team
            for (int i = 0; i < team.size(); i++)
            {
                team[i].print();
            }
        }
};

int main() 
{
    clearScreen();
    gotoxy(20, 10);
	cout << "<< Standify - Premier League's Standing >>";
	gotoxy(40, 15);
	cout << "Press any key to continue";
	_getch();
    get_file();
	_getch();

    return 0;
}
