#include <bits/stdc++.h>

using namespace std;

template <typename T>
class Matrix {
private:
  vector<vector<T>> elements;
  int rows, cols;

public:
  Matrix() : rows(0), cols(0) {}

  Matrix(vector<vector<T>> el) {
    this->elements = el;
    this->rows = elements.size();
    this->cols = elements[0].size();
  }

  static bool checkFormat(string &mat) {
    stack<char> brackets;
    for (int i = 0; i < mat.size(); i++) {
      if (mat[i] == '[')
        brackets.push(mat[i]);
      else if (mat[i] == ']') {
        if (brackets.empty())
          return false;
        brackets.pop();
      }
    }
    return brackets.empty();
  }

  void create(string &mat) {
    string line;
    for (int i = 0; i < mat.size(); i++) {
      if (mat[i] == '[')
        continue;
      if (mat[i] == ']' and line.size()) {
        vector<T> row;
        stringstream ss(line);
        T value;

        while (ss >> value) {
          row.push_back(value);
        }

        this->elements.push_back(row);
        line = "";
      } else
        line += mat[i];
    }

    this->rows = elements.size();
    this->cols = elements[0].size();
  }

  int getRow() {
    return this->rows;
  }

  int getCol() {
    return this->cols;
  }

  vector<vector<T>> getElements() {
    return this->elements;
  }

  T elementAt(int i, int j) {
    return this->elements[i][j];
  }

  bool checkMult(Matrix<T> &mat) {
    int n = this->getCol();
    int m = mat.getRow();
    return n == m;
  }

  Matrix<T> operator * (Matrix<T> &mat) {
    int n = this->getRow();
    int m = this->getCol();
    int p = mat.getCol();
    vector<vector<T>> result(n, vector<int>(p));

    for (int i = 0; i < this->getRow(); i++) {
      for (int j = 0; j < mat.getCol(); j++) {
        for (int k = 0; k < this->getCol(); k++) {
          result[i][j] += this->elementAt(i, k) * mat.elementAt(k, j);
        }
      }
    }

    return Matrix<T>(result);
  }

  string toString() {
    string text = "[";

    for (int i = 0; i < this->getRow(); i++) {
      text += "[";
      for (int j = 0; j < this->getCol(); j++) {
        if (j)
          text += " ";
        text += to_string(this->elementAt(i, j));
      }
      text += "]";
    }

    return text + "]";
  }

  T determinant() {
    T pivot = 0;
    T aux = 0;
    T ans = 1;
    vector<vector<T>> result(this->getElements());

    for (int j = 0; j < this->getCol(); j++) {
      pivot = result[j][j];
      for (int i = j + 1; i < this->getRow(); i++) {
        aux = result[i][j];
        for (int k = 0; k < this->getCol(); k++) {
          result[i][k] -= result[j][k] * (aux / pivot);
        }
      }
    }

    for (int i = 0; i < rows; i++) {
      ans *= result[i][i];
    }

    return ans;
  }

  // Gauss Jordan Method
  Matrix<T> inverse() {
    vector<vector<T>> result(this->getRow(), vector<T>(this->getCol(), 0));
    vector<vector<T>> copy(this->getElements());
    T pivot, fact1, fact2;

    // It is initialized to the identity matrix
    for (int i = 0; i < this->getRow(); i++) {
      result[i][i] = 1;
    }

    // we leave the diagonal matrix lower with zeros
    for (int i = 0; i < this->getRow(); i++) {
      pivot = copy[i][i];

      for (int j = 0; j < this->getCol(); j++) {
        fact1 = copy[i][j];
        fact2 = result[i][j];
        result[i][j] = fact2 / pivot;
        copy[i][j] = fact1 / pivot;
      }

      for (int k = i + 1; k < this->getRow(); k++) {
        fact1 = copy[k][i];
        for (int j = 0; j < this->getCol(); j++) {
          copy[k][j] -= fact1 * copy[i][j];
          result[k][j] -= fact1 * result[i][j];
        }
      }
    }

    // it diagonalized
    for (int j = this->getCol() - 1; j >= 0; j--) {
      for (int i = j - 1; i >= 0; i--) {
        fact1 = copy[i][j];
        for (int k = 0; k < this->getCol(); k++) {
          copy[i][k] -= fact1 * copy[j][k];
          result[i][k] -= fact1 * result[j][k];
        }
      }
    }

    return Matrix<T>(result);
  }

  void print() {
    for (int i = 0; i < this->getRow(); i++) {
      for (int j = 0; j < this->getCol(); j++) {
        cout << this->elementAt(i, j) << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
};

int main(int argc, char const *argv[]) {
  // Ejemplo de formato:
  // [[1 2 3][4 5 6][1 2 3]] [[7 8 4][2 3 4][3 2 1]]
  // [[3 4 -7 6][1 2 -3 4][5 6-7 5][-8 -9 1 2]]
  // [[1 2 3 4][2 1 5 6][3 5 1 7][4 6 7 1]]

  string m1(argv[1]);
  // string m2(argv[2]);

  Matrix<double> mat1;
  mat1.create(m1);
  Matrix<double> mat2 = mat1.inverse();
  cout << (isnan(mat1.determinant())) << endl;
  // mat2.print();


  // mat2.create(m2);

  // Matrix<int> mat3 = mat1 * mat2;
  // mat3.print();

  // vector<vector<int>> mat1 = createMatrix(m1);
  // cout << detMatrix(mat1) << endl;
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
