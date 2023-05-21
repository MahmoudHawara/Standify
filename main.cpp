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

int lastRound = 0, yp;
map<string, int>nameToId;

int main();
void menu();
void openFileDialogue();
bool headToHead(int , int);         // mid man function to link between Team class and League class

// change console's dimensions
void resizeConsole()
{
    HWND console = GetConsoleWindow();
    MoveWindow(console, 100, 0, 1090, 800, TRUE);
}

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
        int pendingMatches;

        // Constructor to initialize the object
        Team(string name, int id, int mp = 0, int p = 0, int w = 0, int l = 0, int d = 0, int gf = 0, int ga = 0, bool v = 0, int pm = 0)
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
            this->pendingMatches = pm;
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
            this->pendingMatches = 0;
        }

        // Method to print the details of the team
        void print(int y, int c)
        {   

            gotoxy(5, y);
            cout << c;
            gotoxy(15, y);
            cout << this->Name;
            gotoxy(38, y);
            cout << this->matchesPlayed;
            gotoxy(53, y);
            cout << this->win;
            gotoxy(62, y);
            cout << this->lose;
            gotoxy(72, y);
            cout << this->draw;
            gotoxy(84, y);
            cout << this->goalsFor;
            gotoxy(100, y);
            cout << this->goalsAgainst;
            gotoxy(120, y);
            cout << max(this->goalsAgainst, this->goalsFor) - min(this->goalsAgainst, this->goalsFor);
            gotoxy(136, y);
            cout << this->points;

        }

        // This is a boolean operator that compares two instances of the Team class
        bool operator<(const Team& other)
        {
            // If the points of this team are not equal to the points of the other team
            if (this->points != other.points)
                // Return true if this team has more points than the other team, false otherwise
                return this->points > other.points;
            
            // If the difference between goals for and goals against for this team is not equal to the difference
            // between goals for and goals against for the other team
            if (this->goalsFor - this->goalsAgainst != other.goalsFor - other.goalsAgainst)
                // Return true if the difference is greater for this team, false otherwise
                return this->goalsFor - this->goalsAgainst > other.goalsFor - other.goalsAgainst;
            
            // If the number of wins for this team is not equal to the number of wins for the other team
            if (this->win != other.win)
                // Return true if this team has more wins than the other team, false otherwise
                return this->win > other.win;
            
            // If the number of goals scored by this team is not equal to the number of goals scored by the other team
            if (this->goalsFor != other.goalsFor)
                // Return true if this team has scored more goals than the other team, false otherwise
                return this->goalsFor > other.goalsFor;
            
            // If none of the above conditions are met, call the headToHead function
            // to determine the order of the teams based on their head-to-head record
            return headToHead(this->Id, other.Id);
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
                else if(match.winner == 'A')
                {
                    team[homeID].lose++;
                    team[match.awayTeamId].points += 3;
                    team[match.awayTeamId].win++;
                }
                else{
                    team[homeID].pendingMatches++;
                    team[match.awayTeamId].pendingMatches++;
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

        // Method to get the first match played in a given round for each team
        int getFirst(int homeID, int roundNum) 
        {
            int cnt = (int)teamSchedule[homeID].size();
            int l = 0, r = cnt - 1;

            while(l < r) 
            {
                int match_idx = (l + r) / 2;
                auto match = teamSchedule[homeID][match_idx];

                if(match.round < roundNum) l = match_idx + 1;
                else r = match_idx; 
            }

            return l;
        }

        // Method to get the matches played for a given round     
        void DFS_Round(int homeID, int roundNum, int endRound) 
        {
            if(team[homeID].vis) return;
            team[homeID].vis = 1;
            
            int match_idx = getFirst(homeID, roundNum);

            for(; match_idx < teamSchedule[homeID].size(); ++match_idx)
            {
                auto match = teamSchedule[homeID][match_idx];
                
                if(match.round > endRound) return;

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
                else if(match.winner == 'A')
                {
                    team[homeID].lose++;
                    team[match.awayTeamId].points += 3;
                    team[match.awayTeamId].win++;
                }
                else{
                    team[homeID].pendingMatches++;
                    team[match.awayTeamId].pendingMatches++;
                }

                DFS_Round(match.awayTeamId, roundNum, endRound);
            }
        }
        void DFS_Round(int roundNum, int endRound) 
        {
            resetTeams();
            for(int homeID = 0; homeID < team.size(); ++homeID)  
            {   
                DFS_Round(homeID, roundNum, endRound); 
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
                else if(match.winner == 'A')
                {
                    team[homeID].lose++;
                    team[match.awayTeamId].points += 3;
                    team[match.awayTeamId].win++;
                }
                else
                {
                    team[homeID].pendingMatches++;
                    team[match.awayTeamId].pendingMatches++;
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
        bool printStanding(int y)
        {

            bool ret = 0;
            teamToPrint = team;

            // First, sort the teams
            sortTeams(teamToPrint);

            // Print the standing of each team
            for (int i = 0; i < teamToPrint.size(); i++)
            {
                teamToPrint[i].print(y, i + 1);
                if(teamToPrint[i].pendingMatches) ret = 1;
                y++;
            }
            yp = y;
            return ret;
        }

        // A function that determines head-to-head ranking between two teams.
        bool headToHead(int teamId1, int teamId2)
        {
            // Initialize variables to keep track of wins and goal differences for both teams.
            int team1Wins = 0, team1DiffGoals = 0, team2Wins = 0, team2DiffGoals = 0;

            // Loop through the matches of the first team against the second team.
            for (int match = 0; match < teamSchedule[teamId1].size(); match++)
            {
                // If the away team is not the second team, skip this iteration.
                if (teamSchedule[teamId1][match].awayTeamId != teamId2) continue;
                
                // Increment the win count for the corresponding team based on who won the match.
                teamSchedule[teamId1][match].winner == 'H' ? team1Wins++ : team2Wins++;
                
                // Calculate the difference in goals scored and conceded for both teams.
                team1DiffGoals += teamSchedule[teamId1][match].goalsForHome - teamSchedule[teamId1][match].goalsForAway;
                team2DiffGoals += teamSchedule[teamId1][match].goalsForAway - teamSchedule[teamId1][match].goalsForHome;
            }

            // Loop through the matches of the second team against the first team.
            for (int match = 0; match < teamSchedule[teamId2].size(); match++)
            {
                // If the away team is not the first team, skip this iteration.
                if (teamSchedule[teamId2][match].awayTeamId != teamId1) continue;
                
                // Increment the win count for the corresponding team based on who won the match.
                teamSchedule[teamId2][match].winner == 'H' ? team2Wins++ : team1Wins++;
                
                // Calculate the difference in goals scored and conceded for both teams.
                team2DiffGoals += teamSchedule[teamId2][match].goalsForHome - teamSchedule[teamId2][match].goalsForAway;
                team1DiffGoals += teamSchedule[teamId2][match].goalsForAway - teamSchedule[teamId2][match].goalsForHome;
            }

            // If the number of wins is different for the two teams, return the one with more wins.
            if (team1Wins != team2Wins) return team1Wins > team2Wins;

            // Otherwise, return the team with a better goal difference.
            return team1DiffGoals > team2DiffGoals;
        }

        // This function resets the team schedule by clearing all game assignments for each team
        void reset()
        {
            teamSchedule.clear();
        }
};

League league_rounds;
League league_date;

// this function as a mid man between the Team class and League class
bool headToHead(int teamId1 , int teamId2)
{
    return league_rounds.headToHead(teamId1, teamId2);
}

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
long long isInt(string x) 
{
    for(int i = 0; i < x.length(); ++i) 
    {
        if(!isdigit(x[i])) return -1;
    }
    return strToInt(x);
}

// check if the input is string
bool isString(string x) 
{
    for(int i = 0; i < x.length(); ++i) 
    {
        if(isdigit(x[i])) return 0;
    }
    return 1;
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
string isGoodCSVFile(string filePath)
{   
    map<int, bool> rounds;

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
        int i = 0, round, goalsForHome, goalsForAway, c = 0;
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
                    round = isInt(field);
                    if(round == -1) 
                    {
                        return "The round number should be an integer";
                    }
                    
                    lastRound = max(lastRound, round);
                    rounds[round] = 1;
                    break;

                case 1: 
                    if(!isValidDate(field)) 
                    {
                        return "The date should follow the format (DD/MM/YYYY)";
                    }
                    break;

                case 2: 
                case 3: 
                    if(!isString(field)) 
                    {
                        return "The team name should be a string";
                    }
                    break;

                case 4: 
                    if(field[0] == '-') c++;
                    else {
                        goalsForHome = isInt(field);
                        if(goalsForHome == -1) 
                        {
                            return "Goals count should be an integer";
                        }
                    }
                    break;

                case 5: 
                    if(field[0] == '-') c++;
                    else {
                        goalsForAway = isInt(field);
                        if(goalsForAway == -1) 
                        {
                            return "Goals count should be an integer";
                        }
                    }
                    break;

                case 6:
                    winner = field[0];
                    if(winner == '-') c++;

                    if(c == 1 || c == 2) 
                    {
                        return "Matches that haven't yet taken place cannot contain a goals count or a result"; 
                    }

                    winner = toupper(winner);
                    if(winner != 'H' && winner != 'A' && winner != 'D' && winner != '-') 
                    {
                        return "The winner should be one of (H, A, D, -)";
                    }
                    if((goalsForHome > goalsForAway && winner != 'H') || (goalsForAway > goalsForHome && winner != 'A') ||
                    (goalsForAway == goalsForHome && winner != 'D' && winner != '-'))
                    {
                        return "The winner should match the goals count";
                    }

                    break;

                default:
                    return "Invalid input file";
            }
            i++;
        }
    }

    if(rounds.empty()) 
    {
        return "Invalid input file";
    }

    for(int i = 1; i <= lastRound; ++i)
    {
        if(rounds[i] == 0) 
        {
            return "Some rounds result are not found";
        }
    }

    // Close the input file
    inputFile.close();

    return "";
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
                    if(winner != '-')
                    {
                       winner = toupper(winner); 
                    } 
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
                    string msg = isGoodCSVFile(string(szFile));
                    // check if it is good CSV file or not
                    if (msg == "")
                    {
                        // The File is Good so take it and go ahead
                        lastRound = 0;
                        implementTheLeagueFromFile(string(szFile));
                        menu();
                        return;
                    }
                    else 
                    {
                        // the file is not good, so tell the user and make him upload another one
                        gotoxy(20, 14); 
                        cout << msg << "                                                          ";
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

void showStandings(string h, string x, string rEnd, bool r) 
{
    clearScreen();

    gotoxy(10, 3);
    cout << "<< Standify - Premier League's Standing >>";
    gotoxy(5, 6);
    if(rEnd == "") cout << "<< " << h << x << " >>";
    else cout << "<< " << h << x << "-" << rEnd << " >>";

    gotoxy(5, 9);
    cout << "#";
    gotoxy(15, 9);
    cout << "Team Name";
    gotoxy(32, 9);
    cout << "Matches Played";
    gotoxy(52, 9);
    cout << "Won";
    gotoxy(61, 9);
    cout << "Lost";
    gotoxy(71, 9);
    cout << "Draw";
    gotoxy(81, 9);
    cout << "Goals For";
    gotoxy(95, 9);
    cout << "Goals Against";
    gotoxy(116, 9);
    cout << "Goals Diff";
    gotoxy(134, 9);
    cout << "Points";

    bool ok = 0;
    if(r) {
        ok = league_rounds.printStanding(11);
    }
    else {
        ok = league_date.printStanding(11);
    }

    if(ok)
    {
        gotoxy(58, yp + 3);
        cout << "There are unplayed matches";
        yp += 3;
    }
    gotoxy(60, yp + 3);
    cout << "Press any key to back";
    
    _getch();
    menu();
}

void menu() 
{   

    clearScreen();

    gotoxy(10, 5);
    cout << "<< Standify - Premier League's Standing >>";
    gotoxy(20, 8);
    cout << "1 - The League Full Standing";
    gotoxy(20, 9);
    cout << "2 - The League Standing for a given Round";
    gotoxy(20, 10);
    cout << "3 - The League Standing for a given interval of rounds";
    gotoxy(20, 11);
    cout << "4 - The League Standing till a given Round";
    gotoxy(20, 12);
    cout << "5 - The League Standing till a given Date";
    gotoxy(20, 13);
    cout << "6 - Back";
    gotoxy(20, 14);
    cout << "7 - Exit";
    
    string choice;
    gotoxy(25, 17);
    cout << "Enter your choice: ";
    cin >> choice;
    
    int c = isInt(choice);
    if(c < 1 || c > 6) menu();
    
    if(c == 1) {
        league_rounds.DFS_Rounds(lastRound);
        showStandings("Full standing for league", "", "", 0);
    }
    else if(c < 5) {
        string round;

        gotoxy(28, 19);
        if(c != 3) cout << "Enter a round number: ";
        else cout << "Enter start round number: ";
        cin >> round;

        int r = isInt(round);
        if(r < 1 || r > lastRound) {
            gotoxy(31, 22);
            cout << "Invalid round number, the number of rounds equals " << lastRound;
            _getch();
            menu();
        }

        if(c == 2) {        
            // get the standing for the round 
            league_rounds.DFS_Round(r, r);
            // then show the standings
            showStandings("Standing for the round ", round, "", 0);
        }
        else if(c == 3)
        {
            string round2;

            gotoxy(28, 20);
            cout << "Enter end round number: ";
            cin >> round2;

            int r2 = isInt(round2);
            if(r2 < r || r2 > lastRound) {
                gotoxy(31, 23);
                cout << "The end round number must be between the given start " << r << " and " << lastRound;
                _getch();
                menu();
            }
            
            // get the standing for the round 
            league_rounds.DFS_Round(r, r2);
            // then show the standings
            showStandings("Standing for rounds interval ", round, round2, 0);
        }
        else {              
            // get the standing till the round 
            league_rounds.DFS_Rounds(r);   
            // then show the standings
            showStandings("Standing till round ", round, "", 0);
        }
    }
    else if(c == 5) {
        string date;
        int day, month, year = 0, f = 1;

        gotoxy(28, 19);
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
            showStandings("Standing till the date ", date,"", 1);
        }
        else {
            gotoxy(31, 22);
            cout << "Invalid date - Follow the pattern and try again";
            _getch();
            menu();
        }
    }
    else if(c == 6) {
        openFileDialogue();
    }
    else {
        main();
    }
}

int main() 
{   
    resizeConsole();
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
