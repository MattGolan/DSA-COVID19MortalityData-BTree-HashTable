#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <ctime>
#include "HashTable.h"
#include "BTree.h"

using namespace std;

/* Our team utilized concepts from "UFO COP3503C: Programming Fundamentals II" to develop functions for reading csv
     * files. Credit to Professor Fox for conceptual approach. */

// Reads all relevant rows in a file.
void ReadFile(ifstream& currentData, HashTable& currentHashTable,BTree& currentBTree);

// Calls functions to convert strings to usable data and adds data to hash table.
void AddToHashTable(string state, string gender, string ageRange, string group, string year, string month, string deaths, HashTable& currentHashTable);
void AddToBTree(string state, string gender, string ageRange, string group, string year, string month, string deaths, BTree& currentBTree);

// Helper functions to convert strings into usable data.
int StateCalculation(string& state);
int GenderCalculation(string& gender);
int AgeRangeCalculation(string& ageRange);
int TimeCalculation(int& timeValue, string& group, string& year, string& month);

// Print displays to prompt user input.
bool DisplayWelcome();
void DisplayStateOptions(string& state);
void DisplayTimeOptions(string& time);
void DisplayAgeOptions(string& age);
void DisplayGenderOptions(string& gender);

int main() {

    // Variable declarations.
    ifstream mortalityDataFile;
    HashTable currentHashTable;
    BTree currentBTree;
    mortalityDataFile.open("C:\\\\Users\\\\mttgl\\\\OneDrive\\\\Desktop\\\\Provisional_COVID-19_Deaths_by_Sex_and_Age.csv");

    // Checks if file was opened properly.
    if (mortalityDataFile.is_open())
    {
        // Removes header data.
        string headerInformation;
        getline(mortalityDataFile, headerInformation);

        // Calls function to read relevant data.
        ReadFile(mortalityDataFile, currentHashTable, currentBTree);
    }
    else
    {
        cout << "Error: The file was never opened!" << endl;
    }

    bool keepGoing = DisplayWelcome();
    while (keepGoing)
    {
        // Declarations
        string stateString;
        string timeString;
        string ageGroupString;
        string genderString;

        // Calls print functions.
        DisplayStateOptions(stateString);
        DisplayTimeOptions(timeString);
        DisplayAgeOptions(ageGroupString);
        DisplayGenderOptions(genderString);

        // *=== TEMP STAG ===* //
        int stagTemp = (stoi(stateString)*50*17*3)+(stoi(timeString)*17*3)+(stoi(ageGroupString)*3)+(stoi(genderString));
        int hashReturnValue = currentHashTable.GetValue(stoi(stateString), stoi(ageGroupString), stoi(genderString), stoi(timeString));
        int treeReturnValue = currentBTree.Search(currentBTree.GetRoot(), stagTemp);


        // Checks output and branches to category of response.
        if (hashReturnValue == -1 || treeReturnValue == -1)
        {
            cout << endl;
            cout << "Unfortunately, the subset of data you selected is unavailable." << endl;
            cout <<"It has been suppressed in accordance with NCHS confidentiality standards." << endl;
            cout << endl;
        }
        else if (hashReturnValue == -87 || treeReturnValue == -87)
        {
            cout << endl;
            cout << "Error: Not in dataset!" << endl;
            cout << endl;
        }
        else
        {
            cout << "Number of deaths (Hash Table): " << hashReturnValue << endl;
            cout << endl;
            cout << "Number of deaths (B-Tree): " << treeReturnValue << endl;
            cout << endl;
            cout << "Repeating each search 1 million times . . . " << endl;
            cout << endl;

            // *=== HASH SEARCH / TIME ===* //
            auto hashSearchStart = clock();
            for (unsigned int i = 0; i < 1000000; i++)
            {
                hashReturnValue = currentHashTable.GetValue(stoi(stateString), stoi(ageGroupString), stoi(genderString), stoi(timeString));
            }
            auto hashSearchEnd = clock();

            // *=== TREE SEARCH / TIME ===* //
            auto treeSearchStart = clock();
            for (unsigned int i = 0; i < 1000000; i++)
            {
                treeReturnValue = currentBTree.Search(currentBTree.GetRoot(), stagTemp);
            }
            auto treeSearchEnd = clock();
            //Prints comparison for 1000000 searches of each.
            cout << "For a million searches, The Hash Table was around " << double((treeSearchEnd-treeSearchStart)/(hashSearchEnd-hashSearchStart));
            cout << " Times faster than the B tree!"<< endl;
            cout << endl;
        }
        cout << "Would you like to continue searching? (Press 'n' to stop)" << endl;
        string userInput;
        cin >> userInput;
        if (userInput == "n")
        {
            keepGoing = false;
        }
    }

    cout << endl;
    cout << "Goodbye!" << endl;

    return 0;
}

