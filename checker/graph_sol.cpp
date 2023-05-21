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

// mid man function to link between Team class and League class
bool headToHead(int , int);

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
            this->pendingMatches = 0;
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

        // Method to sort the matches based on rounds
        void sortMatchesBasedOnRounds()
        {
            for (int i = 0; i < this->teamSchedule.size(); i++)
            {
                sort(teamSchedule[i].begin(), teamSchedule[i].end());
            }
        }

        // Method to sort the teams based on their standing
        void sortTeams(vector<Team>& team)
        {
            // Sort the teams
            sort(team.begin(), team.end());
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

        league_rounds.addMatch(homeTeamId, awayTeamId, match);

    }

    // Close the input file
    inputFile.close();
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

    ReadFile(filePath);

    ofstream out("graph_solution.csv");

    league_rounds.sortMatchesBasedOnRounds();

    for(int r = 1; r <= lastRound; ++r) 
    {
        league_rounds.DFS_Rounds(r);

        teamToPrint = team;
        sort(teamToPrint.begin(), teamToPrint.end());

        for(int i = 0; i < teamToPrint.size(); ++i) 
        {
            out << teamToPrint[i].Name << ',' << teamToPrint[i].matchesPlayed << ',' << teamToPrint[i].win << ',' << teamToPrint[i].lose
            << ',' << teamToPrint[i].draw << ',' << teamToPrint[i].goalsFor << ',' << teamToPrint[i].goalsAgainst << ',' << teamToPrint[i].points << '\n';
        }
    }

    out.close();

    return 0;
}
