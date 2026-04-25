#ifndef LEVEL2_H
#define LEVEL2_H

#include "Level.h"
#include <vector>
#include <memory>

class Level2 : public Level {
public:
    Level2(Game* g);

    void init() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LEVEL2_H
