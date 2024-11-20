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
        for (auto current = _items.begin(); current != _items.end() - 1;
             current++) {
            result << current->stringify() << ",";
        }
        result << _items.back().stringify() << "])";
        return result.str();
    }
};

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

    // Print all the values of the table with consistent spacing
    for (auto row : table) {
        for (auto col : row) {
            cout << setw(4) << col;  // Use setw to set the width of each column
        }
        cout << endl;
    }

    // Select the items and put them in the bag
    for (int row = rows, col = columns; row > 0;) {
        if (table[row][col] != table[row - 1][col]) {
            Item selectedItem = items[row - 1];
            sack.items().push_back(selectedItem);
            col -= selectedItem.weight();
            cout << selectedItem.stringify() << endl;
        }
        row--;
    }

    return sack;
}

Knapsack findOptimalWithRep(Knapsack sack, vector<Item> items) { return sack; }

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
