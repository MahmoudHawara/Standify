#include <cstdlib>
#include <iostream>

using namespace std;

int main() 
{
    int round = 42;

    // Compile the program
    system("g++ -o graph_sol.exe graph_sol.cpp"); 

    // Construct the command with the integer argument
    string command = "graph_sol.exe ";
    command += to_string(round);
    command += " &";

    // Run the program in the background
    system(command.c_str());


}