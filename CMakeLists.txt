cmake_minimum_required(VERSION 3.25.0)
project(RL-NEAT VERSION 1.0.0)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
find_package(SQLite3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Main
  main.cpp
  #testing.cpp

  src/games/CTF/entities/agents/NEATAgent.cpp
  src/games/CTF/entities/Character.cpp
  src/games/CTF/entities/Entity.cpp
  src/games/CTF/entities/Flagpole.cpp
  src/games/CTF/environment/Environment.cpp
  src/games/CTF/environment/TrainingEnvironment.cpp

  src/guiEvents/events.cpp

  src/neuralNetworks/NEAT/Connection.cpp
  src/neuralNetworks/NEAT/Brain.cpp
  src/neuralNetworks/NEAT/Node.cpp

  lib/vector2D/Vector.cpp
  lib/vector2D/Vector.hpp
  lib/random/Random.cpp

  src/sql.cpp
)

# Link external libraries
target_link_libraries(Main
  #SFML
  sfml-graphics
  sfml-window
  sfml-system

  #SQLite3
  sqlite3
)
