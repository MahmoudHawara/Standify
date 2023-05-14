#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <windows.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

using namespace std;

int lastRound = 0;
map<string, int>nameToId;

int main();
void menu();
void openFileDialogue();

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

// This class represents a team in the league
class Team
{
    public: 
        string Name;                // The name of the team
        int Id;                     // The id of the team
        int matchesPlayed;          // Number of matches played by the team
        int points;                 // Number of points earned by the team
        int win;                    // Number of matches won by the team
        int lose;                   // Number of matches lost by the team
        int draw;                   // Number of matches drawn by the team
        int goalsFor;               // Number of goals scored by the team
        int goalsAgainst;           // Number of goals conceded by the team
        bool vis;                   // check if the team visited or not in dfs

        // Constructor to initialize the object
        Team(string name, int id, int mp = 0, int p = 0, int w = 0, int l = 0, int d = 0, int gf = 0, int ga = 0, bool v = 0)
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
            this->vis            = v;
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
            this->vis            = 0;
        }

        // Method to print the details of the team
        void print(int y)
        {   
            gotoxy(5, y);
            cout << this->Name;
            gotoxy(28, y);
            cout << this->matchesPlayed;
            gotoxy(43, y);
            cout << this->win;
            gotoxy(52, y);
            cout << this->lose;
            gotoxy(63, y);
            cout << this->draw;
            gotoxy(75, y);
            cout << this->goalsFor;
            gotoxy(91, y);
            cout << this->goalsAgainst;
            gotoxy(107, y);
            cout << this->points;
        }

        // Overloaded operator < to compare two teams based on their points
        bool operator<(const Team& other)
        {
            return this->points > other.points;
        }
};

vector<Team>team, teamToPrint;

// This class represents a match between two teams
class Match
{
    public:
        int homeTeamId;         // The id of the home team
        int awayTeamId;         // The id of the away team
        int round;              // The round number of the match
        int day;                // The day of the match
        int month;              // The month of the match
        int year;               // The year of the match
        int goalsForHome;       // The number of goals scored by the home team
        int goalsForAway;       // The number of goals scored by the away team
        char winner;            // d = Draw, h = Home, a = Away

        // Constructor to initialize the object
        Match(int hId, int aId, int r, int d, int m, int y, int gh, int ga, char w)
        {
            this->homeTeamId    = hId;
            this->awayTeamId    = aId;
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

        void print()
        {
            cout << this->round << ", ";
            cout << team[this->homeTeamId].Name << ", ";
            cout << team[this->awayTeamId].Name << ", ";
            cout << this->goalsForHome << ", ";
            cout << this->goalsForAway << ", ";
            cout << this->winner << '\n';
        }
};

// This class represents a league with teams and their matches schedule
class League
{
    private:
        vector<vector<Match>> teamSchedule;     // adjacency list of the graph
    
    public: 
        // Constructor to initialize the object
        League(){}

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
        // match  - Match object representing the match
        void addMatch(int homeId, int awayId, Match match)
        {
            // Check if both teams exist in the schedule
            if (max(homeId, awayId) >= (int)team.size())
            {
                cerr << "Raise Error: You Forget to push a Team in The team Vector\n";
                assert(0);
            }

            // Check if both teams exist in the teamSchedule
            if (max(homeId, awayId) >= (int)teamSchedule.size())
            {
                cerr << "Raise Error: You Forget to push a Team in The teamSchedule Vector\n";
                assert(0);
            }

            // Add the match to the home team's schedule
            this->teamSchedule[homeId].push_back(match);

        }

        // Method to add a new team to the team schedule.
        void addTeam()
        {
            // The push_back() function adds an empty vector to the end of the teamSchedule vector.
            teamSchedule.push_back({});
        }

