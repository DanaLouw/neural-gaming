// Dana Louw 2025-01-09

// cd ~/Dev/C++/machineLearning/RL-NEAT/latest
// ./'build&run.sh' || scripts/'build&run.sh'

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
//#include <cstdlib>
//#include <ctime>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <sqlite3.h>
#include <vector>

#include "src/games/CTF/entities/Flagpole.hpp"
#include "src/sql.cpp"

#include "src/games/CTF/entities/agents/Agent.hpp"
#include "src/guiEvents/events.hpp"


bool isRunning = true;
//const uint8_t CORES = 1;
const unsigned int POPULATION_SIZE = 1024;
const unsigned int GENERATIONS = 4096 * 8; // 4096, 2048, 1024
const unsigned int TIME_PER_GENERATION = 60;
const unsigned int SAVE_INTERVAL = 2048;

void trainAI(sqlite3* DB, bool* doneTraining, std::vector<NEATAgent>* topPerformers) {
  int training_seed = time(0);
  srand(training_seed);
  
  // Create Agents
  std::vector<NEATAgent> agents;
  for (int i = 0; i < POPULATION_SIZE; i++) {
    agents.push_back(NEATAgent());
  };

  // Create the next generation
  std::vector<NEATAgent> nextGeneration;
  for (int i = 0; i < POPULATION_SIZE; i++) {
    nextGeneration.push_back(NEATAgent());
  };

  // Main loop
  std::cout << "Starting training\n";
  for (int currentGeneration = 0; currentGeneration < GENERATIONS; currentGeneration++) {

    // Set the agents to the next generation and reset them
    int goal1 = rand() % 160 - 80;
    int goal2 = rand() % 160 - 80;

    for (int i = 0; i < POPULATION_SIZE; i++) {
      agents[i] = nextGeneration[i];
      agents[i].reset();

      for (int j = 0; j < TIME_PER_GENERATION; j++) {
        agents[j].update();
      }

      agents[i].fitness(std::vector<Flagpole> {Flagpole(0, {0, 0}), Flagpole(1, {0, 0})});
    }

    // Sort the agents by fitness from highest to lowest
    std::sort(agents.begin(), agents.end(), [](NEATAgent a, NEATAgent b) {
      return a.getFitness() > b.getFitness();
    });

    std::cout << "Generation: " << currentGeneration << "\n";

    // Copy the top 30% of the agents to the next generation unchanged
    for (int i = 0; i < round(POPULATION_SIZE * 0.3); i++) {
      nextGeneration[i] = agents[i];
    }

    // Copy a random agent of the bottom 70% of the agents and mutate it
    for (int i = round(POPULATION_SIZE * 0.3) + 1; i < POPULATION_SIZE; i++) {
      nextGeneration[i] = agents[rand() % POPULATION_SIZE];
      nextGeneration[i].genome.mutate();
    }

    // Save the best agent to the database
    if ((currentGeneration + 1) % SAVE_INTERVAL == 0) {
      topPerformers->push_back(nextGeneration[0]);
      StoreSerialToDatabase(DB, nextGeneration[0].genome.serialize());
      std::cout << "Fitness scores: ";
      for (int i = 0; i < POPULATION_SIZE; i++) {
        std::cout << agents[i].getFitness() << " ";
      }
      std::cout << "\n";
    }

    if (isRunning == false) {
      return;
    } 
  }

  //std::cout << "Best agent:\n";
  //nextGeneration[0].genome.print();

  *doneTraining = true;
}

void simulateTopPerformers(std::vector<NEATAgent> topPerformers, sf::RenderWindow& window) {
  
  
  for (int i = 0; i < topPerformers.size(); i++) {
    topPerformers[i].render(window);
  }
}


int main() {
  sqlite3* DB;
  char* messaggeError;
  bool doneTraining = false;
  std::vector<NEATAgent> topPerformers;

  // Open database
  if (sqlite3_open("./data/saves.sqlite3", &DB) != SQLITE_OK) {
    std::cout << "Unable to open database\n";
    return 1;
  } else {
    std::cout << "Opened database\n";
  }

  // Create table
  const std::string createSQLTable = "CREATE TABLE IF NOT EXISTS 'TopPerformers' ("
    "'ID'	INTEGER NOT NULL UNIQUE,"
    "'Generation'	INTEGER,"
    "'Genome'	BLOB NOT NULL,"
    //"'Name'	TEXT,"
    "PRIMARY KEY('ID' AUTOINCREMENT)"
  ");"
  ;

  sqlite3_exec(DB, createSQLTable.c_str(), NULL, 0, &messaggeError);

  std::thread t1(trainAI, DB, &doneTraining, &topPerformers);


  // GUI
  // Create the window
  sf::RenderWindow window(sf::VideoMode(1920u, 1080u),"RL-NEAT");//, sf::Style::Fullscreen);
  window.setFramerateLimit(30);
  
  // Point 0, 0:
  sf::CircleShape zero;
  zero.setRadius(2.5f);
  zero.setOrigin(2.5f, 2.5f);
  zero.setFillColor(sf::Color{0, 0, 255, 255});
  zero.setPosition(950, 540);

  // Font
  sf::Font font;
  if (!font.loadFromFile("./data/Font.ttf")) {
    std::cout << "Unable to load font\n";
    return 1;
  }

  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::White);
  text.setPosition(10, 10);
  text.setString("Hello world");

  while (window.isOpen()) {

    // Events
    processEvents(window, text);

    // Draw
    window.clear();

    window.draw(text);
    window.draw(zero);
    
    window.display();

    if (doneTraining == true) {
      std::cout << "Done training\n";
      break;
    }
  }

  isRunning = false;
  t1.join();

  std::cin.get();

  // Simulate top performers
  simulateTopPerformers(topPerformers, window);
  
  // Close the window and database
  window.close();
  sqlite3_close(DB);

  return 0;
}