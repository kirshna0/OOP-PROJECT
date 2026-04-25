#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <optional>
#include <string>

// Abstract Base Class demonstrating Abstraction
class Entity {
protected:
    sf::Vector2f position;
    int health;
    int maxHealth;
    float speed;
    std::string name;
    sf::Texture texture;
    // FIX (SFML 3): sf::Sprite must be constructed with a valid texture.
    // std::optional lets derived classes defer sprite creation until after
    // they have loaded their texture, eliminating the IntelliSense error.
    std::optional<sf::Sprite> sprite;

    // Call this from derived-class constructors AFTER loading texture.
    void initSprite();

public:
    Entity(std::string name, float x, float y, int hp, float spd);
    virtual ~Entity() = default;

    // Pure virtual functions for polymorphism
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    void takeDamage(int amount);
    void heal(int amount) { health += amount; if (health > maxHealth) health = maxHealth; }
    void setColor(sf::Color color) { if (sprite) sprite->setColor(color); }

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);

    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;

    std::string getName() const;
    sf::FloatRect getBounds() const;
};

#endif // ENTITY_H
