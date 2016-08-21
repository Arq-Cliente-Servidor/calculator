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

  int getNumRows() {
    return this->rows;
  }

  int getNumCols() {
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
    int n = this->getNumCols();
    int m = mat.getNumRows();
    return n == m;
  }

  Matrix<T> operator * (Matrix<T> &mat) {
    int n = this->getNumRows();
    int m = this->getNumCols();
    int p = mat.getNumCols();
    vector<vector<T>> result(n, vector<int>(p));

    for (int i = 0; i < this->getNumRows(); i++) {
      for (int j = 0; j < mat.getNumCols(); j++) {
        for (int k = 0; k < this->getNumCols(); k++) {
          result[i][j] += this->elementAt(i, k) * mat.elementAt(k, j);
        }
      }
    }

    return Matrix<T>(result);
  }

  string toString() {
    string text = "\n[";

    for (int i = 0; i < this->getNumRows(); i++) {
      text += (i)? " [" : "[";
      for (int j = 0; j < this->getNumCols(); j++) {
        if (j)
          text += " ";
        text += to_string(this->elementAt(i, j));
      }
      text += (i < this->getNumRows() - 1)? "]\n" : "]";
    }

    return text + "]";
  }

  T determinant() {
    T pivot = 0;
    T aux = 0;
    T ans = 1;
    vector<vector<T>> result(this->getElements());

    for (int j = 0; j < this->getNumCols(); j++) {
      pivot = result[j][j];
      for (int i = j + 1; i < this->getNumRows(); i++) {
        aux = result[i][j];
        for (int k = 0; k < this->getNumCols(); k++) {
          result[i][k] -= result[j][k] * (aux / pivot);
        }
      }
    }

    for (int i = 0; i < rows; i++) {
      ans *= result[i][i];
    }

    return ans;
  }

  // Inverse for the Gauss Jordan Method
  Matrix<T> inverse() {
    vector<vector<T>> result(this->getNumRows(), vector<T>(this->getNumCols(), 0));
    vector<vector<T>> copy(this->getElements());
    T pivot, fact1, fact2;

    // It is initialized to the identity matrix
    for (int i = 0; i < this->getNumRows(); i++) {
      result[i][i] = 1;
    }

    // we leave the diagonal matrix lower with zeros
    for (int i = 0; i < this->getNumRows(); i++) {
      pivot = copy[i][i];

      for (int j = 0; j < this->getNumCols(); j++) {
        fact1 = copy[i][j];
        fact2 = result[i][j];
        result[i][j] = fact2 / pivot;
        copy[i][j] = fact1 / pivot;
      }

      for (int k = i + 1; k < this->getNumRows(); k++) {
        fact1 = copy[k][i];
        for (int j = 0; j < this->getNumCols(); j++) {
          copy[k][j] -= fact1 * copy[i][j];
          result[k][j] -= fact1 * result[i][j];
        }
      }
    }

    // it diagonalized
    for (int j = this->getNumCols() - 1; j >= 0; j--) {
      for (int i = j - 1; i >= 0; i--) {
        fact1 = copy[i][j];
        for (int k = 0; k < this->getNumCols(); k++) {
          copy[i][k] -= fact1 * copy[j][k];
          result[i][k] -= fact1 * result[j][k];
        }
      }
    }

    return Matrix<T>(result);
  }

  // checks if a value is infinite, NAN or not
  bool validValue(T value) {
    return !isnan(value) and !isinf(value);
  }

  void print() {
    for (int i = 0; i < this->getNumRows(); i++) {
      for (int j = 0; j < this->getNumCols(); j++) {
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
    cout << "Operation: " << op << endl;

    double result = 0.0;
    double op1 = 0.0, op2 = 0.0;
    Matrix<int> mat1, mat2, mat3;
    Matrix<double> matDet;
    string matrixResult;

    // Binary operations
    if (op == "add") {
      req >> ope2;
      op1 = stof(ope1.c_str());
      op2 = stof(ope2.c_str());
      result = op1 + op2;
      cout << op1 << " + " << op2 << " = " << result << endl;
    }
    else if (op == "sub") {
      req >> ope2;
      op1 = stof(ope1.c_str());
      op2 = stof(ope2.c_str());
      result = op1 - op2;
      cout << op1 << " - " << op2 << " = " << result << endl;
    }
    else if (op == "mult") {
      req >> ope2;
      op1 = stof(ope1.c_str());
      op2 = stof(ope2.c_str());
      result = op1 * op2;
      cout << op1 << " * " << op2 << " = " << result << endl;
    }
    else if (op == "div") {
      req >> ope2;
      op1 = stof(ope1.c_str());
      op2 = stof(ope2.c_str());
      result = op1 / op2;
      cout << op1 << " / " << op2 << " = " << result << endl;
    }

    // Unary operations
    else if (op == "sqrt") {
      op1 = stof(ope1.c_str());
      result = sqrt(op1);
      cout << "sqrt(" << op1 << ") = " << result << endl;
    }
    else if (op == "log") {
      op1 = stof(ope1.c_str());
      result = log10(op1);
      cout << "log10(" << op1 << ") = " << result << endl;
    }

    // Matrix operations
    else if (op == "mmult") {
      req >> ope2;
      if (mat1.checkFormat(ope1) and mat1.checkFormat(ope2)) {
        mat1.create(ope1);
        mat2.create(ope2);

        cout << "Matrix one:";
        cout << mat1.toString() << endl;
        cout << "Matrix two:";
        cout << mat2.toString() << endl;

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
        cout << "Matrix:";
        cout << matDet.toString() << endl;

        if (!matDet.validValue(matDet.determinant())) {
          matrixResult = "It is not possible to do the determinant of the matrix";
        }
        else matrixResult = to_string(matDet.determinant());
      }
      else {
        matrixResult = "The matrix was not entered in the correct format";
      }
    }

    else if (op == "minv") {
      if (matDet.checkFormat(ope1)) {
        matDet.create(ope1);
        cout << "Matrix:";
        cout << matDet.toString() << endl;

        if (!matDet.validValue(matDet.determinant()) or matDet.determinant() == 0.0) {
          matrixResult = "It is not possible to do the inverse of the matrix";
        }
        else matrixResult = matDet.inverse().toString();
      }
      else {
        matrixResult = "The matrix was not entered in the correct format";
      }
    }

    if (op == "mmult" or op == "mdet" or op == "minv") {
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
