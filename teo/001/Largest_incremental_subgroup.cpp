#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

// Print all the values of the table with consistent spacing
// Overload for vector<int>
void printTable(const vector<int> &table) {
    for (const auto &col : table) {
        cout << setw(4) << col;
    }
    cout << endl;
}

void printTable(const vector<char>& table) {
    for (char ch : table) {
        cout << ch << " ";
    }
    cout << endl;
}

// Overload for vector<vector<int>>
void printTable(const vector<vector<int>> &table) {
    for (const auto &row : table) {
        for (const auto &col : row) {
            cout << setw(4) << col;
        }
        cout << endl;
    }
}

vector<int> LIS(vector<int> &num_list) {
    int list_size = num_list.size();
    vector<vector<int>> matrix(list_size, vector<int>(list_size));

    for (int i = 1; i < list_size; i++) {
        for (int row = 0, col = i; col < list_size; row++, col++) {
            // Check to try to skip some steps
            int left_value = matrix[row][col - 1];
            int bottom_value = matrix[row + 1][col];
            if (left_value < bottom_value) {
                matrix[row][col] = bottom_value;
                continue;
            } else if (left_value > bottom_value) {
                matrix[row][col] = left_value;
                continue;
            }

            // Find if an extra combination can be made.
            bool bigger;
            int new_num = num_list[col];

            if (left_value == 0) {
                bigger = new_num > num_list[col - 1];
            } else {
                int check_col = col - 2;
                for (; left_value == matrix[row][check_col]; check_col--) {
                }
                bigger = new_num > num_list[check_col + 1];
            }

            if (left_value + bigger > bottom_value) {
                matrix[row][col] = left_value + 1;
            } else {
                matrix[row][col] = bottom_value;
            }
        }
    }

    // Trace Back
    vector<int> solution;
    for (int row = 0, col = list_size - 1; col >= 0;) {
        int left_value = matrix[row][col - 1];
        int bottom_value = matrix[row + 1][col];
        int current_value = matrix[row][col];

        if (current_value == 0) {
            solution.push_back(num_list[col]);
            break;
        }

        if (current_value == bottom_value) {
            row++;
            continue;
        }

        if (current_value == left_value) {
            col--;
            continue;
        }

        solution.push_back(num_list[col]);
        col--;
    }

    reverse(solution.begin(), solution.end());

    return solution;
}

vector<char> LIS(const vector<char>& chars) {
    vector<int> nums(chars.begin(), chars.end());
    vector<int> lisNums = LIS(nums);
    vector<char> lisChars(lisNums.begin(), lisNums.end());
    return lisChars;
}

int main() {
    srand(time(0));

    // Very extreme example with integers
    vector<int> extremeTest(100);
    for (int i = 0; i < 100; ++i) {
        extremeTest[i] = rand() % 100;
    }

    cout << "Extreme Test with Integers:" << endl;
    printTable(extremeTest);
    printTable(LIS(extremeTest));

    // Example with characters
    vector<char> charTest= {'g','e','e','k','s','f','o','r','g','e','e','k','s'};

    cout << "Extreme Test with Characters:" << endl;
    printTable(charTest);
    printTable(LIS(charTest));

    return 0;
}