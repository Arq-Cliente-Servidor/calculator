#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

typedef vector<vector<int>> Matrix;

Matrix createMatrix(string &mat) {
  Matrix matrix;
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

  return matrix;
}

bool isValid(Matrix &mat1, Matrix &mat2) {
  int n = mat1[0].size();
  int m = mat2.size();
  return n == m;
}

Matrix multMatrix(Matrix &mat1, Matrix &mat2) {
  int n = mat1.size();
  int m = mat2.size();
  int p = mat2[0].size();
  Matrix result(m, vector<int>(p));

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < p; j++) {
      for (int k = 0; k < m; k++) {
        result[i][j] += mat1[i][k] * mat2[k][j];
      }
    }
  }

  return result;
}

string matToString(Matrix &mat) {
  string text = "[";

  for (int i = 0; i < mat.size(); i++) {
    text += "[";
    for (int j = 0; j < mat[i].size(); j++) {
      if (j) text += " ";
      text += to_string(mat[i][j]);
    }
    text += "]";
  }

  return text + "]";
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
      mat1 = createMatrix(ope1);
      mat2 = createMatrix(ope2);

      // Check if it is posibble multiply mat1 by mat2
      if (isValid(mat1, mat2)) {
        mat3 = multMatrix(mat1, mat2);
        resultm = matToString(mat3);
      }
      else {
        resultm = "It is not possible to do matrix multiplication";
      }
    }

    // else if (op == "mdet") {
    //   // TODO
    // }

    if (op == "mmult" or op == "mdet") {
      rep << resultm;
      s.send(rep);
      cout << "Sent: " << resultm << endl;
    }
    else {
      rep << result;
      s.send(rep);
      cout << "Sent: " << result << endl;
    }
  }

  cout << "Finished." << endl;
}
