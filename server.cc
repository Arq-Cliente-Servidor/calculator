// Coded by: Sebastian Duque Restrepo - Carolina Gomez Trejos
#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

typedef vector<vector<int>> Matrix;

// convert a string to an matrix
Matrix createMatrix(string &mat) {
  Matrix matrix;
  string line;

  for (int i = 0; i < mat.size(); i++) {
    if (mat[i] == '[')
      continue;
    if (mat[i] == ']' and line.size()) {
      vector<int> row;
      stringstream ss(line);
      int value;

      while (ss >> value) {
        row.push_back(value);
      }

      matrix.push_back(row);
      line = "";
    } else
      line += mat[i];
  }

  return matrix;
}

// checks if it is possible multiply two matrices
bool checkMult(Matrix &mat1, Matrix &mat2) {
  int n = mat1[0].size();
  int m = mat2.size();
  return n == m;
}

// checks if the matrix has the correct format
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

void printMatrix(Matrix &matrix) {
  for (int i = 0; i < matrix.size(); i++) {
    for (int j = 0; j < matrix[i].size(); j++) {
      cout << matrix[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

Matrix multMatrix(Matrix &mat1, Matrix &mat2) {
  int n = mat1.size();
  int m = mat2.size();
  int p = mat2[0].size();
  Matrix result(n, vector<int>(p));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < p; j++) {
      for (int k = 0; k < m; k++) {
        result[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }

  return result;
}

Matrix detMatrix(Matrix &mat) {
  int rows = mat.size();
  int cols = mat[0].size();
  double pivot = 0.0;
  double aux = 0.0;
  vector<vector<double>> result(rows, vector<double>(cols));

  // convert matrix int to matrix double
  for (int i = 0; i < rows); i++) {
    for (int j = 0; j < cols; j++) {
      result[i][j] = mat[i][j];
    }
  }

  for (int j = 0; j < cols; j++) {
    pivot = result[i][i];
    for (int i = j + 1; i < rows; i++) {
      aux = result[i][j];
      for (int k = 0; k < cols; k++) {
        result[i][k] = result[i][k] - (result[j][k] * pivot / aux);
      }
    }
  }

}

string matToString(Matrix &mat) {
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
    Matrix mat1, mat2, mat3;
    string resultm;

    // Binary operations
    if (op == "add") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 + op2;
    } else if (op == "sub") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 - op2;
    } else if (op == "mult") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 * op2;
    } else if (op == "div") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 / op2;
    }

    // Unary operations
    else if (op == "sqrt") {
      op1 = atoi(ope1.c_str());
      result = sqrt(op1);
    } else if (op == "exp") {
      op1 = atoi(ope1.c_str());
      result = exp(op1);
    }

    // Matrix operations
    else if (op == "mmult") {
      req >> ope2;
      if (checkMat(ope1) and checkMat(ope2)) {
        mat1 = createMatrix(ope1);
        mat2 = createMatrix(ope2);
        // Check if it is posibble multiply mat1 by mat2
        if (checkMult(mat1, mat2)) {
          mat3 = multMatrix(mat1, mat2);
          resultm = matToString(mat3);
        } else {
          resultm = "It is not possible to do matrix multiplication";
        }
      } else {
        resultm = "The matrix was not entered in the correct format";
      }
    }

    else if (op == "mdet") {
      if (checkMat(ope1)) {
        mat1 = createMatrix(ope1);
        resultm = to_string(detMatrix(mat1));
      } else {
        resultm = "The matrix was not entered in the correct format";
      }
    }

    if (op == "mmult" or op == "mdet") {
      rep << resultm;
      s.send(rep);
      cout << "Sent: " << resultm << endl;
    } else {
      rep << result;
      s.send(rep);
      cout << "Sent: " << result << endl;
    }
  }

  cout << "Finished." << endl;
}