// Function definitions

// Reads file
void ReadFile(ifstream& currentData, HashTable& currentHashTable, BTree& currentBTree)
{
    // Reads through every line in the csv file.
    string currentRow;
    while (getline(currentData, currentRow))
    {
        // Variable declarations.
        string data;
        string state;
        string gender;
        string ageRange;
        string group;
        string year;
        string month;
        string deaths;

        // Converts current row into istringstream.
        istringstream data_stream(currentRow);


        // Reads relevant data to variables and irrelevant data to disposable data variable.
        getline(data_stream, data, ',');
        getline(data_stream, data, ',');
        getline(data_stream, data, ',');
        getline(data_stream, group, ',');
        getline(data_stream, year, ',');
        getline(data_stream, month, ',');
        getline(data_stream, state, ',');
        getline(data_stream, gender, ',');
        getline(data_stream, ageRange, ',');
        getline(data_stream, deaths, ',');

        if (state != "United States" && state != "New York City" && !deaths.empty())
        {
            AddToHashTable(state, gender, ageRange, group, year, month, deaths, currentHashTable);


            AddToBTree(state, gender, ageRange, group, year, month, deaths, currentBTree);

        }
        else if (state != "United States" && state != "New York City")
        {
            AddToBTree(state, gender, ageRange, group, year, month, "-1", currentBTree);
            AddToHashTable(state, gender, ageRange, group, year, month, "-1", currentHashTable);
        }

    }
}

//Adds data to each structure.
void AddToBTree(string state, string gender, string ageRange, string group, string year, string month, string deaths, BTree& currentBTree){
    // Declares integer variables and calls calculation functions to assign values.
    int stateValue;
    stateValue = StateCalculation(state);
    int genderValue;
    genderValue = GenderCalculation(gender);
    int ageRangeValue;
    ageRangeValue = AgeRangeCalculation(ageRange);
    int timeValue;
    timeValue = TimeCalculation(timeValue, group, year, month);

    //Combine the variables together into a string to form a unique combination that will act as the identifier.
    int STAG = (stateValue*50*17*3)+(timeValue*17*3)+(ageRangeValue*3)+(genderValue);
    //The data to insert is the pair where the first element is the STAG identifier and the second element is the death statistics
    currentBTree.ModifyNode(currentBTree.InsertNode(currentBTree.GetRoot(), make_pair(STAG, stoi(deaths))));
    // Handles edge case where the root node is split.
    if (currentBTree.GetRoot()->split.first != -99)
    {
        currentBTree.GetRoot()->split = {-99, -99};
    };
}
void AddToHashTable(string state, string gender, string ageRange, string group, string year, string month, string deaths, HashTable& currentHashTable)
{
    // Declares integer variables and calls calculation functions to assign values.
    int stateValue;
    stateValue = StateCalculation(state);
    int genderValue;
    genderValue = GenderCalculation(gender);
    int ageRangeValue;
    ageRangeValue = AgeRangeCalculation(ageRange);
    int timeValue;
    timeValue = TimeCalculation(timeValue, group, year, month);

    // Updates the hash table.
    currentHashTable.CheckForNode(stateValue, ageRangeValue, genderValue, timeValue, stoi(deaths));
}

