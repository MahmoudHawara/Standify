#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include <string>
#include <sstream>
#include <regex>

using namespace std;

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

void run_programs(string filePath) 
{
    // Compile the two programs
    system("g++ -o .\\checker\\brute_force_sol.exe .\\checker\\brute_force_sol.cpp"); 
    system("g++ -o .\\checker\\graph_sol.exe .\\checker\\graph_sol.cpp");
    
    // Construct the command with the integer argument
    string command  = ".\\checker\\brute_force_sol.exe ";
    string command2 = ".\\checker\\graph_sol.exe ";
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
bool isGoodCSVFile(string filePath)
{
    // TODO: Implementation pending  =>  Sherif
    return true;
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
                    // check if it is good CSV file or not
                    if (isGoodCSVFile(string(szFile)))
                    {
                        // The File is Good so take it and go ahead
                        return (string(szFile));  
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
    return result;
}

bool compareCSV(string file1, string file2)
{
    auto csv1 = parseCSV(file1);
    auto csv2 = parseCSV(file2);

    // Compare number of rows and columns
    if (csv1.size() != csv2.size() || csv1[0].size() != csv2[0].size())
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
    gotoxy(45, 20);
    if (compareCSV(file1, file2))
    {
        cout << "Accepted" << endl;
    }
    else
    {
        cout << "Files are different" << endl;
    }
    _getch();
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

    run_programs(filePath);

    goToCompare("brute_force_solution.csv", "graph_solution.csv");

    return 0;
}
