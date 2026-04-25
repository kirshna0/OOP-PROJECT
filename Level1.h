#ifndef LEVEL1_H
#define LEVEL1_H

#include "Level.h"
#include <vector>
#include <memory>

class Level1 : public Level {
public:
    Level1(Game* g);

    void init() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LEVEL1_H
