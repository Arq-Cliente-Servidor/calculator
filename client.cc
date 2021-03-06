// Coded by: Sebastian Duque Restrepo - Carolina Gomez Trejos
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char *argv[]) {
  const string endpoint = "tcp://localhost:4242";
  if (argc != 3 && argc != 4) {
    cerr << "Error calling the program." << endl;
    return 1;
  }

  string op(argv[1]);
  if (argc == 3 and (op == "add" or op == "mult" or op == "div" or op == "sub" or op == "mmult")) {
    cerr << "Invalid number of operands, expected 2." << endl;
    return 1;
  } else if (argc == 4 and (op == "sqrt" or op == "log" or op == "minv" or op == "mdet")) {
    cerr << "Invalid number of operands, expected 1." << endl;
    return 1;
  }

  // initialize the 0MQ context
  context ctx;
  socket s(ctx, socket_type::request);

  // open the connection
  cout << "Opening connection to " << endpoint << "..." << endl;
  s.connect(endpoint);

  // send a message
  cout << "Sending operation..." << endl;
  message req;
  string op1, op2;

  if (argc == 3) {
    op1 = argv[2];
    req << op << op1;
  } else if (argc == 4) {
    op1 = argv[2];
    op2 = argv[3];
    req << op << op1 << op2;
  }

  s.send(req);
  cout << "Request sent." << endl;

  message rep;
  s.receive(rep);

  if (op == "mmult" or op == "mdet" or op == "minv") {
    string result;
    rep >> result;
    cout << "Response " << result << endl;
  } else {
    double result = 0.0;
    rep >> result;
    cout << "Response " << fixed << result << endl;
  }

  cout << "Finished." << endl;
  return 0;
}
