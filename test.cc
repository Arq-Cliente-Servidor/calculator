#include <bits/stdc++.h>

using namespace std;

typedef vector<vector<int>> Matrix;

vector<vector<int>> createMatrix(string &mat) {
  vector<vector<int>> matrix;
  string line;

  for (int i = 0; i < mat.size(); i++) {
    if (mat[i] == '[')
      continue;
    if (mat[i] == ']' and line.size()) {
      vector<int> col;
      stringstream ss(line);
      int value;

      while (ss >> value) {
        col.push_back(value);
      }

      matrix.push_back(col);
      line = "";
    } else
      line += mat[i];
  }

  // cout << "rows: " << ma/trix.size() << " cols: " << matrix[0].size() << endl;
  return matrix;
}

bool checkMat(string &mat) {
  stack<char> brackets;

  for (int i = 0; i < mat.size(); i++) {
    if (mat[i] == '[')
      brackets.push(mat[i]);
    else if (mat[i] == ']') {
      if (brackets.empty()) return false;
      brackets.pop();
    }
  }

  return brackets.empty();
}

void printMatrix(vector<vector<double>> &matrix) {
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

vector<vector<int>> multMatrix(vector<vector<int>> &mat1, vector<vector<int>> &mat2) {
  int n = mat1[0].size();
  int m = mat1.size();
  int p = mat2[0].size();
  vector<vector<int>> result(m, vector<int>(p));

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      for (int k = 0; k < n; k++) {
        result[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }

  return result;
}

string matToString(vector<vector<int>> &mat) {
  string text = "[";

  for (int i = 0; i < mat.size(); i++) {
    text += "[";
    for (int j = 0; j < mat[i].size(); j++) {
      if (j)
        text += " ";
      text += to_string(mat[i][j]);
    }
    text += "]";
  }

  return text + "]";
}

double detMatrix(Matrix &mat) {
  int rows = mat.size();
  int cols = mat[0].size();
  double pivot = 0.0;
  double aux = 0.0;
  double ans = 1.0;
  vector<vector<double>> result(rows, vector<double>(cols));

  // convert matrix int to matrix double
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      result[i][j] = (double)mat[i][j];
    }
  }

  for (int j = 0; j < cols; j++) {
    pivot = result[j][j];
    for (int i = j + 1; i < rows; i++) {
      aux = result[i][j];
      for (int k = 0; k < cols; k++) {
        result[i][k] -= result[j][k] * (aux / pivot);
      }
    }
  }

  for (int i = 0; i < rows; i++) {
    ans *= result[i][i];
  }

  return ans;
}

int main(int argc, char const *argv[]) {
  // Ejemplo de formato:
  // [[1 2 3][4 5 6][1 2 3]] [[7 8 4][2 3 4][3 2 1]]

  string m1(argv[1]);
  vector<vector<int>> mat1 = createMatrix(m1);
  cout << detMatrix(mat1) << endl;
  // printMatrix(mat2);
  // cout << checkMat(m1) << endl;
  // string m2(argv[2]);

  // cout << m1 << endl;
  // printMatrix(mat1);
  // cout << matToString(mat1) << endl;

  // cout << m2 << endl;
  // vector<vector<int>> mat2 = createMatrix(m2);
  // printMatrix(mat2);

  // vector<vector<int>> mat3 = multMatrix(mat1, mat2);
  // printMatrix(mat3);

  return 0;
}
