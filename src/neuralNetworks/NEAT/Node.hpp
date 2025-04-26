// Dana Louw 2025-01-09

#ifndef Node_H
#define Node_H

#include <cstdint>
#include <string>

#include "../../../lib/random/Random.hpp"

extern RandomGenerator* randomGenerator;

// Activation functions
enum class activationTypes : uint8_t {
  LINEAR,
  TANH,
  SIGMOID,
  RELU,
  LRELU,
  POWER,
  SINE,
  COSINE,
  //PRELU,
  //ELU,
  //SELU,
  //GELU,
  //SWISH,
  //SOFTPLUS,
  //SOFTSIGN,
  //SOFTMAX,
  //HARDTANH,
  //HARDSIGMOID,
  //HARDSWISH,
  //HARDSELU,
  //HARDLEAKYRELU,
  //GELU_NEW,
  //GELU_TANH,
  //GELU_ERF,
  //GELU_GAUSSIAN,
  //GELU_GELU,
  Count
};

// Node types
enum class nodeTypes : uint8_t {
  INPUT,
  OUTPUT,
  HIDDEN,
  MEMMORY,
  NUMBER_OF_NODE_TYPES
};

// Main node structure
struct node {
  uint32_t id;
  nodeTypes type;

  double bias;
  activationTypes activation;

  // Constructor
  node();
  node(uint32_t id, nodeTypes type, double bias, activationTypes activation);

  double activate(double &value);

  // Operators
  bool operator==(const node &other) const;

  // Mutators
  void adjustBias(double amount = 0.0);
  void randomActivation();

  // Getters
  std::string toString() const;
};

#endif
