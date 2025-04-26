// Dana Louw 2025-04-04

#include "TrainingEnvironment.hpp"
#include <iostream>

TrainingEnvironment::TrainingEnvironment(unsigned int generations, unsigned int timePerGeneration, unsigned int saveInterval) {
  this->generations = generations;
  this->timePerGeneration = timePerGeneration;
  this->saveInterval = saveInterval;
}

void TrainingEnvironment::run() {
  std::cout << "Running training\n";
  for (unsigned int i = 0; i < generations; i++) {
    for (unsigned int j = 0; j < timePerGeneration; j++) {
      for (unsigned int k = 0; k < agents.size(); k++) {
        std::cout << "Generation: " << i << ", Time: " << j << "\n";
        agents[k].update();        
      }
    }
  }
}