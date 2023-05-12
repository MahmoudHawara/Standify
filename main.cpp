#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

string path; // the path of the Input File

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
                    // to do 
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

int main() 
{
    system("cls"); 
    gotoxy(20, 10);
	cout << "<< Standify - Premier League's Standing >>";
	gotoxy(40, 15);
	cout << "Press any key to continue";
	_getch();
    get_file();
    return 0;
}
