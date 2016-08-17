
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
    int op1 = 0;
    int op2 = 0;
    req >> op >> op1;
    int result;
    if (op == "sqrt") {
      result = sqrt(op1);
    }
    if (op == "ln") {
      result = log(op1);
    }
    if (op == "add") {
      req >> op2;
      result = op1 + op2;
    }
    if (op == "sub") {
      req >> op2;
      result = op1 - op2;
    }
    if (op == "mult") {
      req >> op2;
      result = op1 * op2;
    }
    if (op == "div") {
      req >> op2;
      result = op1 / op2;
    }
    message rep;
    rep << result;
    s.send(rep);
    cout << "Sent: " << result << endl;
  }

  cout << "Finished." << endl;
}
