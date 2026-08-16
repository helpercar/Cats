#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include <unordered_map>

struct Options {
    sf::Color color = sf::Color(250, 0, 0);
    int angle = 0;
    int vertexes = 0;
};

class CatSprite : public sf::Drawable, public sf::Transformable {
public:
    CatSprite();
    CatSprite(int x, int y);
    int centerX;
    int centerY;
    void createShape(int rad, int x, int y, std::string name, Options opt);

    // Required override to handle drawing the group
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Shape>> shapes;
protected:
};