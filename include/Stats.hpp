#pragma once

#include <string>
#include <random>
#include <random>
#include <cmath>
#include <unordered_map>

class Stats{
public:
    std::unordered_map<std::string, int> stats_map;
    // ACROSS: Power, Finess, Resistance
    // DOWN: Physical, Mental, Social
    // Based off World Of Darkness
    int max_hp;
    int level = 1;

    Stats();
    Stats(int level);
    Stats(
        int strength, int dexterity, int stamina,
         int intelligence, int wits, int resolve,
        int presence, int manipulation, int composure, int level
    );

    int generate_random_stat(int level);
    float generate_num(float limit, int count);
    void print_stats();
    void update_stats(uint16_t mask, std::vector<int> values);

private:
protected:
    
};