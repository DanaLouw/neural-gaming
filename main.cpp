// Dana Louw 2025-01-09

// cd ~/Dev/C++/machineLearning/RL-NEAT/latest
// ./'build&run.sh' || scripts/'build&run.sh'

#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sqlite3.h>

#include "src/games/CTF/environment/Environment.hpp"
#include "lib/random/Random.hpp"

// Constants
// 1024 2048 4096 8192 16384
const unsigned int POPULATION_SIZE = 1024;
const unsigned int TIME_PER_GENERATION = 60;
const unsigned int GENERATIONS = 1024;
const unsigned int SAVE_INTERVAL = 256;
const unsigned int PROGRESS_INTERVAL = 16;


int main() {
  sqlite3* DB;
  char* messaggeError;

  RandomGenerator *randomGenerator = RandomGenerator::getInstance();

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


  bool doneTraining = true;
  std::vector<NEATAgent> topPerformers;
  int training_seed = time(0);
  srand(training_seed);

  // Create the environment and add agents and flags
  Environment environment;
  environment.addFlagpole(Flagpole(0, {0, 0}));
  environment.addFlagpole(Flagpole(1, {0, 0}));

  environment.flagpoles[0].body.setFillColor(sf::Color{0, 0, 255, 255});
  environment.flagpoles[1].body.setFillColor(sf::Color{255, 0, 0, 255});

  environment.timePerGeneration = TIME_PER_GENERATION;
  environment.generations = GENERATIONS;
  environment.saveInterval = SAVE_INTERVAL;
  environment.progressInterval = PROGRESS_INTERVAL;

  for (unsigned int i = 0; i < POPULATION_SIZE; i++) {
    environment.addAgent(NEATAgent(i));
  }

  // Set the starting conditions
  environment.reset();

  std::cout << "Environment created\n";
  std::cout << "Running environment\n";

  environment.run();

  // GUI
  // Create the window
  //sf::RenderWindow window(sf::VideoMode(1920u, 1080u),"RL-NEAT");//, sf::Style::Fullscreen);
  //window.setFramerateLimit(30);
  
  // Point 0, 0:
  sf::CircleShape zero;
  zero.setRadius(2.5f);
  zero.setOrigin(2.5f, 2.5f);
  zero.setFillColor(sf::Color{0, 0, 255, 255});
  zero.setPosition(950, 540);

  // Font
  //sf::Font font;
  //if (!font.loadFromFile("./data/Font.ttf")) {
    //std::cout << "Unable to load font\n";
    //return 1;
  //}

  //sf::Text text;
  //text.setFont(font);
  //text.setCharacterSize(24);
  //text.setFillColor(sf::Color::White);
  //text.setPosition(10, 10);
  //text.setString("Hello world");

  //while (window.isOpen()) {

    // Events
    //processEvents(window, text);

    // Draw
    //window.clear();

    //window.draw(text);
    //window.draw(zero);
    
    //window.display();

    //if (doneTraining == true) {
      //std::cout << "Done training\n";
      //break;
    //}
  //}
  //window.close();
  
  std::cout << environment.topPerformers.size() << "\n";

  sf::RenderWindow window(sf::VideoMode(1920u, 1080u),"RL-NEAT");//, sf::Style::Fullscreen);
  //window.create(sf::VideoMode(1920u, 1080u),"RL-NEAT", sf::Style::Fullscreen);
  window.setFramerateLimit(30);

  // Simulate top performers
  bool doneSimulating = false;
  std::string commanlineInput;
  while ((window.isOpen()) && (!doneSimulating)) {
    std::cin >> commanlineInput;
    if (commanlineInput == "q") {
      std::cout << "quit\n";
      doneSimulating = true;
    } else  if (commanlineInput == "c") {
      std::cout << "continuing training\n";
      environment.run();
    } else if (commanlineInput == "r") {
      std::cout << "re-simulate:\n";
      environment.simulateTopPerformers(window);
    } else if (commanlineInput == "p") {
      std::cout << "print last top performer's brain\n";
      environment.topPerformers.back().brain.print();
    }
  }
  
  // Close the window and database
  window.close();
  sqlite3_close(DB);

  return 0;
}