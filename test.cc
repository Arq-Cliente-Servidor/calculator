#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> createMatrix(string &mat) {
  vector<vector<int>> matrix;
  string line;

  for (int i = 0; i < mat.size(); i++) {
    if (mat[i] == '[') continue;
    if (mat[i] == ']' and line.size()) {
      vector<int> col;
      stringstream ss(line);
      int value;

      while (ss >> value) {
        col.push_back(value);
      }

      matrix.push_back(col);
      line = "";
    }
    else line += mat[i];
  }

  cout << "rows: " << matrix.size() << " cols: " << matrix[0].size() << endl;
  return matrix;
}

void printMatrix(vector<vector<int>> &matrix) {
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

int main(int argc, char const *argv[]) {
  // Ejemplo de formato:
  // [[1 2 3][4 5 6][1 2 3]] [[7 8 4][2 3 4][3 2 1]]

  string m1(argv[1]);
  string m2(argv[2]);

  cout << m1 << endl;
  vector<vector<int>> mat1 = createMatrix(m1);
  printMatrix(mat1);

  cout << m2 << endl;
  vector<vector<int>> mat2 = createMatrix(m2);
  printMatrix(mat2);

  return 0;
}
