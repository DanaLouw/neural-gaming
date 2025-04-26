// Dana Louw 2025-01-10

#include <algorithm>
//#include <cstddef>
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
//#include <utility>
#include <vector>
#include <optional>

#include "../../../lib/random/Random.hpp"
#include "Brain.hpp"
#include "Connection.hpp"
#include "Node.hpp"

extern RandomGenerator* randomGenerator;

Brain::Brain(uint inputNodes, uint outputNodes, uint numStartingConnections) {
  // Set input and output nodes
  this->inputNodes = inputNodes;
  this->outputNodes = outputNodes;

  // Add input and output nodes
  for (int i = 0; i < inputNodes; i++) {
    node newNode(i, nodeTypes::INPUT, randomGenerator->randomDouble(-1.0, 1.0), activationTypes::LINEAR);
    nodes.push_back(newNode);
    values.push_back(0.0);
  }

  // Add hidden nodes
  for (int i = 0; i < 5; i++) {
    node newNode(i + nodes.size(), nodeTypes::HIDDEN, randomGenerator->randomDouble(-1.0, 1.0), activationTypes::TANH);
    nodes.push_back(newNode);
    values.push_back(0.0);
  }

  // Add output nodes
  for (int i = 0; i < outputNodes; i++) {
    node newNode(i + nodes.size(), nodeTypes::OUTPUT, randomGenerator->randomDouble(-1.0, 1.0), activationTypes::LINEAR);
    nodes.push_back(newNode);
    values.push_back(0.0);
  }

  // Add starting connections
  for (int i = 0; i <= numStartingConnections; i++) {
    addConnection();
  }
}

// Calculate the outputs
void Brain::calculate() {
  // Set remaining values to 0
  for (int i = inputNodes; i < nodes.size(); i++) {
    values[i] = 0;
  }

  // Calculate the values
  int fromNodeIndex = -1;
  for (int i = 0; i < connections.size(); i++) {
    // If the from node index has changed:
    if (connections[i].from != fromNodeIndex) {
      fromNodeIndex = connections[i].from;
      
      // Add the bias to the current node value and apply activation
      values[fromNodeIndex] += nodes[fromNodeIndex].bias;
      nodes[fromNodeIndex].activate(values[fromNodeIndex]);
    }

    // Calculate the value of the output node if it is enabled
    if (connections[i].enabled) {
      values[connections[i].to] += values[fromNodeIndex] * connections[i].weight;
    }
  }
  
  // Add bias, apply the activation function and store the Output node values
  for (int i = nodes.size() - outputNodes; i < nodes.size(); i++) {
    values[i] += nodes[i].bias;
    nodes[i].activate(values[i]);
  }
};


// Serialization
std::vector<uint8_t> Brain::serialize() const {
  size_t amountOfNodes = nodes.size();
  size_t amountOfConnections = connections.size();
  std::vector<uint8_t> serialized(2 * sizeof(size_t) + amountOfNodes * sizeof(node) + amountOfConnections * sizeof(connection));

  // Copy the node- and connection sizes to the serialized vector
  memcpy(serialized.data(), &amountOfNodes, sizeof(size_t));
  memcpy(serialized.data() + sizeof(size_t), &amountOfConnections, sizeof(size_t));

  // Copy the nodes and connections to the serialized vector
  memcpy(serialized.data() + 2 * sizeof(size_t), nodes.data(), amountOfNodes * sizeof(node));
  memcpy(serialized.data() + 2 * sizeof(size_t) + amountOfNodes * sizeof(node), connections.data(), amountOfConnections * sizeof(connection));

  return serialized;
}
void Brain::deserialize(std::vector<uint8_t> serialized) {
  size_t amountOfNodes, amountOfConnections;
  memcpy(&amountOfNodes, serialized.data(), sizeof(size_t));
  memcpy(&amountOfConnections, serialized.data() + sizeof(size_t), sizeof(size_t));

  nodes.resize(amountOfNodes);
  connections.resize(amountOfConnections);

  // Codeium CLAIMS memcpy can be a cause of undefined behavior if structs have constructors: Consider changing this to use std::copy (sugested by Codeium) WHEN this claim turns out to be true
  memcpy(nodes.data(), serialized.data() + 2 * sizeof(size_t), amountOfNodes * sizeof(node));
  memcpy(connections.data(), serialized.data() + 2 * sizeof(size_t) + amountOfNodes * sizeof(node), amountOfConnections * sizeof(connection));
}


