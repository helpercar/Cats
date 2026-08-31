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
        shape = (opt.vertexes != 0) ? std::make_unique<sf::CircleShape>(dimensions.x, opt.vertexes) : std::make_unique<sf::CircleShape>(dimensions.x);
    }
    
    shape->setScale({opt.scalex, opt.scaley});
    shape->setFillColor(opt.color);

    return shape;
}

void CatSprite::attachPart(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle, int zIndex, Options opt) {
    sf::Vector2f origin{opt.originx, opt.originy};
    
    // If no custom origin was provided, automatically calculate the geometric center
    if (origin.x == 0.f && origin.y == 0.f) {
        sf::FloatRect bounds = shape->getLocalBounds();
        origin = {bounds.size.x / 2.f, bounds.size.y / 2.f};
    }

    auto partPtr = std::make_unique<SpritePart>(std::move(shape), zIndex, origin);
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

    // TODO: refactor function to make it cleaner,
    // I am realizing it is just a bunch of the same functionality over and over
    // So I should just put this somewhere else and pass the parameters into the function

    // Torso
    Options torsoOpt = {.type = ShapeType::Rectangle, .color = sf::Color(155, 155, 155)};
    auto torsoShape = createShape({60.f, 30.f}, torsoOpt);
    
    // Calculate torso center origin
    auto torsoPart = std::make_unique<SpritePart>(std::move(torsoShape), 0, sf::Vector2f{30.f, 15.f}); 
    SpritePart* torsoRef = torsoPart.get();
    shapes["torso"] = std::move(torsoPart);

    // Head
    Options headOpt = {.type = ShapeType::Circle, .color = sf::Color(240, 0, 0)};
    auto headShape = createShape({20.f, 20.f}, headOpt);
    
    // Attach head directly to torso reference
    torsoRef->attachChild("head", std::move(headShape), {60.f, -10.f}, 0.f, 5, headOpt);
    SpritePart& headRef = torsoRef->getChild("head");

    // Left Ear
    Options leftEarOpt = {.type = ShapeType::Circle, .color = sf::Color(155, 20, 155), .vertexes = 3};
    auto leftEar = createShape({12.f, 12.f}, leftEarOpt);
    headRef.attachChild("leftEar", std::move(leftEar), {2.f, 0.f}, 325.f, 6, leftEarOpt);

    // Right Ear
    Options rightEarOpt = {.type = ShapeType::Circle, .color = sf::Color(155, 20, 155), .vertexes = 3};
    auto rightEar = createShape({12.f, 12.f}, rightEarOpt);
    headRef.attachChild("rightEar", std::move(rightEar), {34.f, 0.f}, 45.f, 4, rightEarOpt);

    // Leg Shared Setup
    Options legOpt = {.type = ShapeType::Circle, .color = sf::Color(100, 100, 100), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f};
    Options rightLegOpt = {.type = ShapeType::Circle, .color = sf::Color(120, 120, 120), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f};

    // Front Left Leg
    auto frontLeftTopLeg = createShape({5.f, 5.f}, legOpt);
    attachPart("frontlefttopleg", std::move(frontLeftTopLeg), {24.f, 15.f}, 0.f, -4, legOpt);
    getPart("frontlefttopleg").attachChild("frontleftbottomleg", std::move(createShape({5.f, 5.f}, legOpt)), {0.f, 15.f}, 0.f, -5, legOpt);

    // Front Right Leg
    auto frontRightTopLeg = createShape({5.f, 5.f}, rightLegOpt);
    attachPart("frontrighttopleg", std::move(frontRightTopLeg), {18.f, 15.f}, 0.f, 4, rightLegOpt);
    getPart("frontrighttopleg").attachChild("frontrightbottomleg", std::move(createShape({5.f, 5.f}, rightLegOpt)), {0.f, 15.f}, 0.f, 5, rightLegOpt);

    // Back Left Leg
    auto backLeftTopLeg = createShape({5.f, 5.f}, legOpt);
    attachPart("backlefttopleg", std::move(backLeftTopLeg), {-18.f, 15.f}, 0.f, -4, legOpt);
    getPart("backlefttopleg").attachChild("backleftbottomleg", std::move(createShape({5.f, 5.f}, legOpt)), {0.f, 15.f}, 0.f, -5, legOpt);

    // Back Right Leg
    Options backRightLegOpt = {.type = ShapeType::Circle, .color = sf::Color(140, 140, 140), .originx = 2.5f, .originy = 2.5f, .scaley = 2.f};
    auto backRightTopLeg = createShape({5.f, 5.f}, backRightLegOpt);
    attachPart("backrighttopleg", std::move(backRightTopLeg), {-24.f, 15.f}, 0.f, 4, backRightLegOpt);
    getPart("backrighttopleg").attachChild("backrightbottomleg", std::move(createShape({5.f, 5.f}, backRightLegOpt)), {0.f, 15.f}, 0.f, 5, backRightLegOpt);
}


