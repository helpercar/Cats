#include "../include/CatSprite.hpp"
#include <iostream>
#include <memory>

void CatSprite::createShape(int rad, int x, int y, std::string name, Options opt) {
    std::unique_ptr<sf::CircleShape> shape;
    if (opt.vertexes != 0) {
        shape = std::make_unique<sf::CircleShape>(rad, opt.vertexes);
    }
    else {
        shape = std::make_unique<sf::CircleShape>(rad);
    }

    shape->setPosition({centerX + x, centerY + y});
    shape->setOrigin(sf::Vector2f(rad, rad));

    if (opt.angle != 0) {
        shape->setRotation(sf::degrees(opt.angle));
    }

    shape->setFillColor(opt.color);
    shapes[name] = std::move(shape);
}

CatSprite::CatSprite() {
    this->centerX = 50;
    this->centerY = 50;

    createShape(20, 50, 50, "head", {.color = sf::Color(240, 240, 240)});
    createShape(10, 33, 35, "leftear", {.angle = -45, .vertexes = 3});
    createShape(10, 67, 35, "rightear", {.angle = 45, .vertexes = 3});
}

CatSprite::CatSprite(int x, int y) {
    this->centerX = x;
    this->centerY = y;

    createShape(20, 50, 50, "head", {.color = sf::Color(240, 240, 240)});
    createShape(10, 33, 35, "leftear", {.angle = -45, .vertexes = 3});
    createShape(10, 67, 35, "rightear", {.angle = 45, .vertexes = 3});
}


void CatSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Combine the parent group's transform with the child shapes
    states.transform *= getTransform();

    // Draw each individual shape inside the group
    for (const auto& [key, shape] : shapes) {
        target.draw(*shape, states);
    }
}