#include "../include/SpritePart.hpp"

SpritePart::SpritePart(std::unique_ptr<sf::Shape> shape, int zIndex, sf::Vector2f origin) 
    : m_shape(std::move(shape)), m_zIndex(zIndex) {
    this->setOrigin(origin); 
}

sf::Shape& SpritePart::getShape() { 
    return *m_shape; 
}

int SpritePart::getZIndex() const { 
    return m_zIndex; 
}

void SpritePart::attachChild(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle, int zIndex, Options opt) {
    sf::Vector2f origin{opt.originx, opt.originy};
    
    if (origin.x == 0.f && origin.y == 0.f) {
        sf::FloatRect bounds = shape->getLocalBounds();
        origin = {bounds.size.x / 2.f, bounds.size.y / 2.f};
    }

    auto childPtr = std::make_unique<SpritePart>(std::move(shape), zIndex, origin);
    childPtr->setPosition(localOffset);
    childPtr->setRotation(sf::degrees(initialAngle));
    
    m_children[name] = std::move(childPtr);
}


SpritePart& SpritePart::getChild(const std::string& name) {
    auto it = m_children.find(name);
    if (it != m_children.end()) {
        return *(it->second);
    }
    
    throw std::runtime_error("Child part named '" + name + "' not found");
}

void SpritePart::collectRenderItems(std::vector<RenderItem>& queue, sf::Transform parentTransform) const {
    // Combine parent matrix with local matrix
    sf::Transform globalTransform = parentTransform * getTransform();

    // Pack the raw shape pointer along with its true global position matrix
    queue.push_back({ m_shape.get(), globalTransform, m_zIndex });

    // Recursively pass the combined matrix state down to collect children
    for (const auto& [name, childPtr] : m_children) {
        childPtr->collectRenderItems(queue, globalTransform);
    }
}
