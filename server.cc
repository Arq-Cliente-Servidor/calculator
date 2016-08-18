
#include <cmath>
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

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

void printMatrix(vector<vector<int>> &matrix) {
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
    message req;
    message rep;
    s.receive(req);

    string op;
    string ope1;
    string ope2;
    req >> op >> ope1;

    int result;
    int op1;
    int op2;
    vector<vector<int>> mat1, mat2, resultm;

    if (op == "sqrt") {
     op1 = atoi(ope1.c_str());
     result = sqrt(op1);
    }
    if (op == "exp") {
      op1 = atoi(ope1.c_str());
      result = exp(op1);
    }
    if (op == "add") {
      req >> ope2;
      p1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 + op2;
    }
    if (op == "sub") {
      req >> ope2;
      op1 = atoi(ope1.c_str());
      op2 = atoi(ope2.c_str());
      result = op1 - op2;
   }
   if (op == "mult") {
     req >> ope2;
     op1 = atoi(ope1.c_str());
     op2 = atoi(ope2.c_str());
     result = op1 * op2;
   }
   if (op == "div") {
     req >> ope2;
     op1 = atoi(ope1.c_str());
     op2 = atoi(ope2.c_str());
     result = op1 / op2;
   }
   if (op == "mmult") {
     req >> ope2;
     mat1 = createMatrix(ope1);
     mat2 = createMatrix(ope2);
     resultm = multMatrix(mat1, mat2);
   }

    // else if (op == "mdet") {
    //   // TODO
    // }

    if (op == "mmult" or op == "mdet") {
      rep << resultm;
      s.send(rep);
      cout << "Sent: ";
      printMatrix(resultm);
      cout << endl;
    }
    else {
      rep << result;
      s.send(rep);
      cout << "Sent: " << result << endl;
    }
  }

  cout << "Finished." << endl;
}
