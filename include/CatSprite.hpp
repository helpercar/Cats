#pragma once
#include "../include/SpritePart.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

class CatSprite : public sf::Drawable, public sf::Transformable {
public:
    CatSprite();
    CatSprite(int x, int y);

    int centerX;
    int centerY;

    std::unique_ptr<sf::Shape> createShape(sf::Vector2f dimensions, Options opt);
    void attachPart(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle = 0.f, int zIndex = 0, Options opt = {});
    SpritePart& getPart(const std::string& name);

private:
    // Override Drawable with the current implementation of z layer drawing
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    std::unordered_map<std::string, std::unique_ptr<SpritePart>> shapes;
};
