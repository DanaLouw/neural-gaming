// Dana Louw 2025-04-04

#include "Entity.hpp"

Entity::Entity(vector2d position) {
  this->position = position;
}

void Entity::render(sf::RenderWindow& window, float scale) {
  vector2u windowDimensions = vector2u(window.getSize().x, window.getSize().y);
  body.setPosition(position.x*scale + (windowDimensions.x >> 1), position.y*scale + (windowDimensions.y >> 1));
  window.draw(body);
}

void Entity::move(vector2d direction) {
  position += direction;

  vector2u dimensions = vector2u(1920u, 1080u);

  if (position.x > float(dimensions.x >> 1)) {
    position.x = float(dimensions.x >> 1);
  }
  if (position.x < -float(dimensions.x >> 1)) {
    position.x = -float(dimensions.x >> 1);
  }
  if (position.y > float(dimensions.y >> 1)) {
    position.y = float(dimensions.y >> 1);
  }
  if (position.y < -float(dimensions.y >> 1)) {
    position.y = -float(dimensions.y >> 1);
  }

}