// Dana Louw 2025-03-26

#ifndef FLAGPOLE_H
#define FLAGPOLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "Entity.hpp"

class Flagpole : public Entity {
  public:
    unsigned int id;
    bool flagCaptured = false;
    vector2d flagPosition;
  
    Flagpole() {}
    Flagpole(unsigned int id, vector2d position);
    void update() override {}
    void move(vector2d direction) override;
    void moveTowards(vector2d point, float magnitude);
};

#endif