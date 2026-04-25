#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Spell.h"
#include <vector>
#include <memory>

class Player : public Entity {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int activeSpellIndex;

public:
    Player(float x, float y);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    void castSpell(sf::Vector2f targetDir);
    void addSpell(std::unique_ptr<Spell> spell);
    void switchSpell();

    std::vector<std::unique_ptr<Spell>>& getActiveSpells();
    std::string getCurrentSpellName() const;

    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;
    void handleEvent(const sf::Event& event);
};

#endif // PLAYER_H
