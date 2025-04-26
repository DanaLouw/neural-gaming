// Dana Louw 2025-04-04

#ifndef GLOBAL_ENTITY_H
#define GLOBAL_ENTITY_H

#include "../../../../lib/vector2D/Vector.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>

class Entity {
public:
  vector2d position;
  sf::CircleShape body;

  Entity() {}
  Entity(vector2d position);

  virtual void render(sf::RenderWindow& window, float scale = 1.f);
  virtual void update() = 0;
  virtual void move(vector2d direction) = 0;
};

#endif