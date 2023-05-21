#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include <cassert>
#include <unistd.h>

using namespace std;

int lastRound = 0;
int main();

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

void run_programs(string filePath) 
{
    // Compile the two programs
    system("g++ -o .\\brute_force_sol.exe .\\brute_force_sol.cpp"); 
    system("g++ -o .\\graph_sol.exe .\\graph_sol.cpp");
    
    // Construct the command with the integer argument
    string command  = ".\\brute_force_sol.exe ";
    string command2 = ".\\graph_sol.exe ";
    command  += filePath;
    command2 += filePath;
    command  += " &";
    command2 += " &";

    // Run the program in the background
    system(command.c_str());
    system(command2.c_str());
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

string openFileDialogue()
{   

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
                        return (string(szFile));  
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

    return "";
}

vector<vector<string>> parseCSV(string filename)
{
    ifstream file(filename);
    vector<vector<string>> result;
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        vector<string> row;
        string item;
        while (getline(ss, item, ','))
        {
            row.push_back(item);
        }
        result.push_back(row);
    }
    file.close();
    return result;
}

bool compareCSV(string file1, string file2)
{
    auto csv1 = parseCSV(file1);
    auto csv2 = parseCSV(file2);


    // Compare number of rows and columns
    if (csv1.size() != csv2.size() || (csv1.empty() == 0 && csv1[0].size() != csv2[0].size()))
    {
        return false;
    }

    // Compare data in each row and column
    for (int i = 0; i < csv1.size(); i++)
    {
        for (int j = 0; j < csv1[i].size(); j++)
        {
            if (csv1[i][j] != csv2[i][j])
            {
                return false;
            }
        }
    }

    return true;
}

void goToCompare(string file1, string file2)
{
    if (compareCSV(file1, file2))
    {
        gotoxy(55, 20);
        cout << "Accepted";
    }
    else
    {
        gotoxy(45, 20);
        cout << "Files are different";
    }
    _getch();
    main();
}

int main() 
{
    clearScreen();

    gotoxy(20, 10);
	cout << "<< Standify - Test Graph Solution >>";
	gotoxy(40, 15);
	cout << "Press any key to continue";
	_getch();

    string filePath = openFileDialogue();
    filePath = "\"" + filePath + "\"";

    // set the path of checker directory 
    chdir("");
    
    run_programs(filePath);

    goToCompare(".\\brute_force_solution.csv", ".\\graph_solution.csv");

    return 0;
}