// Mutations
void Brain::mutate() {
  // Determine mutation type
  int mutationType = randomGenerator->randomInt(0, 3);
  if (mutationType == 0) {
    mutateStructure();
  } else {
    mutateValues();
  }
}
void Brain::mutateStructure() {
  int mutationType = randomGenerator->randomInt(0, 1);
  if (mutationType == 0) {
    addNode(randomGenerator->randomInt(0, connections.size()-1));
  } else {
    addConnection(randomGenerator->randomDouble(-1.0, 1.0));
  }
}
void Brain::mutateValues() {
  int changeValue = rand() % 100;
  if (changeValue < 20) {
    nodes[rand() % nodes.size()].randomActivation();
  } else if (changeValue < 40) {
    switch (rand() % 2) {
    case 0:
      nodes[rand() % nodes.size()].bias = randomGenerator->randomDouble(-1.0, 1.0);
      break;
    case 1:
      connections[rand() % connections.size()].weight = randomGenerator->randomDouble(-1.0, 1.0);
      break;
    }
  } else {
    switch (rand() % 2) {
    case 0:
      nodes[rand() % nodes.size()].adjustBias();
      break;
    case 1:
      connections[rand() % connections.size()].adjustWeight();
      break;
    }
  }
}
void Brain::addNode(std::optional<uint> connIndex) {
  uint connectionIndex;
  
  // If connectionIndex is given
  if (connIndex.has_value()) {
    connectionIndex = connIndex.value();
    // Check if the connection index given is valid
    if (connectionIndex >= connections.size()) {
      std::cout << "ERROR: " << connectionIndex << " " << connections.size() << "\n";
      throw std::out_of_range("Invalid connection index at Brain::addNode()");
    }
  } else {
    connectionIndex = rand() % connections.size();
  }

  // Create a copy of the connection and give it a weight of 1
  connection newConnection = connections[connectionIndex];
  connections[connectionIndex].weight = 1;

  // Increment every from and to which is greater than from at connectionIndex
  for (int i = 0; i < connections.size(); i++) {
    if (connections[i].from > newConnection.from) {
      connections[i].from++;
    }
    if (connections[i].to > newConnection.from) {
      connections[i].to++;
    }
  }
  
  // Set from at connectionIndex to to of newConnection
  connections[connectionIndex].from = newConnection.to;

  // Add a new connection where connectionIndex was
  connections.push_back(newConnection);

  // Sort the connections by from
  std::sort(connections.begin(), connections.end(), [](connection a, connection b) {
    return a.from < b.from;
  });

  // Add node
  node newNode(nodes.size(), nodeTypes::HIDDEN, randomGenerator->randomDouble(-1.0, 1.0), activationTypes::TANH);
  nodes.push_back(newNode);  

  // Swap the new node back until it's index is equal to from at connectionIndex
  int i = nodes.size() - 1;
  while (i > 0 && connections[connectionIndex].to != i) {
    std::swap(nodes[i], nodes[i-1]);
    i--;
  }
}
void Brain::addConn(connection newConn) {
  // Check if the connection details are valid
  if (newConn.from == newConn.to) {
    std::cout << "ERROR: " << newConn.from << " " << newConn.to << " " << connections.size() << "\n";
    throw std::out_of_range("Invalid inputs at Brain::addConn()");
  } else if (newConn.from >= nodes.size() || newConn.to >= nodes.size()) {
    std::cout << "ERROR: " << newConn.from << " " << newConn.to << " " << connections.size() << "\n";
    throw std::out_of_range("Invalid inputs at Brain::addConn()");
  }

  // Check if from is greater than to and swap
  if (newConn.from > newConn.to) {
    std::swap(newConn.from, newConn.to);    
  }

  // Check 20 times if the connection already exists
  bool unique = false;
  for (int i = 0; i < 20; i++) {
    // Check if the connection already exists
    unique = true;
    for (int j = 0; j < connections.size(); j++) {
      if (connections[j].from == newConn.from && connections[j].to == newConn.to) {
        unique = false;
        break;
      }
    }

    // If it doesn't exist, break
    if (!unique) {
      // Generate a new connection pair
      newConn.from = randomGenerator->randomUInt(0, nodes.size() - 1);

      if (newConn.from < inputNodes) {
        // from is an input node
        newConn.to = randomGenerator->randomUInt(inputNodes, nodes.size() - 1);
      } else if (newConn.from >= nodes.size() - outputNodes) {
        // from is an output node
        newConn.to = randomGenerator->randomUInt(0, nodes.size() - outputNodes - 1);
      } else {
        // from is a hidden node
        newConn.to = randomGenerator->randomUInt(0, nodes.size() - 2);
      }      
      
      // Check if from is greater than to and swap
      if (newConn.from > newConn.to) {
        std::swap(newConn.from, newConn.to);
      } else if (newConn.to == newConn.from) {
        newConn.to++;
      }
    } else {
      break;
    }
  }

  // Define the weight
  if (std::isnan(newConn.weight)) {
    newConn.weight = randomGenerator->randomDouble(-1.0, 1.0);
  }

  // Add the connection
  newConn.id = connections.size();
  connections.push_back(newConn);

  // Sort the connections by from
  std::sort(connections.begin(), connections.end(), [](connection a, connection b) {
    return a.from < b.from;
  });
}
void Brain::addConnection(std::optional<double> weight, std::optional<uint> in, std::optional<uint> out) {
  uint from, to;
  double newWeight = weight.value_or(randomGenerator->randomDouble(-1.0, 1.0));

  if (in.has_value() && out.has_value()) {
    // If connection details are given assign them
    from = in.value();
    to = out.value();
    
    // Check if the connection details are valid
    if (from == to) {
      std::cout << "ERROR: " << from << " " << to << " " << connections.size() << "\n";
      throw std::out_of_range("Invalid inputs at Brain::addConnection()");
    }  
  } else if (!in.has_value() && !out.has_value()) {    
    // If connection details aren't given, create a random connection
    // Create random from and to
    from = rand() % nodes.size();

    if (from < inputNodes) {
      // from is an input node
      to = (rand() % (nodes.size() - inputNodes)) + inputNodes;
    } else if (from >= nodes.size() - outputNodes) {
      // from is an output node
      to = rand() % (nodes.size() - outputNodes);
    } else {
      // from is a hidden node
      to = rand() % (nodes.size() - 1);
    }

    // Check if from and to are the same
    if (to == from) {
      to++;
    }
  } else {
    std::cout << "ERROR: " << from << " " << to << " " << connections.size() << "\n";
    throw std::out_of_range("Invalid inputs at Brain::addConnection()");
  }

  // Swap the nodes if from is greater than to
  if (from > to) {
    std::swap(from, to);
  }

  // Check if the connection already exists and mutate a value if it does
  for (int i = 0; i < connections.size(); i++) {
    if (connections[i].from == from && connections[i].to == to) {
      //std::cout << "Connection already exists at Brain::addConnection(): returning\n";
      mutateValues();
      return;
    }
  }

  // Create the new connection and add it
  connection newConnection(connections.size(), ConnectionType::STANDARD, from, to, newWeight);
  newConnection.enabled = true;
  connections.push_back(newConnection);

  // Sort the connections by from (replaces faulty code block below untill fixed)
  std::sort(connections.begin(), connections.end(), [](connection a, connection b) {
    return a.from < b.from;
  });

  // Doesn't work, will attempt to fix when optimizing
    // Move back the new connection so that from is sorted
    //int i = connections.size() - 1;
    //while (i > 0 && connections[i].from < connections[i-1].from) {
    //  std::swap(connections[i], connections[i-1]);
    //  i--;
    //}
}
void Brain::crossover(Brain parent2) {
  // Crossover the nodes
  for (int i = 0; i < nodes.size(); i++) {
    if (rand() % 2 == 0) {
      nodes[i] = parent2.nodes[i];
    }
  }
  // Crossover the connections
  for (int i = 0; i < connections.size(); i++) {
    if (rand() % 2 == 0) {
      connections[i] = parent2.connections[i];
    }
  }
}


