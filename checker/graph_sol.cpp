#include <cstdlib> 
#include <iostream> 

using namespace std;

int main(int argc, char* argv[]) 
{
    // Check if the number of arguments passed is not equal to 2
    if (argc != 2) 
    { 
        cerr << "Usage: program_name.exe <integer argument>\n"; // Output error message to standard error stream
        return 1;                                         
    }

    string filePath = argv[1]; // Convert the second argument to an integer and store in variable
    cout << "Selected Path: " << filePath << '\n';   // Output received integer argument

}
