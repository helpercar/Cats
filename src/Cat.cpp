#include "../include/Cat.hpp"

Cat::Cat() {
    // Basic level 1 cat
    this->stats = new Stats();
}

Cat::Cat(int level, std::string name) {
    // Cat created by level
    this->stats = new Stats(level);
    this->name = name;
}

Cat::Cat(
    // Cat with specific values (maybe a 500 intelligence cat or something)
    int strength, int dexterity, int stamina,
    int intelligence, int wits, int resolve,
    int presence, int manipulation, int composure,
    int level, std::string name
) {
    this->stats = new Stats(strength, dexterity, stamina, intelligence, wits, resolve, presence, manipulation, composure, level);
    printf("%d\n", level);
    printf("%d\n", this->stats->level);
    this->name = name;
}

void Cat::print_cat_info() {
    std::cout << "Name: " << this->name << std::endl;

    this->stats->print_stats();
}