        // Method to perform DFS on the rounds
        void DFS_Rounds(int homeID, int roundNum)
        {
            if (team[homeID].vis)return;

            team[homeID].vis = 1;
            
            for(auto match: teamSchedule[homeID]) 
            {
                if(match.round > roundNum) return;


                team[homeID].matchesPlayed++;
                team[homeID].goalsFor += match.goalsForHome;
                team[homeID].goalsAgainst += match.goalsForAway;
                
                team[match.awayTeamId].matchesPlayed++;
                team[match.awayTeamId].goalsFor += match.goalsForAway;
                team[match.awayTeamId].goalsAgainst += match.goalsForHome;

                if(match.winner == 'D') 
                {
                    team[homeID].points++;
                    team[match.awayTeamId].points++;
                    team[homeID].draw++;
                    team[match.awayTeamId].draw++;
                }
                else if(match.winner == 'H') 
                {
                    team[homeID].points += 3;
                    team[homeID].win++;
                    team[match.awayTeamId].lose++;
                }
                else 
                {
                    team[homeID].lose++;
                    team[match.awayTeamId].points += 3;
                    team[match.awayTeamId].win++;
                }

                DFS_Rounds(match.awayTeamId, roundNum);
            }
        }
        void DFS_Rounds(int roundNum)
        {
            resetTeams();
            for (int homeID = 0; homeID < team.size(); homeID++)
            {
                DFS_Rounds(homeID, roundNum);
            }
        }

        bool date_compare(int day1, int day2, int year1, int year2, int mon1, int mon2)
        {
            if(year1 > year2) return 1;
            if(year1 == year2 && mon1 > mon2) return 1;
            if(year1 == year2 && mon1 == mon2 && day1 > day2) return 1;
            return 0;
        }

        // Method to perform DFS on the dates
        void DFS_Date(int homeID, int day, int year, int month)
        {
            if(team[homeID].vis) return;
            
            team[homeID].vis = 1;

            for(auto match: this->teamSchedule[homeID])
            {
                if(date_compare(match.day, day, match.year, year, match.month, month)) return;

                team[homeID].matchesPlayed++;
                team[homeID].goalsFor += match.goalsForHome;
                team[homeID].goalsAgainst += match.goalsForAway;

                team[match.awayTeamId].matchesPlayed++;
                team[match.awayTeamId].goalsAgainst += match.goalsForHome;
                team[match.awayTeamId].goalsFor += match.goalsForAway;

                if(match.winner == 'D') 
                {
                    team[homeID].points++;
                    team[match.awayTeamId].points++;
                    team[homeID].draw++;
                    team[match.awayTeamId].draw++;
                }
                else if(match.winner == 'H') 
                {
                    team[homeID].points += 3;
                    team[homeID].win++;
                    team[match.awayTeamId].lose++;
                }
                else 
                {
                    team[homeID].lose++;
                    team[match.awayTeamId].points += 3;
                    team[match.awayTeamId].win++;
                }
                
                DFS_Date(match.awayTeamId, day, year, month);
            }
        }
        void DFS_Date(int day, int year, int month)
        {
            resetTeams();
            for (int homeID = 0; homeID < team.size(); homeID++)
            {
               DFS_Date(homeID, day, year, month);
            }
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
        void sortTeams(vector<Team>& team)
        {
            // Sort the teams
            sort(team.begin(), team.end());
        }

        void print()
        {
            for (int i = 0; i < teamSchedule.size(); i++)
            {
                for (int j = 0; j < teamSchedule[i].size(); j++)
                {
                    cout << team[i].Name << ' ' << team[this->teamSchedule[i][j].awayTeamId].Name << ' ' << this->teamSchedule[i][j].round << '\n';
                }
                cout << "\n\n";
            }
        }

        // Method to print the standing of the teams
        void printStanding(int y)
        {

            teamToPrint = team;

            // First, sort the teams
            sortTeams(teamToPrint);

            // Print the standing of each team
            for (int i = 0; i < teamToPrint.size(); i++)
            {
                teamToPrint[i].print(y);
                y++;
            }
        }

        // This function resets the team schedule by clearing all game assignments for each team
        void reset()
        {
            teamSchedule.clear();
        }
};

League league_rounds;
League league_date;

// This function converts a string representation of a number to a long long integer
long long strToInt(string number)
{
    // We start with a value of 0
    long long ret = 0;
    
    // For each character in the string, we multiply our current value by 10 and add the digit
    for (int i = 0; number[i]; i++)
    {
        ret *= 10;
        ret += (number[i] - '0');
    }
    
    // Once we've processed all the digits, we return the final value
    return ret;
}

// check if the input is integer
long long isInt(string x) {
    for(int i = 0; i < x.length(); ++i) {
        if(!isdigit(x[i])) return 0;
    }
    return strToInt(x);
}

// check if the date follows the pattern
bool isValidDate(string date) {
    regex pattern("\\d{2}[/||-]\\d{2}[/||-]\\d{4}");
    return regex_match(date, pattern);
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
    if (dotpos != len && strcmp(&filename[dotpos], ".csv") == 0)return true;
    else return false;
}

// check if the CSV File is good or not
bool isGoodCSVFile(string filePath)
{
    // TODO: Implementation pending  =>  Sherif
    return true;
}

// This function creates a new team with the given name and adds it to the league
void createNewTeam(string teamName)
{
    // First, we get the current number of teams, which will be the new team's ID
    int teamId = (int)team.size();
    
    // Next, we add a mapping from the team name to the team ID in the nameToId map
    nameToId[teamName] = teamId;
    
    // Then, we create a new Team object with the given name and ID, and add it to the team vector
    team.push_back(Team(teamName, teamId));
    
    // Finally, we inform the league that a new team has been added
    league_rounds.addTeam();
}

// This function reads league match data from a file specified by the filePath parameter
void implementTheLeagueFromFile(string filePath)
{
    // Open the input file
    ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        cerr << "Error: Unable to open file!\n";
        assert(0); // Quit the program if unable to open the file
    }

