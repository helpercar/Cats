#include "../include/CatSprite.hpp"
#include <algorithm>
#include <stdexcept>

SpritePart& CatSprite::getPart(const std::string& name) {
    auto it = shapes.find(name);
    if (it != shapes.end()) {
        return *(it->second);
    }
    throw std::runtime_error("Part named '" + name + "' not found in CatSprite");
}

std::unique_ptr<sf::Shape> CatSprite::createShape(sf::Vector2f dimensions, Options opt) {
    std::unique_ptr<sf::Shape> shape;
    if (opt.type == ShapeType::Rectangle) {
        shape = std::make_unique<sf::RectangleShape>(dimensions);
    } else {
        if (opt.vertexes != 0) {
            shape = std::make_unique<sf::CircleShape>(dimensions.x, opt.vertexes);
        } else {
            shape = std::make_unique<sf::CircleShape>(dimensions.x);
        }
    }

    shape->setScale({opt.scalex, opt.scaley});

    if (opt.originx == 0.f && opt.originy == 0.f) {
        sf::FloatRect bounds = shape->getLocalBounds();
        shape->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    } else {
        shape->setOrigin({opt.originx, opt.originy});
    }

    shape->setFillColor(opt.color);
    return shape;
}

void CatSprite::attachPart(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle, int zIndex) {
    auto partPtr = std::make_unique<SpritePart>(std::move(shape), zIndex);
    partPtr->setPosition(localOffset);
    partPtr->setRotation(sf::degrees(initialAngle));
    
    shapes[name] = std::move(partPtr);
}

void CatSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Combine global root world position
    states.transform *= getTransform();

    // Build a flat render queue vector
    std::vector<SpritePart::RenderItem> flatRenderQueue;
    for (const auto& [name, partPtr] : shapes) {
        partPtr->collectRenderItems(flatRenderQueue, states.transform);
    }

    // Perform a master stable sort across all parts based on Z-Index
    std::stable_sort(flatRenderQueue.begin(), flatRenderQueue.end(), 
        [](const SpritePart::RenderItem& a, const SpritePart::RenderItem& b) {
            return a.zIndex < b.zIndex;
        }
    );

    // Loop over the sorted list to draw shapes back-to-front
    for (const auto& item : flatRenderQueue) {
        sf::RenderStates itemStates;
        itemStates.transform = item.transform; // Set calculated absolute transform matrix
        target.draw(*item.shape, itemStates);
    }
}

CatSprite::CatSprite() {
    this->centerX = 50;
    this->centerY = 50;
}

CatSprite::CatSprite(int x, int y) {
    this->centerX = x;
    this->centerY = y;
    this->setPosition({static_cast<float>(x), static_cast<float>(y)});

    // Torso, Center Layer
    auto torso = createShape({60.f, 30.f}, {.type = ShapeType::Rectangle, .color = sf::Color(155, 155, 155)});
    attachPart("torso", std::move(torso), {0.f, 0.f}, 0.f, 0);

    // Head, Layer 5
    auto head = createShape({20.f, 20.f}, {.type = ShapeType::Circle, .color = sf::Color(240, 0, 0)});
    attachPart("head", std::move(head), {30.f, -30.f}, 0.f, 5);

    // Left Leg Branches, Layers -5, -4
    auto frontLeftTopLeg = createShape({5.f, 5.f}, {.type = ShapeType::Circle, .color = sf::Color(120, 120, 120), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f});
    attachPart("frontlefttopleg", std::move(frontLeftTopLeg), {20.f, 15.f}, 0.f, -5);

    auto frontLeftBottomLeg = createShape({5.f, 5.f}, {.type = ShapeType::Circle, .color = sf::Color(120, 120, 120), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f});
    getPart("frontlefttopleg").attachChild("frontleftbottomleg", std::move(frontLeftBottomLeg), {0.f, 15.f}, 0.f, -5);

    // Right Leg Branches, Layers -5, -4
    auto frontRightTopLeg = createShape({5.f, 5.f}, {.type = ShapeType::Circle, .color = sf::Color(140, 140, 140), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f});
    attachPart("frontrighttopleg", std::move(frontRightTopLeg), {-20.f, 15.f}, 0.f, 5);

    auto frontRightBottomLeg = createShape({5.f, 5.f}, {.type = ShapeType::Circle, .color = sf::Color(140, 140, 140), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f});
    getPart("frontrighttopleg").attachChild("frontrightbottomleg", std::move(frontRightBottomLeg), {0.f, 15.f}, 0.f, 5);
}
