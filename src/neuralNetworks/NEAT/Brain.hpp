// Dana Louw 2025-01-08

#ifndef Brain_H
#define Brain_H

#include <optional>
#include <vector>

#include "Connection.hpp"
#include "Node.hpp"


class Brain {
  private:
    uint32_t inputNodes;
    uint32_t outputNodes;

    void addNode(std::optional<uint> connIndex = std::nullopt);
    void addConn(connection conn = connection());
    void addConnection(std::optional<double> weight = std::nullopt, std::optional<uint> in = std::nullopt, std::optional<uint> out = std::nullopt);
    void mutateStructure();
    void mutateValues();

    std::vector<node> nodes;
    std::vector<connection> connections;
    std::vector<double> values; // <values[nodeIndex]>

  public:
    Brain() {};
    Brain(uint32_t inputNodes, uint32_t outputNodes, uint numStartingConnections = 1);
    
    // Operations
    void calculate();
    void mutate();
    void crossover(Brain parent2);

    // Serialization
    std::vector<uint8_t> serialize() const;
    void deserialize(std::vector<uint8_t> serialized);

    // Setters
    void reset();
    void setInputs(std::vector<double> inputs);
    void setValue(uint index, double value);

    // Getters
    uint numNodes();
    uint numConnections();
    double getValue(uint index);
    double getOutput(uint index);
    node getNode(uint index);
    connection getConnection(uint index);
    std::string toString();

    // Printers
    void print() const;
    void printValues() const;
};


#endif