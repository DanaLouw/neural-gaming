#include <cmath>
#include <iostream>

#include "src/neuralNetworks/NEAT/Connection.hpp"

using std::cout;

int main() {
  std::cout << "Hello, World!" << std::endl;

  connection c(0, ConnectionType::STANDARD, 0, 1, 1.0);
  connection c2(0, ConnectionType::STANDARD, 1, 0, 1.0);
  cout << (c == c2) << std::endl;

  return 0;
}