// Converts user input into usable data.
int StateCalculation(string& state)
{
    // Declares variable and iterates through to determine state.
    string listOfStates[52] = {"Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware",
                               "District of Columbia","Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas",
                               "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi",
                               "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York",
                               "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina",
                               "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", "West Virginia",
                               "Wisconsin", "Wyoming", "Puerto Rico"};

    for (int i = 0; i < 52; i++)
    {
        if (state == listOfStates[i])
        {
            return i;
        }
    }
}
int GenderCalculation(string& gender)
{
    // Assigns based on gender category.
    if (gender == "All Sexes")
    {
        return 0;
    }
    else if (gender == "Male")
    {
        return 1;
    }
    else
    {
        return 2;
    }
}
int AgeRangeCalculation(string& ageRange)
{
    // Creates an array of strings with all ange range options.
    string listOfAgeRanges[17] = {"All Ages", "Under 1 year", "0-17 years", "1-4 years",
                                  "5-14 years", "15-24 years", "18-29 years", "25-34 years",
                                  "30-39 years", "35-44 years","40-49 years", "45-54 years",
                                  "50-64 years","55-64 years", "65-74 years", "75-84 years",
                                  "85 years and over"};

    // iterates through list, compares with variable, and returns index value.
    for (int i = 0; i < 17; i++)
    {
        if (ageRange == listOfAgeRanges[i])
        {
            return i;
        }
    }
}
int TimeCalculation(int& timeValue, string& group, string& year, string& month) {

    // Evaluates group, year, and month data to create a timeValue integer between 0 and 50.
    if (group == "By Total")
    {
        timeValue = 0;
    }
    else
    {
        if (year == "2020")
        {
            timeValue = 1;
        }
        else if (year == "2021")
        {
            timeValue = 14;
        }
        else if (year == "2022")
        {
            timeValue = 27;
        }
        else
        {
            timeValue = 40;
        }
    }
    if (month != "")
    {
        timeValue += stoi(month);
    }

    // Returns calculated value.
    return timeValue;
}

