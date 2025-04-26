// Dana Louw 2025-03-18

#include <cmath>

#include "Node.hpp"
#include "../../../lib/random/Random.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>

// Constructors
node::node() {
  this->id = 0;
  this->type = nodeTypes::HIDDEN;
  this->bias = 0.0;
  this->activation = activationTypes::LINEAR;
}
node::node(uint32_t id, nodeTypes type, double bias, activationTypes activation) {
  this->id = id;
  this->type = type;

  this->bias = bias;
  this->activation = activation;
}


double node::activate(double &value) {
  switch (activation) {
    case activationTypes::LINEAR:
      //do nothing
      break;
    case activationTypes::TANH:
      value = std::tanh(value);
      break;
    case activationTypes::SIGMOID:
      value = 1 / (1 + std::exp(-value));
      break;
    case activationTypes::RELU:
      value = std::max(0.0, value);
      break;
    case activationTypes::LRELU:
      // Leaky ReLU
      value = std::max(0.01 * value, value);
      break;
    case activationTypes::POWER:
      value = std::pow(value, 2);
      break;
    case activationTypes::SINE:
      value = std::sin(value);
      break;
    case activationTypes::COSINE:
      value = std::cos(value);
      break;
    case activationTypes::Count:
      break;
    }
  return value;
}


// Operators
bool node::operator==(const node &other) const {
  return id == other.id && type == other.type && bias == other.bias && activation == other.activation;
}


// Mutators
void node::adjustBias(double amount) {
  if (amount == 0.0) {
    amount = randomGenerator->randomDouble(-1.0, 1.0)/10;
  }
  this->bias = std::clamp(this->bias + amount, -1.0, 1.0);
}
void node::randomActivation() {
  if (this->type == nodeTypes::INPUT) {
    this->activation = activationTypes::LINEAR;
    return;
  }
  
  // Get a random activation type
  uint8_t newActivation = rand() % static_cast<uint8_t>(activationTypes::Count);
  if (activation == static_cast<activationTypes>(newActivation)) {
    newActivation++;
  }
  activation = static_cast<activationTypes>(newActivation);
}


// Getters
std::string node::toString() const {
  std::string type;
  switch (this->type) {
    case nodeTypes::INPUT:
      type = "INPUT";
      break;
    case nodeTypes::OUTPUT:
      type = "OUTPUT";
      break;
    case nodeTypes::HIDDEN:
      type = "HIDDEN";
      break;
    case nodeTypes::MEMMORY:
      type = "MEMMORY";
      break;
    case nodeTypes::NUMBER_OF_NODE_TYPES:
      break;
  }

  std::string activation;
  switch (this->activation) {
    case activationTypes::LINEAR:
      activation = "LINEAR";
      break;
    case activationTypes::TANH:
      activation = "TANH";
      break;
    case activationTypes::SIGMOID:
      activation = "SIGMOID";
      break;
    case activationTypes::RELU:
      activation = "RELU";
      break;
    case activationTypes::LRELU:
      activation = "LRELU";
      break;
    case activationTypes::POWER:
      activation = "POWER";
      break;
    case activationTypes::SINE:
      activation = "SINE";
      break;
    case activationTypes::COSINE:
      activation = "COSINE";
      break;
    case activationTypes::Count:
      break;
  }

  return std::to_string(id) + ": " + std::to_string(bias) + " " + type + " " + activation;
}
