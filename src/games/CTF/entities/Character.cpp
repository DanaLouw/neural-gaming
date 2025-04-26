// Dana Louw 2025-03-27

#include <SFML/System/Vector2.hpp>
#include <iostream>

#include "Character.hpp"
//#include "Projectile.hpp"

void Character::move(vector2d direction) {
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

//void Character::hit(Projectile& projectile) {
  // hit by projectile
//}

void Character::hit(Character& character) {
  // hit by character  
}

void Character::render(sf::RenderWindow& window, float scale) {
  vector2u windowDimensions = vector2u(window.getSize().x, window.getSize().y);
  body.setPosition(position.x*scale + (windowDimensions.x >> 1), position.y*scale + (windowDimensions.y >> 1));
  window.draw(body);
}

// Setters
void Character::resetStats() {
  health = 0u;
  damageDealt = 0u;
  damageTaken = 0u;
  enemiesKilled = 0u;
  lastHitBy = nullptr;

  hasFriendFlag = false;
  hasEnemyFlag = false;
  flagsCaptured = 0u;
  flagsRetrieved = 0u;
  flagCaptured = false;
}

// Getters
unsigned int Character::getHealth() {
  return health;
}
unsigned int Character::getDamageDealt() {
  return damageDealt;
}
unsigned int Character::getDamageTaken() {
  return damageTaken;
}
unsigned int Character::getEnemiesKilled() {
  return enemiesKilled;
}
Character* Character::getLastHitBy() {
  return lastHitBy;
}

// Printers
void Character::printPosition() {
  std::cout << "Position: " << position.x << ", " << position.y << std::endl;
}
