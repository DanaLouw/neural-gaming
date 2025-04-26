// Dana Louw 2025-01-06
// Adapted from https://github.com/Poppro/PyreNet/tree/master/src/random

#ifndef Random_Generator_H
#define Random_Generator_H

#include <random>
#include <mutex>
#include <sys/types.h>

class RandomGenerator {
private:
  static RandomGenerator* randomGenerator;
  static std::once_flag initInstanceFlag;

  static std::mutex instanceMutex;
  std::default_random_engine *generator;

  RandomGenerator();
  RandomGenerator(RandomGenerator const &);
  RandomGenerator& operator=(RandomGenerator const &);

public:
  static RandomGenerator* getInstance();

  double randomDouble(double lower, double upper);
  double randomGaussianDouble(double mean, double standardDeviation);
  int randomInt(int lower, int upper);
  uint randomUInt(uint lower, uint upper);
};


#endif