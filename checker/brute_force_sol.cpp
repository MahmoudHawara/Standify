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

bool headToHead(int , int );

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

vector<Team>team, toPrint;

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

vector<Match> matches;

// The function takes two team IDs as input and returns a boolean value
// indicating whether the first team has won more matches or has a
// better goal difference than the second team in their head-to-head matches.
bool headToHead(int teamId1, int teamId2) 
{
    // Initialize variables to count wins and goal differences.
    int team1Wins = 0, team1DiffGoals = 0, team2Wins = 0, team2DiffGoals = 0;
    
    // Loop through all matches.
    for (int match = 0; match < matches.size(); match++)
    {
        // Check if the current match is between the two teams.
        if (matches[match].homeTeamId == teamId1 && matches[match].awayTeamId == teamId2) 
        {
            // Update win count and goal differences for each team.
            matches[match].winner == 'H' ? team1Wins++ : team2Wins++;
            team1DiffGoals += matches[match].goalsForHome - matches[match].goalsForAway;
            team2DiffGoals += matches[match].goalsForAway - matches[match].goalsForHome;
        }
        else if (matches[match].homeTeamId == teamId2 && matches[match].awayTeamId == teamId1) 
        {
            // Update win count and goal differences for each team.
            matches[match].winner == 'H' ? team2Wins++ : team1Wins++;
            team2DiffGoals += matches[match].goalsForHome - matches[match].goalsForAway;
            team1DiffGoals += matches[match].goalsForAway - matches[match].goalsForHome;
        }
    }
    // If one team has won more matches, return true. Otherwise, compare
    // the goal differences and return true if the first team has a higher
    // goal difference than the second team.
    if (team1Wins != team2Wins)return team1Wins > team2Wins;
    else return team1DiffGoals > team2DiffGoals;
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

// This function creates a new team with the given name and adds it to the league
void createNewTeam(string teamName)
{
    // First, we get the current number of teams, which will be the new team's ID
    int teamId = (int)team.size();
    
    // Next, we add a mapping from the team name to the team ID in the nameToId map
    nameToId[teamName] = teamId;
    
    // Then, we create a new Team object with the given name and ID, and add it to the team vector
    team.push_back(Team(teamName, teamId));
}

// This function reads league match data from a file specified by the filePath parameter
void ReadFile(string filePath)
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

        matches.push_back(match);
    }

    // Close the input file
    inputFile.close();
}

void solve(int roundNum) 
{

    for(int i = 0; i < team.size(); ++i) 
    {
        team[i].reset();
    }

    for(auto match: matches) 
    {   
        if(match.round > roundNum) continue; 

        team[match.homeTeamId].matchesPlayed++;
        team[match.homeTeamId].goalsFor += match.goalsForHome;
        team[match.homeTeamId].goalsAgainst += match.goalsForAway;
        
        team[match.awayTeamId].matchesPlayed++;
        team[match.awayTeamId].goalsFor += match.goalsForAway;
        team[match.awayTeamId].goalsAgainst += match.goalsForHome;

        if(match.winner == 'D') 
        {
            team[match.homeTeamId].points++;
            team[match.awayTeamId].points++;
            team[match.homeTeamId].draw++;
            team[match.awayTeamId].draw++;
        }
        else if(match.winner == 'H') 
        {
            team[match.homeTeamId].points += 3;
            team[match.homeTeamId].win++;
            team[match.awayTeamId].lose++;
        }
        else 
        {
            team[match.homeTeamId].lose++;
            team[match.awayTeamId].points += 3;
            team[match.awayTeamId].win++;
        }
    }

    toPrint = team;
    sort(toPrint.begin(), toPrint.end());
}

int main(int argc, char* argv[]) 
{
    // Check if the number of arguments passed is not equal to 2
    if (argc != 2) 
    { 
        cerr << "Usage: program_name.exe <integer argument>\n"; // Output error message to standard error stream
        return 1;                                         
    }

    string filePath = argv[1]; // Convert the second argument to an integer and store in variable

    // cout << "Selected Path: " << filePath << '\n';   // Output received integer argument

    ReadFile(filePath);

    ofstream out(".\\checker\\brute_force_solution.csv");

    for(int r = 1; r <= lastRound; ++r) 
    {
        solve(r);

        for(int i = 0; i < toPrint.size(); ++i) 
        {
            out << toPrint[i].Name << ',' << toPrint[i].matchesPlayed << ',' << toPrint[i].win << ',' << toPrint[i].lose
            << ',' << toPrint[i].draw << ',' << toPrint[i].goalsFor << ',' << toPrint[i].goalsAgainst << ',' << toPrint[i].points << '\n';
        }
    }

    out.close();

    return 0;
}
