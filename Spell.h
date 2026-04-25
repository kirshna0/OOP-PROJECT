#ifndef SPELL_H
#define SPELL_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <string>

// Abstract Base Class for Spells
class Spell {
protected:
    std::string name;
    int damage;
    float speed;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool active;
    sf::Texture texture;
    // FIX (SFML 3): Defer sprite construction until texture is loaded.
    std::optional<sf::Sprite> sprite;

    // Call after loading texture in derived-class constructor.
    void initSprite();

public:
    Spell(std::string name, int dmg, float spd);
    virtual ~Spell() = default;

    virtual void cast(sf::Vector2f startPos, sf::Vector2f direction) = 0;
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);

    bool isActive() const;
    void deactivate();
    void setDamage(int dmg) { damage = dmg; }
    void setActive(bool a) { active = a; }
    int getDamage() const;
    std::string getName() const { return name; }
    sf::FloatRect getBounds() const;
};

#endif // SPELL_H
