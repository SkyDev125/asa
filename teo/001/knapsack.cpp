#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Item {
   private:
    int _value;
    int _weight;

   public:
    Item(int value, int weight) {
        _value = value;
        _weight = weight;
    }
    int value() { return _value; }
    int weight() { return _weight; }
    string stringify() {
        return string("(value: ") + to_string(_value) + string(", weight: ") +
               to_string(_weight) + string(")");
    }
};

class Knapsack {
   private:
    int _weight_limit;
    vector<Item> _items;

   public:
    Knapsack(int weight_limit) { this->_weight_limit = weight_limit; }
    Knapsack(int weight_limit, vector<Item> items) : _items(items) {
        this->_weight_limit = weight_limit;
    }
    int weight() { return _weight_limit; }
    vector<Item>& items() { return _items; }
    string stringify() {
        ostringstream result;
        result << "Knapsack(weight_limit: " + to_string(_weight_limit) +
                      ", items: [";
        if (!_items.empty()) {
            for (auto current = _items.begin(); current != _items.end() - 1;
                 current++) {
                result << current->stringify() << ",";
            }
            result << _items.back().stringify();
        }
        result << "])";
        return result.str();
    }
};

// Print all the values of the table with consistent spacing
void printTable(vector<vector<int>>& table) {
    for (auto row : table) {
        for (auto col : row) {
            cout << setw(4) << col;
        }
        cout << endl;
    }
}

Knapsack findOptimalWithoutRep(Knapsack sack, vector<Item>& items) {
    int columns = sack.weight();
    int rows = items.size();

    // Create a 2D vector (table) with rows and columns
    vector<vector<int>> table(rows + 1, vector<int>(columns + 1));
    for (int row = 1; row <= rows; row++) {
        for (int col = 1; col <= columns; col++) {
            // Get the required indexes.
            Item item = items[row - 1];
            int best_col = col - item.weight();

            // Calculate the values.
            int top_value = table[row - 1][col];
            if (best_col < 0) {
                table[row][col] = top_value;
                continue;
            };
            int new_value = table[row - 1][best_col] + item.value();

            // Set the new value for the table.
            if (new_value > top_value) {
                table[row][col] = new_value;
            } else {
                table[row][col] = top_value;
            }
        }
    }

    printTable(table);

    // Select the items and put them in the bag
    for (int row = rows, col = columns; row > 0;) {
        if (table[row][col] != table[row - 1][col]) {
            Item selectedItem = items[row - 1];
            sack.items().push_back(selectedItem);
            col -= selectedItem.weight();
        }
        row--;
    }

    return sack;
}

Knapsack findOptimalWithRep(Knapsack sack, vector<Item> items) {
    int columns = sack.weight();
    int rows = items.size();

    // Create a 2D vector (table) with rows and columns
    vector<vector<int>> table(rows + 1, vector<int>(columns + 1));
    for (int row = 1; row <= rows; row++) {
        for (int col = 1; col <= columns; col++) {
            // Get the required indexes.
            Item item = items[row - 1];
            int top_value = table[row - 1][col];
            int new_value = 0;
            int check_num = col / item.weight();

            // Calculate the values.
            if (check_num <= 0) {
                table[row][col] = top_value;
                continue;
            };
            // test for all combinations of having x*Items.
            for (int i = check_num; i > 0; i--) {
                int test_col = col - i * item.weight();
                int total_value = table[row - 1][test_col] + i * item.value();
                if (total_value > new_value) {
                    new_value = total_value;
                }
            }

            // Set the new value for the table.
            if (new_value > top_value) {
                table[row][col] = new_value;
            } else {
                table[row][col] = top_value;
            }
        }
    }

    printTable(table);

    // Select the items and put them in the bag
    for (int row = rows, col = columns; row > 0;) {
        if (table[row][col] != table[row - 1][col]) {
            Item selectedItem = items[row - 1];
            int selectedItemWeight = selectedItem.weight();

            // find how many times we have to move back
            int check_num = col / selectedItemWeight;
            while (check_num > 0) {
                int total_value =
                    check_num * selectedItem.value() +
                    table[row - 1][col - check_num * selectedItemWeight];
                if (table[row][col] == total_value) {
                    col -= check_num * selectedItemWeight;
                    break;
                }
                check_num--;
            }

            // Add the item the remaining times.
            while (check_num > 0) {
                sack.items().push_back(selectedItem);
                check_num--;
            }
        }
        row--;
    }
 
    return sack;
}

int main() {
    int max_weight = 10;
    vector<Item> items = {Item(60, 2), Item(100, 3), Item(120, 5),
                          Item(80, 4), Item(90, 6),  Item(70, 1)};
    Knapsack sack = Knapsack(max_weight);
    cout << "<================= Without Repetition ==================>" << endl;
    cout << findOptimalWithoutRep(sack, items).stringify() << endl;
    cout << "<================== With Repetition ====================>" << endl;
    cout << findOptimalWithRep(sack, items).stringify() << endl;
}
