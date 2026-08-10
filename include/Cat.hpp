#pragma once

#include "./Stats.hpp"
#include <iostream>

class Cat {
public:
    Cat();
    Cat(int level, std::string name);
    Cat(
        int strength, int dexterity, int stamina,
         int intelligence, int wits, int resolve,
        int presence, int manipulation, int composure, int level, std::string name
    );

    void print_cat_info();

    Stats* stats;
    std::string name = "Bob";   // Fallback name

private:
    void level_up();
    
    // Function to update a Cat's Stats, use the bitmask to see what values are to be updated, then use the values within "values" to update
    void update_stats(unsigned int bitmask, std::vector<int> values);
protected:

};