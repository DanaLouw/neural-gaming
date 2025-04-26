// Dana Louw 2025-01-09

#ifndef Connection_H
#define Connection_H


#include <cstdint>
#include <string>
#include <optional>
#include <sys/types.h>

#include "../../../lib/random/Random.hpp"

// External random generator
extern RandomGenerator* randomGenerator;

enum class ConnectionType : uint8_t {
  STANDARD,
  RECURRENT,
  BIDIRECTIONAL,
  Count
};

struct connection {
  uint32_t id;
  ConnectionType type;
  bool enabled;

  uint32_t from;
  uint32_t to;
  double weight;
  // Why not 64 bit node and/or connection ids? Because 64 is verry large:
  //    not even the human brain has 2^64 connections (much less neurons), much less a neural net which plays simple games
  //    even 32 bits is verry large for a neural net but I don't know if 16 bits are enough, so 32 bits it is!

  // Constructors
  connection();
  connection(uint32_t id, ConnectionType type, uint32_t from, uint32_t to, double weight);

  // Operators
  bool operator==(const connection &other) const;

  // Mutators
  void adjustWeight(std::optional<double> amount = std::nullopt);

  // Getters
  std::string toString() const;
};

#endif
