#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include "include/Stats.hpp"
#include "include/FileReader.hpp"
#include "include/Cat.hpp"

std::unique_ptr<FileReader> fr;
std::vector<std::vector<std::string>> csv_file;
std::random_device rd;
unsigned int window_width = 600;
unsigned int window_height = 600;

void wait_for_input() {
    std::cout << "Press Enter to continue...";
    
    // Read and discard a single character (the Enter key)
    std::cin.get(); 
}

std::string get_random_name() {
    // Uses the loaded csv file and gets a name from first or last name within the list
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, csv_file.size() - 1);

    std::uniform_int_distribution<int> flip(0, 1);

    return csv_file[distrib(gen)][flip(gen)];
}

void test_stats() {
    // Just learned about smart pointers so I am using those from now on
    // Default Constructor
    std::unique_ptr<Stats> default_stats = std::make_unique<Stats>();
    default_stats->print_stats();
    wait_for_input();

    // Manual Input Values
    std::unique_ptr<Stats> manual_stats1 = std::make_unique<Stats>(1, 2, 3, 4, 5, 6, 7, 8, 9, 4);
    manual_stats1->print_stats();
    wait_for_input();
    
    std::unique_ptr<Stats> manual_stats2 = std::make_unique<Stats>(1, 2, 5, 4, 5, 6, 7, 8, 9, 4);
    manual_stats2->print_stats();
    wait_for_input();

    std::unique_ptr<Stats> manual_stats3 = std::make_unique<Stats>(1, 2, 5, 4, 5, 7, 7, 8, 9, 4);
    manual_stats3->print_stats();
    wait_for_input();

    std::unique_ptr<Stats> manual_stats4 = std::make_unique<Stats>(1, 2, 5, 4, 5, 7, 7, 8, 2, 4);
    manual_stats4->print_stats();
    wait_for_input();

    // Manual Level Input
    std::unique_ptr<Stats> level_stats1 = std::make_unique<Stats>(1);
    level_stats1->print_stats();
    wait_for_input();

    std::unique_ptr<Stats> level_stats2 = std::make_unique<Stats>(5);
    level_stats2->print_stats();
    wait_for_input();

    std::unique_ptr<Stats> level_stats3 = std::make_unique<Stats>(10);
    level_stats3->print_stats();
    wait_for_input();

    std::unique_ptr<Stats> level_stats4 = std::make_unique<Stats>(100);
    level_stats4->print_stats();
    wait_for_input();

    // Testing the stats update and bitmasking
    level_stats4->update_stats(0b101010001, {5, 6, 7, 8});
    level_stats4->print_stats();
    wait_for_input();
}

void test_file_reader() {
    std::unique_ptr<FileReader> fr = std::make_unique<FileReader>();

    std::vector<std::vector<std::string>> csv_file = fr->read_file("data/random_names.csv");

    for (const auto& row : csv_file) {
        for (const auto& cell : row) {
            std::cout << cell << "\t\t ";
        }

        std::cout << "\n";
    }
    
}

void test_cat() {
    std::unique_ptr<Cat> cat1 = std::make_unique<Cat>();
    cat1->print_cat_info();
    wait_for_input();

    std::unique_ptr<Cat> cat2 = std::make_unique<Cat>(5, get_random_name(), 50, 50);
    cat2->print_cat_info();
    wait_for_input();

    std::unique_ptr<Cat> cat3 = std::make_unique<Cat>(7, get_random_name(), 50, 50);
    cat3->print_cat_info();
    wait_for_input();

    std::unique_ptr<Cat> cat4 = std::make_unique<Cat>(1, 2, 3, 4, 5, 6, 7, 8, 9, 4, get_random_name(), 50, 50);
    cat4->print_cat_info();
    wait_for_input();

}

int main(int, char**){
    std::cout << "Hello, from Cats!\n";

    fr = std::make_unique<FileReader>();
    csv_file = fr->read_file("data/random_names.csv");

    Cat cat1 = Cat();
    Cat cat2 = Cat(200, 200);

    // Test Stats and functions
    // test_stats();
    // test_file_reader();
    // std::cout << "Random Name: " << get_random_name() << std::endl;
    // test_cat();
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Testing SFML");
    window.setFramerateLimit(60);

    // Copy of code from Pacman project, remove later, just for testing SFML functionality
    sf::CircleShape pacImg;
    pacImg.setRadius(8);
    pacImg.setOutlineColor(sf::Color::Black);
    pacImg.setOutlineThickness(2);
    pacImg.setFillColor(sf::Color::Yellow);
    float topLeftX = (window_width - (28 * 8)) / 2;
    float topLeftY = (window_height - 31 * 8) / 2;
    float posX = topLeftX + (13.5 * 8);
    float posY = topLeftY + (17 * 8);
    pacImg.setPosition({posX, posY});
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            // Check if the user clicked the 'X' close button
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Black); // Clear screen with black background
        window.draw(pacImg);
        window.draw(cat1);
        window.draw(cat2);
        window.display();               // Flip buffers to show the frame
    }
}