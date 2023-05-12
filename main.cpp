#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

string path; // the path of the Input File

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
    // Initialize variables for file selection
    OPENFILENAME ofn;           // common dialog box structure
    char szFile[260];           // buffer for file name
    HWND hwnd = NULL;           // owner window

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

    cout << "**************** press 1 to browse ****************\n\n\n";

    while (true)
    {
        // Check if the '1' key is pressed
        if (_getch() == '1') 
        {
            // Display the Open dialog box.
            if (GetOpenFileName(&ofn)==TRUE) 
            {
                // Do something with the selected file
                cout << "\n**********************************\n";
                cout << "Selected file: " << szFile << endl;
                cout << "**********************************\n\n";
                if (isCSVFile(szFile))
                {
                    cout << "\n\t\t\t################# Your File is CSV File #################\n\n";
                    return;
                }
            }
        }
        else cout << "To browse, please press 1\n";
    }
}

int main() 
{
    printf("\t\t\t#######################################################\n");
    printf("\t\t\t\t\tChoose a CSV File to Upload\n");
    printf("\t\t\t#######################################################\n\n\n");

    get_file();
    cout << path << '\n';
    
    char c = getchar();
    return 0;
}
