// Dana Louw 2025-03-24

#include "Environment.hpp"
#include "../../../../lib/random/Random.hpp"
#include <cstdlib>
#include <iostream>

void Environment::update() {
  // Update flagpoles
  for (Flagpole& Flagpole : flagpoles) {
    Flagpole.update();
  }

  // Simulate agent's thoughts
  for (NEATAgent& agent : agents) {
    agent.prepareBrain(flagpoles);
  }
  // Update agents based on their thoughts
  for (NEATAgent& agent : agents) {
    agent.update();
  }

  // Handle capture events for agents who are close enought to a flagpole
  for (NEATAgent& agent : agents) {
    if (std::sqrt(std::pow(agent.position.x - flagpoles[0].position.x, 2) + std::pow(agent.position.y - flagpoles[0].position.y, 2)) < 5) {
      agent.flagCaptured = false;
      agent.flagsCaptured++;
    } else if (std::sqrt(std::pow(agent.position.x - flagpoles[1].position.x, 2) + std::pow(agent.position.y - flagpoles[1].position.y, 2)) < 5) {
      agent.flagCaptured = true;
    }
  }
}

void Environment::select() {
  // Calculate fitness
  for (NEATAgent& agent : agents) {
    agent.fitness(flagpoles);
  }

  // Sort agents by fitness from highest to lowest
  std::sort(agents.begin(), agents.end(), [](NEATAgent a, NEATAgent b) {
    return a.getFitness() > b.getFitness();
  });

  // Create the next generation
  std::vector<NEATAgent> nextGeneration;

  // Copy the top 30% of the agents to the next generation unchanged
  for (int i = 0; i < round(agents.size() * 0.3); i++) {
    nextGeneration.push_back(agents[i]);
  }

  // Copy a random agent of the bottom 70% of the agents and mutate it
  for (int i = round(agents.size() * 0.3) + 1; i < agents.size(); i++) {
    NEATAgent agent = agents[rand() % agents.size()];
    agent.mutate();
    nextGeneration.push_back(agent);
  }
}

// Subject to change
void Environment::reset() {
  // Reset flagpoles
  for (Flagpole& Flagpole : flagpoles) {
    Flagpole.flagCaptured = false;
    double xPos = randomGenerator->randomDouble(-double(dimensions.x >> 1), double(dimensions.x >> 1));
    double yPos = randomGenerator->randomDouble(-double(dimensions.y >> 1), double(dimensions.y >> 1));
    Flagpole.position = vector2d(xPos, yPos);

    // If the position is too close to 0,0 move it by a random amount between 1 and 51
    // This is to prevent undesired behavior when working with the moveTowards() function
    if (std::sqrt(std::pow(Flagpole.position.x, 2) + std::pow(Flagpole.position.y, 2)) < 1) {
      Flagpole.move(vector2d(rand() % 50 + 1, rand() % 50 + 1));
    }
    Flagpole.flagPosition = Flagpole.position;
  }
  // If the flagpoles are too close to each other, move one of them away in the direction of 0,0 by a random amount between 10 and 100
  if (std::sqrt(std::pow(flagpoles[0].position.x - flagpoles[1].position.x, 2) + std::pow(flagpoles[0].position.y - flagpoles[1].position.y, 2)) < 10) {
    flagpoles[1].moveTowards({0, 0}, rand() % 90 + 10);
    flagpoles[1].flagPosition = flagpoles[1].position;
  }

  // Reset agents
  agentStartPos.x = randomGenerator->randomDouble(-double(dimensions.x >> 1), double(dimensions.x >> 1));
  agentStartPos.y = randomGenerator->randomDouble(-double(dimensions.y >> 1), double(dimensions.y >> 1));
  bool flagCaptured = randomGenerator->randomDouble(0, 1) > 0.5;
  for (NEATAgent& agent : agents) {
    agent.reset();
    agent.position = agentStartPos;
    agent.flagCaptured = flagCaptured;
  }
  for (NEATAgent& agent : topPerformers) {
    agent.reset();
    agent.position = agentStartPos;
    agent.flagCaptured = flagCaptured;
  }
}
void Environment::run() {
  for (unsigned int i = 0; i < generations; i++) {
    // Age the agents
    for (auto& agent : agents) {
      agent.age++;
    }
    
    // Run the agents
    for (unsigned int j = 0; j < timePerGeneration; j++) {
      update();
    }
    // Commence the selection process
    select();

    // Save the top performers
    if (i % saveInterval == 0) {
      topPerformers.push_back(agents[0]);
    }

    if (i % progressInterval == 0) {
      float bestScore = 0.f, bestAverage = 0.f;
      for (auto& agent : agents) {
        if (agent.fitnessScore > bestScore) {
          bestScore = agent.fitnessScore;
        }
        if (agent.netFitness/agent.age > bestAverage) {
          bestAverage = agent.netFitness/agent.age;
        }
      }
      std::cout << "Generation " << i << ":\n";
      std::cout << "   Best score: " << bestScore << "\n";
      std::cout << "   Best average: " << bestAverage << "\n";
    }
    reset();
  }
  topPerformers.push_back(agents[0]);
  std::cout << "Done!\n";
}

void Environment::addAgent(NEATAgent agent) {
  agents.push_back(agent);
}
void Environment::addFlagpole(Flagpole flagpole) {
  flagpoles.push_back(flagpole);
}

void Environment::simulateTopPerformers(sf::RenderWindow& window) {
  // Set the color of the last agent to yellow to identify it among the others
  topPerformers.back().body.setFillColor(sf::Color{255, 255, 0, 255});
  // Create a sorted copy of the top performers
  std::sort(topPerformers.begin(), topPerformers.end(), [](NEATAgent a, NEATAgent b) {
    return a.getFitness() > b.getFitness();
  });

  reset();

  std::cout << "Simulating top performers:\n";
  std::cout << "Friendly flagpole: " << flagpoles[0].position.toString() << "\n";
  std::cout << "Enemy flagpole: " << flagpoles[1].position.toString() << "\n";
  std::cout << "Agent start position: " << agentStartPos.toString() << "\n";
  std::cout << "\n";


  // Simulate the top performers
  for (unsigned int i = 0; i < timePerGeneration; i++) {
    // Render the flagpoles
    for (Flagpole& flagpole : flagpoles) {
      flagpole.render(window);
    }

    for (NEATAgent& agent : topPerformers) {
      agent.prepareBrain(flagpoles);
    }
    for (NEATAgent& agent : topPerformers) {
      agent.update();
      //std::cout << agent.position.toString() << "\n";
      agent.render(window);
    }

    window.display();
    window.clear();
  }
  topPerformers.back().brain.printValues();
  // Set the color of the last agent back to green
  topPerformers.back().body.setFillColor(sf::Color{0, 255, 0, 255});
}