// Setters
void Brain::reset() {
  // Set values to 0
  for (uint i = 0; i < values.size(); i++) {
    values[i] = 0.0;
  }
}
void Brain::setInputs(std::vector<double> inputs) {
  reset();
  // Set the input values
  for (uint i = 0; i < inputNodes; i++) {
    values[i] = inputs[i];
  }
}
void Brain::setValue(uint index, double value) {
  values[index] = value;
}


// Getters
uint Brain::numNodes() {
  return nodes.size();
}
uint Brain::numConnections() {
  return connections.size();
}
double Brain::getValue(uint index) {
  return values[index];
}
double Brain::getOutput(uint index) {
  return values[index + nodes.size() - outputNodes];
}
std::string Brain::toString() {
  std::string str = "";

  for (int i = 0; i < nodes.size(); i++) {
    str += std::to_string(nodes[i].bias) + " ";
  }

  return str;
};


// Printers
void Brain::print() const {
  std::cout << "Nodes: " << nodes.size() << "\nIndex\tNode_data\n";
  for (int i = 0; i < nodes.size(); i++) {
    std::cout << i << "\t" << nodes[i].toString() << "\n";
  }
  std::cout << "\n===============================================================================================================\n";
  std::cout << "Connections: " << connections.size() << "\nEnabled\tIn\tWeight\tOut\n";
  for (int i = 0; i < connections.size(); i++) {
    std::cout << connections[i].toString() << "\n";
  }
}
void Brain::printValues() const {
  std::cout << "Values: " << values.size() << "\nIndex\tValue\n";
  for (int i = 0; i < values.size(); i++) {
    std::cout << i << "\t" << values[i] << "\n";
  }
}
