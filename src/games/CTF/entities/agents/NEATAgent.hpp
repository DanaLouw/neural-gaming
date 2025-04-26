// Dana Louw 2025-01-06

#ifndef AGENT_H
#define AGENT_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "../Character.hpp"
#include "../../../../neuralNetworks/NEAT/Brain.hpp"
#include "../Flagpole.hpp"

#include <vector>


// Struct of a single agent
class NEATAgent : public Character {
  private:
  
  public:
    Brain brain;

    float netFitness = 0.f;
    float fitnessScore = 0.f;
    unsigned int inputNodes = 7u;
    unsigned int outputNodes = 2u;
    unsigned int age = 0u;
    
    NEATAgent();
    NEATAgent(unsigned int id);

    void prepareBrain(std::vector<Flagpole> flagpoles);
    void update() override;
    void mutate();
    void fitness(std::vector<Flagpole> flagpoles);

    // Setters
    void reset();
    void setBrain(Brain brain);
    void setAge(unsigned int age);

    // Getters
    Brain getBrain();
    float getFitness();
    float getNetFitness();
    unsigned int getAge();
};

#endif