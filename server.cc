
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int toInt(string x) {
  stringstream s;
  s << x;
  int r;
  s >> r;
  return r;
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
    s.receive(req);
    string op;
    string ope1;
    string ope2;
    req >> op >> ope1;
    int result;
    int op1 = toInt(ope1);
    int op2;
    if (op == "sqrt") {
      result = sqrt(op1);
    }
    if (op == "ln") {
      result = log(op1);
    }
    if (op == "add") {
      req >> ope2;
      op2 = toInt(ope2);
      result = op1 + op2;
    }
    if (op == "sub") {
      req >> ope2;
      op2 = toInt(ope2);
      result = op1 - op2;
    }
    if (op == "mult") {
      req >> ope2;
      op2 = toInt(ope2);
      result = op1 * op2;
    }
    if (op == "div") {
      req >> ope2;
      op2 = toInt(ope2);
      result = op1 / op2;
    }
    if (op == "mmat") {
      req >> ope2;
      op2 = toInt(ope2);
    }
    cout << op << " -> " << ope1 << " " << ope2 << endl;
    message rep;
    rep << result;
    s.send(rep);
    cout << "Sent: " << result << endl;
  }

  cout << "Finished." << endl;
}
