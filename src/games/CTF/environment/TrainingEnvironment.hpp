// Dana Louw 2025-04-04

#ifndef TRAINING_ENVIRONMENT_H
#define TRAINING_ENVIRONMENT_H

#include "Environment.hpp"

class TrainingEnvironment : public Environment {
  public:
    unsigned int generations;
    unsigned int timePerGeneration;
    unsigned int saveInterval;

    TrainingEnvironment(unsigned int generations, unsigned int timePerGeneration, unsigned int saveInterval);
    void run();
};

#endif