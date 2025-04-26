// Dana Louw 2025-03-27

#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

#include "Entity.hpp"

class Character : public Entity {
  private:    
    // Stats
    unsigned int health = 0u;
    unsigned int damageDealt = 0u;
    unsigned int damageTaken = 0u;
    unsigned int enemiesKilled = 0u;
    Character* lastHitBy = nullptr;

    // Flags
    bool hasFriendFlag = false;
    bool hasEnemyFlag = false;
  
  public:
    unsigned int id;
    
    // Physics
    vector2d velocity = {0., 0.};
    float mass = 1.f;
    
    // Flags
    unsigned int flagsCaptured = 0u;
    unsigned int flagsRetrieved = 0u;
    bool flagCaptured = false;

    unsigned int teamID;

    sf::CircleShape body;

    Character() {};
    void render(sf::RenderWindow& window, float scale = 1.f) override;

    void move(vector2d direction) override;

    // Collisions
    //void hit(Projectile& projectile);
    void hit(Character& character);

    void die();

    // Setters
    void resetStats();
    
    // Getters
    unsigned int getHealth();
    unsigned int getDamageDealt();
    unsigned int getDamageTaken();
    unsigned int getEnemiesKilled();
    Character* getLastHitBy();

    // Printers
    void printPosition();

    virtual ~Character() {}
};

#endif