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

// Overload for vector<vector<int>>
void printTable(const vector<vector<int>> &table) {
    for (const auto &row : table) {
        for (const auto &col : row) {
            cout << setw(10) << col;
        }
        cout << endl;
    }
}

class Matrix {
   private:
    int _rows;
    int _columns;
    vector<vector<int>> _matrix;

   public:
    Matrix(int rows, int columns, vector<vector<int>> &matrix) {
        _rows = rows;
        _columns = columns;
        _matrix = matrix;
    }
    Matrix(int rows, int columns) {
        _rows = rows;
        _columns = columns;
    }
    int columns() { return _columns; }
    int rows() { return _rows; }
    vector<vector<int>> &matrix() { return _matrix; }
    Matrix multiply(Matrix &M2) {
        if (_columns != M2.rows()) {
            throw invalid_argument(
                "Matrix dimensions do not match for multiplication");
        }
        vector<vector<int>> solution_v(_rows, vector<int>(M2.columns()));
        Matrix solution(_rows, M2.columns(), solution_v);
        for (int row = 0; row < _rows; row++) {
            for (int col = 0; col < M2.columns(); col++) {
                int temp_value = 0;
                for (int k = 0; k < _columns; k++) {
                    temp_value += _matrix[row][k] * M2.matrix()[k][col];
                }
                solution.matrix()[row][col] = temp_value;
            }
        }
        return solution;
    }
    string strigify() {
        stringstream matrix;
        for (const auto &row : _matrix) {
            for (const auto &column : row) {
                matrix << setw(10) << column;
            }
            matrix << endl;
        }
        return matrix.str();
    }
};

string parentise(vector<vector<int>> &aux_matrix, int row, int col,
                 bool outer = true) {
    if (row == col) {
        return "A" + to_string(row + 1);
    }
    int k = aux_matrix[row][col];
    string left = parentise(aux_matrix, row, k, false);
    string right = parentise(aux_matrix, k + 1, col, false);
    if (outer) {
        return left + "*" + right;
    } else {
        return "(" + left + "*" + right + ")";
    }
}

Matrix multiplyByOrder(vector<Matrix> &matrices,
                       vector<vector<int>> &aux_matrix, int row, int col) {
    if (row == col) {
        return matrices[row];
    }
    int k = aux_matrix[row][col];
    Matrix left = multiplyByOrder(matrices, aux_matrix, row, k);
    Matrix right = multiplyByOrder(matrices, aux_matrix, k + 1, col);
    return left.multiply(right);
}

Matrix optimalMultiplication(vector<Matrix> &matrices) {
    int total_matrices = matrices.size();
    vector<vector<int>> matrix(total_matrices, vector<int>(total_matrices));
    vector<vector<int>> aux_matrix(total_matrices, vector<int>(total_matrices));

    for (int i = 1; i < total_matrices; i++) {
        for (int row = 0, col = i; col < total_matrices; row++, col++) {
            int mult_min = INT32_MAX;

            for (int k = col - 1; k >= row; k--) {
                int mult_total = matrices[row].rows() * matrices[k].columns() *
                                 matrices[col].columns();
                mult_total = matrix[row][k] + matrix[k + 1][col] + mult_total;

                if (mult_total < mult_min) {
                    mult_min = mult_total;
                    aux_matrix[row][col] = k;
                }
            }

            matrix[row][col] = mult_min;
        }
    }

    cout << "<===================== Matrix =====================>" << endl;
    printTable(matrix);
    cout << "<===================== Aux Matrix =====================>" << endl;
    printTable(aux_matrix);
    cout << "<===================== Parentisised =====================>"
         << endl;
    cout << parentise(aux_matrix, 0, total_matrices - 1) << endl;

    return multiplyByOrder(matrices, aux_matrix, 0, total_matrices - 1);
}

int main() {
    vector<vector<int>> v1 = {{1, 2}, {2, 3}};
    vector<vector<int>> v2 = {{1, 2, 4}, {2, 3, 6}};
    vector<vector<int>> v3 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> v4 = {{1, 2}, {3, 4}, {5, 6}};
    vector<vector<int>> v5 = {{1, 2, 3, 4}, {5, 6, 7, 8}};
    vector<vector<int>> v6 = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
    vector<Matrix> matrices = {Matrix(2, 2, v1), Matrix(2, 3, v2),
                               Matrix(3, 3, v3), Matrix(3, 2, v4),
                               Matrix(2, 4, v5), Matrix(4, 2, v6)};

    cout << optimalMultiplication(matrices).strigify();
}