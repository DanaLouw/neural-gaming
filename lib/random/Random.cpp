// Dana Louw 2025-01-06
// Adapted from https://github.com/Poppro/PyreNet/tree/master/src/random

#include "Random.hpp"

RandomGenerator *RandomGenerator::randomGenerator = nullptr;
std::mutex RandomGenerator::instanceMutex;

// Constructor
RandomGenerator::RandomGenerator() {
  std::chrono::time_point t = std::chrono::high_resolution_clock::now();
  std::default_random_engine *gen = new std::default_random_engine(t.time_since_epoch().count());
  this->generator = gen;
}

RandomGenerator* RandomGenerator::getInstance() {
  // Lock instanceMutex
  std::unique_lock<std::mutex> lg(instanceMutex);

  // If an instance doesn't exist, create ONE
  if (randomGenerator == nullptr)
    randomGenerator = new RandomGenerator();
  return randomGenerator;
}


// Random number generator
double RandomGenerator::randomDouble(double lower, double upper) {
  std::unique_lock<std::mutex> lg(instanceMutex);
  std::uniform_real_distribution<double> dist(lower, upper);
  return dist(*generator);
}

double RandomGenerator::randomGaussianDouble(double mean, double standardDeviation) {
  std::unique_lock<std::mutex> lg(instanceMutex);
  std::normal_distribution<double> dist(mean, standardDeviation);
  return dist(*generator);
}

int RandomGenerator::randomInt(int lower, int upper) {
  std::unique_lock<std::mutex> lg(instanceMutex);
  std::uniform_int_distribution<int> dist(lower, upper);
  return dist(*generator);
}

uint RandomGenerator::randomUInt(uint lower, uint upper) {
  std::unique_lock<std::mutex> lg(instanceMutex);
  std::uniform_int_distribution<uint> dist(lower, upper);
  return dist(*generator);
}
