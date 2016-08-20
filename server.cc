// Coded by: Sebastian Duque Restrepo - Carolina Gomez Trejos
#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

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

  // checks if the matrix has the correct format
  bool checkFormat(string &mat) {
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

  // convert a string to an matrix
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
      }
      else line += mat[i];
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

  // checks if it is possible multiply two matrices
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

int main(int argc, char *argv[]) {
  const string endpoint = "tcp://*:4242";

  // initialize the 0MQ context
  context ctx;

  // generate a pull socket
  socket s(ctx, socket_type::reply);

  // bind to the socket
  cout << "Binding to " << endpoint << "..." << endl;
  s.bind(endpoint);

  // receive the message
  while (true) {
    cout << "Receiving message..." << endl;
    message req, rep;
    s.receive(req);

    string op;
    string ope1, ope2;
    req >> op >> ope1;

    int64_t result = 0;
    int op1 = 0, op2 = 0;
    Matrix<int> mat1, mat2, mat3;
    Matrix<double> matDet;
    string matrixResult;

    // Binary operations
    if (op == "add") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 + op2;
    }
    else if (op == "sub") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 - op2;
    }
    else if (op == "mult") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 * op2;
    }
    else if (op == "div") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 / op2;
    }

    // Unary operations
    else if (op == "sqrt") {
      op1 = atoi(ope1.c_str());
      result = sqrt(op1);
    }
    else if (op == "exp") {
      op1 = atoi(ope1.c_str());
      result = exp(op1);
    }

    // Matrix operations
    else if (op == "mmult") {
      req >> ope2;
      if (mat1.checkFormat(ope1) and mat1.checkFormat(ope2)) {
        mat1.create(ope1);
        mat2.create(ope2);
        // Check if it is posibble multiply mat1 by mat2
        if (mat1.checkMult(mat2)) {
          mat3 = mat1 * mat2;
          matrixResult = mat3.toString();
        }
        else {
          matrixResult = "It is not possible to do matrix multiplication";
        }
      }
      else {
        matrixResult = "The matrix was not entered in the correct format";
      }
    }

    else if (op == "mdet") {
      if (matDet.checkFormat(ope1)) {
        matDet.create(ope1);
        matrixResult = to_string(matDet.determinant());
      }
      else {
        matrixResult = "The matrix was not entered in the correct format";
      }
    }

    if (op == "mmult" or op == "mdet") {
      rep << matrixResult;
      s.send(rep);
      cout << "Sent: " << matrixResult << endl;
    }
    else {
      rep << result;
      s.send(rep);
      cout << "Sent: " << result << endl;
    }
  }

  cout << "Finished." << endl;
}
