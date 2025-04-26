// Dana Louw 2025-04-25

#ifndef LINEAR_H
#define LINEAR_H

#include "activationFunction.hpp"

class Linear : public ActivationFunction {
public:
  double activate(double input) override {
    return input;
  }
};

#endif