    string line;
    // Read the first line of the file (header) and discard it
    getline(inputFile, line);

    // Read the rest of the lines in the file and extract match data
    while (getline(inputFile, line)) 
    {
        int i = 0, homeTeamId, awayTeamId, round, day, month, year, goalsForHome, goalsForAway;
        char winner, delimiter = '/';
        string field;
        istringstream ss(line), sss;

        // Extract each field of the current line using comma as the delimiter
        while (getline(ss, field, ',')) 
        {
            // Use a switch statement to determine which field we are currently processing
            switch (i)
            {
                case 0:
                    round = strToInt(field);
                    lastRound = max(lastRound, round);
                    break;

                case 1: 
                    sss = istringstream(field);
                    sss >> day >> delimiter >> month >> delimiter >> year;
                    break;

                case 2: 
                    // If the home team is not already in the map, create a new team and add it to the map
                    if (nameToId.find(field) == nameToId.end())
                    {
                        createNewTeam(field);
                    }
                    homeTeamId = nameToId[field];
                    break;

                case 3: 
                    // If the away team is not already in the map, create a new team and add it to the map
                    if (nameToId.find(field) == nameToId.end())
                    {
                        createNewTeam(field);
                    }
                    awayTeamId = nameToId[field];
                    break;

                case 4: 
                    // If the goals for home team field is '-', set it to '0'
                    if (field[0] == '-')field[0] = '0';
                    goalsForHome = strToInt(field);
                    break;

                case 5: 
                    // If the goals for away team field is '-', set it to '0'
                    if (field[0] == '-')field[0] = '0';
                    goalsForAway = strToInt(field);
                    break;

                case 6:
                    winner = field[0];
                    break;
            }
            i++;
        }

        // Create a Match object using the extracted match data
        Match match(homeTeamId, awayTeamId, round, day, month, year,
                    goalsForHome, goalsForAway, winner);

        // Print the match details (for testing purposes)
        // match.print();

        // Add the match to the league_rounds and league_date objects
        league_rounds.addMatch(homeTeamId, awayTeamId, match);
    }

    // Set league_date to be a copy of league_rounds
    league_date = league_rounds;
    league_rounds.sortMatchesBasedOnRounds();       // sort league_rounds based on match round
    league_date.sortMatchesBasedOnDate();           // // sort league_rounds based on match date

    // Print and test the sorting functions of the league_rounds and league_date objects
    // cout << "\n\n################################################\n\n";
    // league_rounds.print();
    // cout << "\n\n################################################\n\n";
    // league_date.print();
    // cout << "\n\n################################################\n\n";
    // _getch();

