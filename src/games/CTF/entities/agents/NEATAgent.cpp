// Dana Louw 2025-01-06

#include "NEATAgent.hpp"
#include <cmath>
//#include <iostream>


// Constructors
NEATAgent::NEATAgent() {
  body = sf::CircleShape();
  body.setRadius(10.f);
  body.setOrigin(10.f, 10.f);
  body.setFillColor(sf::Color{0, 255, 0, 255});

  brain = Brain(inputNodes, outputNodes, 10);
};
NEATAgent::NEATAgent(unsigned int id) {
  body = sf::CircleShape();
  body.setRadius(5.f);
  body.setOrigin(body.getRadius(), body.getRadius());
  body.setFillColor(sf::Color{0, 255, 0, 255});

  brain = Brain(inputNodes, outputNodes, 10);
  this->id = id;
}


// Calculate fitness
void NEATAgent::fitness(std::vector<Flagpole> flagpoles) {
  fitnessScore = 1.f;
  fitnessScore *= flagsCaptured;
  double distance;
  if (flagCaptured) {
    fitnessScore *= 2;
    distance = std::sqrt(std::pow(position.x - flagpoles[0].position.x, 2) + std::pow(position.y - flagpoles[0].position.y, 2));
  } else {
    distance = std::sqrt(std::pow(position.x - flagpoles[1].position.x, 2) + std::pow(position.y - flagpoles[1].position.y, 2));
  }

  if (distance == 0.0) {
    distance = 1.0;
  }

  fitnessScore /= (distance);

  netFitness += fitnessScore;
};

// Run the agent
void NEATAgent::prepareBrain(std::vector<Flagpole> flagpoles) {  
  // Format inputs
  double hasFlag = 0.0;
  if (flagCaptured) {
    hasFlag = 1.0;
  }

  // Set inputs to the brain
  brain.setInputs({
    velocity.magnitude(),
    velocity.angle(),
    position.to(flagpoles[0].position).magnitude(),
    position.to(flagpoles[0].position).angle(),
    position.to(flagpoles[1].position).magnitude(),
    position.to(flagpoles[1].position).angle(),
    hasFlag
  });
}
void NEATAgent::update() {
  brain.calculate();
  
  // Calculate force
  PolarVector2D force = PolarVector2D(brain.getOutput(0), brain.getOutput(1)*M_PI);
  force.magnitude = std::clamp(force.magnitude,0.0, 2.0);
  force.magnitude /= mass;

  // Calculate velocity
  velocity += vector2d(force);

  // Calculate position
  //position += velocity;
  move(velocity);
}

// Change the agent
void NEATAgent::mutate() {
  netFitness = 0; age = 0;
  brain.mutate();
}

// Setters
void NEATAgent::reset() {
  fitnessScore = 0;
  velocity = {0., 0.};
  resetStats();
}
void NEATAgent::setBrain(Brain brain) {
  this->brain = brain;
}

// Getters
float NEATAgent::getFitness() {
  return fitnessScore;
}
float NEATAgent::getNetFitness() {
  return netFitness;
}
Brain NEATAgent::getBrain() {
  return brain;
}
