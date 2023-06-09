# Checker 

Are you spending too much time manually verifying program outputs for this specific problem? We understand the challenges involved, and that's why we developed this checker to automate the validation process and test all possible scenarios.

Our checker allows you to quickly and efficiently validate your program outputs, saving you valuable time and resources. It ensures that your outputs meet the requirements of this specific problem, allowing you to focus on what matters most - delivering high-quality results.

## Prerequisites

- An understanding of basic `C++` concepts, including command line arguments and file input/output operations.

- A C++ compiler installed on your computer, such as `g++`.

- The `data_set.csv` file provided as sample input CSV to validate your code against.

- A working solution for the problem being solved.

- Your new `graph_sol.cpp` solution file should be located in the same directory as both **''checker.cpp''** and **''brute_force_sol.cpp''**.

## TODO

1. Clone the repository onto your machine.

2. Compile and run `checker.cpp` by typing the following commands in your terminal:

    ```sh
    g++ checker.cpp -o checker.exe -lcomdlg32

    ./checker
    ```
3. Create a new **''graph_sol.cpp''** file that contains your solution for the problem and save it in the same directory as **''checker.cpp''** and **''brute_force_sol.cpp''**.

4. Modify your new **''graph_sol.cpp''** file by adding code that allows it to receive the path of the **CSV** file as an argument from `argv[1]` in C++.

5. The selected file path will be passed to your new **''graph_sol.cpp''** file as the first argument, which you can access using **''argv[1]''**. **Note:** Be sure to properly read and process this argument in your code!

6. Lastly, output your program's solution into a new **CSV** file named `graph_solution.csv` in the same directory as your **"graph_sol.cpp"** files.
  

## Inside the Black Box

 This module works by compiling and running both the user's program `graph_sol.cpp` and a `brute_force_sol.cpp` file, which contains an implementation of a **brute-force algorithm** capable of generating correct outputs for the given input data.

During execution, the checker passes the path to the input CSV file to both programs using **argv[1]** in C++, which then read the file and generate their respective outputs. The checker then compares the output produced by the user's program stored in `graph_solution.csv`against the output generated by the brute-force algorithm stored in `brute_force_solution.csv`.

If the two match, the program confirms that the user's solution is correct. However, if they don't match, the program raises an error message indicating that the user's solution is incorrect.

## Conclousion

In summary, our checker automates the tedious task of verifying program outputs, saving you time and resources. It compares your program output against a `brute-force algorithm` to ensure accuracy, making it a reliable tool for validating user-generated code. With this checker, you can focus on delivering high-quality results without spending too much time on manual verification.
