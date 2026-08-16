#pragma once

#include "./Stats.hpp"
#include "./CatSprite.hpp"
#include <iostream>

class Cat : public sf::Drawable, public sf::Transformable {
public:
    Cat();
    Cat(int x, int y);
    Cat(int level, std::string name, int x, int y);
    Cat(
        int strength, int dexterity, int stamina,
         int intelligence, int wits, int resolve,
        int presence, int manipulation, int composure, int level, std::string name,
        int x, int y
    );

    void print_cat_info();

    Stats* stats;
    std::string name = "Bob";   // Fallback name
    CatSprite sprite;

private:
    void level_up();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    // Function to update a Cat's Stats, use the bitmask to see what values are to be updated, then use the values within "values" to update
    void update_stats(unsigned int bitmask, std::vector<int> values);
protected:

};