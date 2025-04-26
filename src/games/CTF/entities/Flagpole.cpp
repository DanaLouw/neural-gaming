// Dana Louw 2025-03-26

#include "Flagpole.hpp"

Flagpole::Flagpole(unsigned int id, vector2d position)  {
  body.setRadius(10.f);
  body.setOrigin(body.getRadius(), body.getRadius());

  this->id = id;
  this->position = position;
  flagPosition = position;
}

void Flagpole::move(vector2d direction) {
  position += direction;
}

void Flagpole::moveTowards(vector2d point, float magnitude) {
  vector2<double> direction = position.to(point).normalized() * magnitude;
  move(direction);
}