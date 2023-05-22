# Standify

A C++ program that generates league standings based on match results and can filter by date or round. Utilizing graph theory concepts, Standify provides a comprehensive overview of team performance. Perfect for data analysts and sports enthusiasts.

## Prerequestes

- A C++ compiler installed on your computer, such as `g++`.

- CSV File of input data formatted as `data_set.csv` provided as sample input, or it can be used in its current form.

## Usage

1. Clone the repository onto your machine.

2. Compile and run `main.cpp` by typing the following commands in your terminal:

    ```sh
    g++ main.cpp -o main.exe -lcomdlg32

    ./main
    ```

## Algorithms Used

- Depth First Search `DFS`
    used to traverse on The league matches in `DFS_round` and `DFS_date` Functions. 

- Binary Search `BS`
    used to get the first match that falls within the specified range in `getFirst()` Function.

`DFS_round`, `DFS_date` and `getFirst()` these Functions will be explained in the following Sections.

## Handling Input Data 

To start the program, the user must provide an input file in **CSV** format. The script then employs a function `isCSVFile()` to verify whether the file is indeed a **CSV** file. If this validation check fails, the program issues an error message requesting the user to input another file.

However, if the input file passes the **CSV** validation check, the script proceeds with further checks by utilizing a function `checkCSVFormat()` to ensure that the file conforms to the expected structure of the sample input file, `data_set.csv`. In case the file does not match the anticipated format, the program displays a specific error message, prompting the user to format the file as required.

In contrast, if the input file aligns with the anticipated format, the program proceeds to execute as intended.

***The Time Complexity for this Section: `O(m * lg(R))`, where m is the number of matches and n is the number of rounds.***

Upon starting the game, I will create a graph representation of the input data using two objects from the `League` class. The first object, `league_round`, will sort each team's matches based on the rounds, while the second object, `league_date`, will sort them based on their respective dates.

To build this graph representation, I'll use a **vector of vectors** called `teamSchedule`, which follows an `adjacency list` format. Each inner vector within the teamSchedule represents the matches of a particular team, where the index of the vector corresponds to the `homeId` of the team. Within these inner vectors, each element is an instance of the `match` class, containing the information for each individual match.

***The Time Complexity for this Section: `O(m * lg(n))`, where m is the number of matches and n is the number of teams***

## Code Explanation

After the input is taken and represented as an **adjacency list**, the program's menu will appear. This menu offers various options, but the most significant one is the ability to display standings up to a certain round, within a range of rounds, or until a specified date.

Suppose the user opts to view the standings **up to a particular Round** and inputs that round. In that case, the program will leverage the `DFS_round` function to traverse the matches of every team in the **teamSchedule** vector within the **League** class whose round is less than or equal to the given round. Afterward, the `team` vector will be sorted, and the `showStanding` function will be used to display the standings until the chosen round.

***The Time Complexity for this Section: `O(m + n * lg(n))`, where n is the number of teams and m is the number of matches.***

In case the user chooses to display rounds within a **specific range** and inputs the start and end round values, the program will make use of the `DFS_round` function. To begin with, this function will utilize the `getFirst()` function, which involves a `binary search` algorithm, to locate the first match that falls within the specified range. After that, the function will traverse only those matches whose rounds fall within the given range, similar to the approach utilized in the pre-section.

***The Time Complexity for this Section: `O((n + m) * lg(m))`, where m is The number of matches and n is the number of teams.***

Suppose the user opts to view the standings **up to a particular Date** and inputs that date. In that case, the program will leverage the `DFS_date` function to traverse the matches of every team in the **teamSchedule** vector within the **League** class whose round is less than or equal to the given date. Afterward, the `team` vector will be sorted, and the `showStanding` function will be used to display the standings until the chosen round.

***The Time Complexity for this Section: `O(m + n * lg(n))`, where n is the number of teams and m is the number of matches.***

## Complexity

In the preceding two sections, it was elucidated that the *time complexity is `O((n + m) * lg(m) * k)`* and the space complexity is *`O(m + n)`, where `m` is The number of matches and `n` is the number of teams and `k` is the number of quireies*.

## Demo 

https://github.com/MahmoudHawara/Standify/assets/66095584/10294c45-0756-4779-ab4e-82d74f975e44