    // Close the input file
    inputFile.close();
}

// This function resets all league data by clearing various data structures and resetting league-wide stats
void resetAll()
{
    // Clear the team vector, which contains information about each team in the league
    team.clear();
    
    // Reset the league-rounds data structure, which tracks various stats for the league over time
    league_rounds.reset();
    
    // Reset the league-date data structure, which may track different stats than league-rounds based on dates
    league_date.reset();
    
    // Clear the nameToId map, which maps team names to their corresponding IDs
    nameToId.clear();
}

void openFileDialogue()
{   

    resetAll();

    clearScreen();
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
                // check if it is CSV File or not
                if (isCSVFile(szFile)) 
                {
                    // check if it is good CSV file or not
                    if (isGoodCSVFile(string(szFile)))
                    {
                        // The File is Good so take it and go ahead
                        implementTheLeagueFromFile(string(szFile));
                        menu();
                        return;
                    }
                    else 
                    {
                        // the file is not good, so tell the user and make him upload another one
                        gotoxy(20, 14); 
                        cout << "Invalid data - Check the file and try again";
                        gotoxy(12, 16); 
                        cout << "Press 'u' to browse: ";
                    }
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

void showStandings(string h, string x, bool r) 
{
    clearScreen();

    gotoxy(10, 3);
    cout << "<< Standify - Premier League's Standing >>";
    gotoxy(5, 6);
    cout << "<< " << h << x << " >>";

    gotoxy(5, 9);
    cout << "Team Name";
    gotoxy(22, 9);
    cout << "Matches Played";
    gotoxy(42, 9);
    cout << "Won";
    gotoxy(51, 9);
    cout << "Lost";
    gotoxy(61, 9);
    cout << "Draw";
    gotoxy(71, 9);
    cout << "Goals For";
    gotoxy(85, 9);
    cout << "Goals Against";
    gotoxy(104, 9);
    cout << "Points";

    if(r) {
        league_rounds.printStanding(11);
    }
    else {
        league_date.printStanding(11);
    }

    _getch();
    menu();
}

void menu() 
{   

    clearScreen();

    gotoxy(10, 5);
    cout << "<< Standify - Premier League's Standing >>";
    gotoxy(20, 8);
    cout << "1 - The league standings for a given round";
    gotoxy(20, 9);
    cout << "2 - The league standings till a given round";
    gotoxy(20, 10);
    cout << "3 - The league standings till a given date";
    gotoxy(20, 11);
    cout << "4 - Back";
    gotoxy(20, 12);
    cout << "5 - Exit";
    
    string choice;
    gotoxy(25, 15);
    cout << "Enter your choice: ";
    cin >> choice;
    
    int c = isInt(choice);
    if(c < 1 || c > 5) menu();
   
    if(c < 3) {
        string round;

        gotoxy(28, 17);
        cout << "Enter a round number: ";
        cin >> round;

        int r = isInt(round);
        if(r < 1 || r > lastRound) {
            gotoxy(31, 19);
            cout << "Invalid round number, the number of rounds equals " << lastRound;
            _getch();
            menu();
        }

        if(c == 1) {        
            // get the standing for the round 

            // then show the standings
            showStandings("Standing for the round ", round, 0);
        }
        else {              
            // get the standing till the round 
            league_rounds.DFS_Rounds(r);   
            // then show the standings
            showStandings("Standing till round ", round, 0);
        }
    }
    else if(c == 3) {
        string date;
        int day, month, year = 0, f = 1;

        gotoxy(28, 17);
        cout << "Enter a date (DD/MM/YYYY): ";
        cin >> date;
        
        if(isValidDate(date)) {
            day = (date[0] - '0' != 0) * 10 + (date[1] - '0'); 
            month = (date[3] - '0' != 0) * 10 + (date[4] - '0'); 
            for(int i = 9; i > 5; --i) {
                year += (date[i] - '0') * f;
                f *= 10;
            }        

            // get the standings till the date
            league_date.DFS_Date(0, day, year, month); 

            // then show the standings
            showStandings("Standing till the date ", date, 1);
        }
        else {
            gotoxy(31, 19);
            cout << "Invalid date - Follow the pattern and try again";
            _getch();
            menu();
        }
    }
    else if(c == 4) {
        openFileDialogue();
    }
    else {
        main();
    }
}

int main() 
{
    clearScreen();
    gotoxy(20, 10);
	cout << "<< Standify - Premier League's Standing >>";
	gotoxy(40, 15);
	cout << "Press any key to continue";
	_getch();
    openFileDialogue();
	_getch();

    return 0;
}
