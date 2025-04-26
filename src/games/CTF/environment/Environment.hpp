// Dana Louw 2025-03-24

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>

#include "../entities/agents/NEATAgent.hpp"
#include "../entities/Flagpole.hpp"
//#include "../groups/Team.hpp"
//#include "tiles/Tile.hpp"

extern RandomGenerator* randomGenerator;

class Environment {
  public:
    std::vector<NEATAgent> agents;
    std::vector<Flagpole> flagpoles;
    std::vector<NEATAgent> topPerformers;

    vector2u dimensions = {1920u - 10u, 1080u - 10u};
    vector2d agentStartPos = {0, 0};

    unsigned int generations;
    unsigned int timePerGeneration;
    unsigned int saveInterval;
    unsigned int progressInterval;

    Environment() {};
    Environment(unsigned int width, unsigned int height);

    void addAgent(NEATAgent agent);
    void addFlagpole(Flagpole flagpole);
    void update();
    void reset();
    void run();
    void select();
    void simulateTopPerformers(sf::RenderWindow& window);
   
    // Getters
    unsigned int numTeams();
};

#endif