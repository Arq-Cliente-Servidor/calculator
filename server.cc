
#include <cmath>
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

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
    s.receive(req);

    string op;
    int op1 = 0, op2 = 0;
    int64_t result = 0;
    req >> op;

    // binary operations
    if (op == "add") {
      req >> op1 >> op2;
      result = op1 + op2;
    }
    else if (op == "sub") {
      req >> op1 >> op2;
      result = op1 - op2;
    }
    else if (op == "mult") {
      req >> op1 >> op2;
      result = op1 * op2;
    }
    else if (op == "div") {
      req >> op1 >> op2;
      result = op1 / op2;
    }

    // unary operations
    else if (op == "sqrt") {
      req >> op1;
      result = sqrt(op1);
    }
    else if (op == "exp") {
      req >> op1;
      result = exp(op1);
    }

    // matrix operations
    else if (op == "mmult") {
      // TODO
    }
    else if (op == "mdet") {
      // TODO
    }

    cout << op << " -> " << op1 << " " << op2 << endl;
    message rep;
    rep << result;
    s.send(rep);
    cout << "Sent: " << result << endl;
  }

  cout << "Finished." << endl;
}
