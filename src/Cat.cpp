#include "../include/Cat.hpp"

Cat::Cat() {
    // Basic level 1 cat
    this->stats = new Stats();
    sprite = CatSprite(50, 50);
}

Cat::Cat(int x, int y) {
    // Basic level 1 cat
    this->stats = new Stats();
    sprite = CatSprite(x, y);
}

Cat::Cat(int level, std::string name, int x, int y) {
    // Cat created by level
    this->stats = new Stats(level);
    this->name = name;
    sprite = CatSprite(x, y);
}

Cat::Cat(
    // Cat with specific values (maybe a 500 intelligence cat or something)
    int strength, int dexterity, int stamina,
    int intelligence, int wits, int resolve,
    int presence, int manipulation, int composure,
    int level, std::string name,
    int x, int y
) {
    this->stats = new Stats(strength, dexterity, stamina, intelligence, wits, resolve, presence, manipulation, composure, level);
    printf("%d\n", level);
    printf("%d\n", this->stats->level);
    this->name = name;
    sprite = CatSprite(x, y);
}

void Cat::print_cat_info() {
    std::cout << "Name: " << this->name << std::endl;

    this->stats->print_stats();
}

void Cat::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Combine this parent entity's transform with incoming states
    states.transform *= getTransform();

    // Pass the combined transform states down to the inner ShapeGroup
    target.draw(sprite, states);
}
