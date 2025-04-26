// Dana Louw 2025-01-06

#include <cmath>
#include <cstdlib>
#include <chrono>
#include <random>
#include "Random.hpp"

double randd() {
  return (double)rand() / ((double)RAND_MAX + 1);
}

double randWeight() {
  // Random chance to be negative
  if ((rand() % 2) == 0) {return -randd();}
  else {return randd();}
}

double randBias() {
  // Random chance to be negative
  if ((rand() % 2) == 0) {return -randd();}
  else {return randd();}
}

double randDouble(double min, double max) {
  std::random_device rd;
  std::mt19937_64 gen(rd() + std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<> dis(min, max);
  return dis(gen);
}
