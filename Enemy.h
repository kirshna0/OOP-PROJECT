#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Spell.h"
#include <memory>

class Game; // Forward declaration

class Enemy : public Entity {
private:
    Game* game;
    float attackTimer;
    float attackCooldown;
    int damage;
    float shootTimer;
    float shootCooldown;
    std::unique_ptr<Spell> magicSpell;

public:
    Enemy(Game* g, std::string name, float x, float y, int hp, float spd, int dmg = 10);
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
    
    Spell* getSpell() { return magicSpell.get(); }

    void moveTowards(sf::Vector2f targetPos, float deltaTime);
    bool canAttack();
    int getDamage() const;
};

#endif // ENEMY_H
