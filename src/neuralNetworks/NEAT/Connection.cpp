// Dana Louw 2025-03-23

#include <algorithm>
//#include <bitset>
#include <cstdint>
#include <sys/types.h>

#include "Connection.hpp"
#include "../../../lib/random/Random.hpp"

RandomGenerator *randomGenerator = RandomGenerator::getInstance();

// Constructors
connection::connection() {
  this->type = ConnectionType::STANDARD;
  this->enabled = true;
  this->from = 0;
  this->to = 1;
  this->weight = 1.0/0;
}
connection::connection(uint32_t id, ConnectionType type, uint32_t from, uint32_t to, double weight) {
  this->type = type;
  this->enabled = true;
  this->from = from;
  this->to = to;
  this->weight = weight;
}


// Operators
bool connection::operator==(const connection &other) const {
  return id == other.id && type == other.type && enabled == other.enabled && from == other.from && to == other.to && weight == other.weight;
}


// Mutators
void connection::adjustWeight(std::optional<double> amount) {
  if (amount.has_value()) {
    weight = std::clamp(weight + amount.value(), -1.0, 1.0);
  } else {
    weight += randomGenerator->randomDouble(-1.0, 1.0)/10;
  }
}


// Getters
std::string connection::toString() const {
  return std::to_string(enabled) + ": " + std::to_string(from) + "~~~" + std::to_string(weight) + "~~>" + std::to_string(to);
}
