#include <iostream>
#include <chrono>
#include <thread>

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

// this function Displays the ASCII art of the Title
void writeTitle()
{
    cout << "\n\n\n";
    cout <<
    "         ****     ********   ********    *     *    ******      ******   *******  *     *\n"
    "       **    **      **      **    **    **    *    **    **      **     **        *   * \n"
    "      **             **      **    **    * *   *    **     **     **     **         * *  \n"
    "       ****          **      ********    *  *  *    **     **     **     *****       *   \n"
    "           **        **      **    **    *   * *    **     **     **     **          *   \n"
    "      **    **       **      **    **    *    **    **    **      **     **          *   \n"
    "        ****         **      **    **    *     *    ******      ******   **          *   \n";
    cout << "\n\n";
}

// this class to draw the progressbar
class ProgressBar {
public:
    ProgressBar(int total, int width = 50) : total_(total), width_(width) {}

    void update(int value, string message = "", string color = "") {
        // Calculate the percentage of completion
        int percentage = value * 100 / total_;

        // Calculate the number of completed and remaining symbols in the progress bar
        int completedSymbols = value * width_ / total_;
        int remainingSymbols = width_ - completedSymbols;

        // Construct the progress bar string with "#" characters
        string progressBar = "\t   [";
        for (int i = 0; i < completedSymbols; ++i) {
            progressBar += "#";
        }
        for (int i = 0; i < remainingSymbols; ++i) {
            progressBar += "-";
        }
        progressBar += "]";

        // Output the progress bar with optional message and color to the console
        cout << "\r" << progressBar << " " << percentage << "% ";
        cout.flush();

        if (!message.empty()) {
            if (!color.empty()) {
                cout << "\033[" << color << "m";
            }
            cout << message;
            if (!color.empty()) {
                cout << "\033[0m";
            }
        }
    }

private:
    int total_;
    int width_;
};

// this function to display a splash screen
void splashScreen()
{
    const int total = 100;
    ProgressBar progressBar(total);

    // Display the splash screen before starting the progress bar
    writeTitle();

    // Update the progress bar with different messages and colors
    for (int i = 0; i <= total/2; i += 10) {
        progressBar.update(i, "Loading", "32");
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    for (int i = total/2; i <= total; i += 10) {
        progressBar.update(i, "Please wait", "31");
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    // clear the screen
    clearScreen();
}

int main()
{
    splashScreen();   
    return 0;
}
