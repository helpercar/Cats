#pragma once
#include "../include/SpritePart.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

enum class ShapeType { 
    Circle, 
    Rectangle 
};

struct Options {
    ShapeType type = ShapeType::Circle; 
    sf::Color color = sf::Color(250, 0, 0);
    unsigned int vertexes = 0;
    float originx = 0.f;
    float originy = 0.f;
    float angle = 0.f;
    float scalex = 1.f;
    float scaley = 1.f;
};

class CatSprite : public sf::Drawable, public sf::Transformable {
public:
    CatSprite();
    CatSprite(int x, int y);

    int centerX;
    int centerY;

    std::unique_ptr<sf::Shape> createShape(sf::Vector2f dimensions, Options opt);
    void attachPart(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle = 0.f, int zIndex = 0);
    SpritePart& getPart(const std::string& name);

private:
    // Override Drawable with the current implementation of z layer drawing
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    std::unordered_map<std::string, std::unique_ptr<SpritePart>> shapes;
};
