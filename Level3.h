#ifndef LEVEL3_H
#define LEVEL3_H

#include "Level.h"
#include <vector>
#include <memory>

class Level3 : public Level {
public:
    Level3(Game* g);

    void init() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};

#endif // LEVEL3_H
