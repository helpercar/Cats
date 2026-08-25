#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

class SpritePart : public sf::Transformable {
public:
    // Structure to hold data for the flat global sorting queue
    struct RenderItem {
        const sf::Shape* shape;
        sf::Transform transform;
        int zIndex;
    };

    SpritePart(std::unique_ptr<sf::Shape> shape, int zIndex = 0);
    
    sf::Shape& getShape();
    int getZIndex() const; 

    void attachChild(const std::string& name, std::unique_ptr<sf::Shape> shape, sf::Vector2f localOffset, float initialAngle = 0.f, int zIndex = 0);
    SpritePart& getChild(const std::string& name);

    // Recursively gathers this shape and all its children into a flat list
    void collectRenderItems(std::vector<RenderItem>& queue, sf::Transform parentTransform) const;

private:
    std::unique_ptr<sf::Shape> m_shape;
    int m_zIndex;
    
    // Stable pointer map container to prevent memory invalidation
    std::unordered_map<std::string, std::unique_ptr<SpritePart>> m_children;
};
