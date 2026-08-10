#include "../include/Stats.hpp"

float Stats::generate_num(float limit, int count) {
    std::random_device rd;
    std::mt19937 gen(rd()); 

    std::uniform_real_distribution<double> distrib(0, limit); 

    float val = 0;

    for (int i = 0; i < count; i++) {
        val += distrib(gen);
    }

    return val;
}

int Stats::generate_random_stat(int level) {
    if (level <= 0) {
        return 0;
    }

    std::random_device rd;
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<int> distrib(0, 1); 

    // Roll 3 times to generate Stats
    return static_cast<int>(std::floor(generate_num(0.25, 12) * level));
}

Stats::Stats() {
    this->stats_map["strength"] = 5;
    this->stats_map["dexterity"] = 5;
    this->stats_map["stamina"] = 5;
    this->stats_map["intelligence"] = 5;
    this->stats_map["wits"] = 5;
    this->stats_map["resolve"] = 5;
    this->stats_map["presence"] = 5;
    this->stats_map["manipulation"] = 5;
    this->stats_map["composure"] = 5;
    this->level = 1;
    this->max_hp = 60;
}

Stats::Stats(int level) {
    this->level = level;

    // 5 is the base level 1, need to remove base from calculations
    int temp = level - 1;

    // The generator has to be called every single time for actual random stats and not the same one
    this->stats_map["strength"] = 5 + generate_random_stat(temp);
    this->stats_map["dexterity"] = 5 + generate_random_stat(temp);
    this->stats_map["stamina"] = 5 + generate_random_stat(temp);
    this->stats_map["intelligence"] = 5 + generate_random_stat(temp);
    this->stats_map["wits"] = 5 + generate_random_stat(temp);
    this->stats_map["resolve"] = 5 + generate_random_stat(temp);
    this->stats_map["presence"] = 5 + generate_random_stat(temp);
    this->stats_map["manipulation"] = 5 + generate_random_stat(temp);
    this->stats_map["composure"] = 5 + generate_random_stat(temp);
    this->max_hp = (this->stats_map["stamina"] + this->stats_map["resolve"] + this->stats_map["composure"]) * 4;
}

Stats::Stats(
    int strength, int dexterity, int stamina,
    int intelligence, int wits, int resolve,
    int presence, int manipulation, int composure,
    int level
) {
    this->stats_map["strength"] = strength;
    this->stats_map["dexterity"] = dexterity;
    this->stats_map["stamina"] = stamina;
    this->stats_map["intelligence"] = intelligence;
    this->stats_map["wits"] = wits;
    this->stats_map["resolve"] = resolve;
    this->stats_map["presence"] = presence;
    this->stats_map["manipulation"] = manipulation;
    this->stats_map["composure"] = composure;

    this->max_hp = (stamina + resolve + composure) * 4;
    this->level = level;
}

void Stats::print_stats() {
    printf("Level: %d\nMax Health: %d\nStrength: %d\nDexterity: %d\nStamina: %d\nIntelligence: %d\nWits: %d\nResolve: %d\nPresence: %d\nManipulation: %d\nComposure: %d\n",
        this->level, this->max_hp,
        this->stats_map["strength"], this->stats_map["dexterity"], this->stats_map["stamina"],
        this->stats_map["intelligence"], this->stats_map["wits"], this->stats_map["resolve"],
        this->stats_map["presence"], this->stats_map["manipulation"], this->stats_map["composure"]
    );
}

void Stats::update_stats(uint16_t mask, std::vector<int> values) {
    // bitmask starts from rightmost bit, and shifts left to check.
    // starts at strength -> dex, and ends on composure, 
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if ((mask & (1 << i)) != 0) {
            switch(i){
            case 0:
                this->stats_map["strength"] = values[count];
                break;
            case 1:
                this->stats_map["dexterity"] = values[count];
                break;
            case 2:
                this->stats_map["stamina"] = values[count];
                break;
            case 3:
                this->stats_map["intelligence"] = values[count];
                break;
            case 4:
                this->stats_map["wits"] = values[count];
                break;
            case 5:
                this->stats_map["resolve"] = values[count];
                break;
            case 6:
                this->stats_map["presence"] = values[count];
                break;
            case 7:
                this->stats_map["manipulation"] = values[count];
                break;
            case 8:
                this->stats_map["composure"] = values[count];
                break;
            }

            count++;
        }
    }
}