// Prints welcome.
bool DisplayWelcome()
{
    // Prints welcome message
    cout << "========================================================================================================\n"
         << "                                      COVID-19 Mortality Database                                       \n"
         << "                                                                                                        \n"
         << " Hello! This program uses data from the CDC to compare COVID-19 mortality data across states, time      \n"
         << " periods, age ranges, and genders. We hope this helps you prepare for future pandemics!                 \n"
         << "                                                                                                        \n"
         << " Would you like to continue (Yes: 'y')?                                                      \n"
         << "                                                                                                        \n"
         << " Data last updated: September 27, 2023                                                                  \n"
         << "========================================================================================================\n";
    cout <<  "Input: ";
    char input;
    cin  >> input;
    cin.clear();
    if (input == 'y')
    {
        return true;
    }
    else
    {
        return false;
    }
}
// Prints state menu.
void DisplayStateOptions(std::string& state) {
    state = "";
    std::string listOfStates[52] = {"Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware",
                                    "District of Columbia","Florida", "Georgia", "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas",
                                    "Kentucky", "Louisiana", "Maine", "Maryland", "Massachusetts", "Michigan", "Minnesota", "Mississippi",
                                    "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey", "New Mexico", "New York",
                                    "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina",
                                    "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", "West Virginia",
                                    "Wisconsin", "Wyoming", "Puerto Rico"};

    cout << "                                      Please Select A State                                             " << endl;
    cout << "========================================================================================================" << endl;

    // Prints out ordered list of states.
    for (int i = 0; i < 52; i++) {
        if (i % 4 == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(20) << listOfStates[i] << " " << "[" << std::setw(2) << i << "]" << " ";
    }
    cout << endl <<endl;
    cout << "========================================================================================================" << endl;
    cout <<  "STATE OPTION: ";
    cin >> state;
    bool validState = false;
    for (unsigned int i = 0; i < sizeof(listOfStates); i++)
    {
        if (state == to_string(i))
        {
            validState = true;
        }
    }
    if (!validState)
    {
        cout << endl << "Error: Invalid state selection! Please try again." << endl << endl;
        DisplayStateOptions(state);
    }
}
// Prints time options and subcategories.
void DisplayTimeOptions(string& time) {

    // Variable declarations.

    string year;
    string month;
    char input1;
    char input2;
    time = "";

    // Print statements
    cout << "                   Do you want to search a specific year (Yes: 'y' or No: 'n')?                         " << endl;
    cout << "========================================================================================================" << endl;
    cout << "Input: ";
    cin >> input1;

    if (input1 == 'n')
    {
        time = "0";
    }
    else if (input1 == 'y')
    {
        cout <<"                            Which year do you want to search (Format: ####)?                             " << endl;
        cout << "========================================================================================================" << endl;
        cout << "YEAR OPTION: ";
        cin >> year;
        time = "";
        if (year == "2020" || year == "2021" || year == "2022" || year == "2023")
        {
            if (year == "2020")
            {
                time = "1";
            }
            else if (year == "2021")
            {
                time = "14";
            }
            else if (year == "2022")
            {
                time = "27";
            }
            else
            {
                time = "40";
            }
            cout << "                   Do you want to search a specific month (Yes: 'y' or No: 'n')?                        " << endl;
            cout << "========================================================================================================" << endl;
            cout << "Input: ";
            cin >> input2;

            if (input2 == 'y')
            {
                cout
                        << "                    Which month do you want to search (input number e.g. September = 9)?                 "
                        << endl;
                cout
                        << "========================================================================================================"
                        << endl;
                cout << "MONTH OPTION: ";
                cin >> month;

                bool validMonth = false;
                for (unsigned int i = 1; i < 13; i++) {
                    if (month == to_string(i)) {
                        validMonth = true;
                    }
                }
                if (!validMonth) {
                    cout << endl << "Error: Invalid time period selection! Please try again." << endl << endl;
                    DisplayTimeOptions(time);
                } else {
                    int total = stoi(month);
                    total += stoi(time);
                    time = to_string(total);
                    if (stoi(time) > 49)
                    {
                        cout << endl << "Unfortunately, the dataset is only current through September 2023." << endl;
                        cout << "Please select a different time period." << endl << endl;
                        DisplayTimeOptions(time);
                    }
                }
            }
            else if (input2 != 'n')
            {
                cout << endl << "Error: Invalid time period selection! Please try again." << endl << endl;
                DisplayTimeOptions(time);
            }
        }
        else
        {
            cout << endl << "Error: Invalid time period selection! Please try again." << endl << endl;
            DisplayTimeOptions(time);
        }
    }
    else
    {
        cout << endl << "Error: Invalid time period selection! Please try again." << endl << endl;
        DisplayTimeOptions(time);
    }
};
// Prints age options.
void DisplayAgeOptions(string& age) {
    string listOfAgeRanges[17] = {"All Ages", "Under 1 year", "0-17 years", "1-4 years",
                                  "5-14 years", "15-24 years", "18-29 years", "25-34 years",
                                  "30-39 years", "35-44 years","40-49 years", "45-54 years",
                                  "50-64 years","55-64 years", "65-74 years", "75-84 years",
                                  "85 years and over"};

    cout << "                                    Please Select An Age Range                                          " << endl;
    cout << "========================================================================================================" << endl;

    for (int i = 0; i < 17; i++) {
        if (i % 3 == 0) {
            cout << endl;
        }
        cout << setw(20) << listOfAgeRanges[i] << " [" << setw(2) << i << "]" << " ";
    }
    cout << endl << endl;
    cout << "========================================================================================================" << endl;
    cout << "AGE OPTION: ";
    cin >> age;

    bool validAge = false;
    for (unsigned int i = 0; i < sizeof(listOfAgeRanges); i++)
    {
        if (age == to_string(i))
        {
            validAge = true;
        }
    }
    if (!validAge)
    {
        cout << endl << "Error: Invalid age range selection! Please try again." << endl << endl;
        DisplayAgeOptions(age);
    }
};
// Prints gender options.
void DisplayGenderOptions(string& gender) {
    cout << "                   Do you want to search a specific gender (Yes: 'y' or No: 'n')?                       " << endl;
    cout << "========================================================================================================" << endl;
    cout << "GENDER OPTION: ";
    char input1;
    char input2;
    cin >> input1;

    if (input1 == 'y')
    {
        cout <<"                       Which gender do you want to search (Men: 'm' or Women: 'w')?                      " << endl;
        cout << "========================================================================================================" << endl;
        cout << "Gender OPTION: ";
        cin >> setw(1) >> input2;

        if (input2 == 'm')
        {
            gender = "1";
        }
        else if (input2 == 'w')
        {
            gender = "2";
        }
        else
        {
            cout << endl << "Error: Invalid gender selection! Please try again." << endl << endl;
            DisplayGenderOptions(gender);
        }
    }
    else if (input1 == 'n')
    {
        gender = "0";
    }
    else
    {
        cout << endl << "Error: Invalid gender selection! Please try again." << endl <<endl;
        DisplayGenderOptions(gender);
    }
}