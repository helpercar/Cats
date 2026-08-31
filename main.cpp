#include <iostream>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "include/Stats.hpp"
#include "include/FileReader.hpp"
#include "include/Cat.hpp"

std::unique_ptr<FileReader> fr;
std::vector<std::vector<std::string>> csv_file;
std::random_device rd;
unsigned int window_width = 600;
unsigned int min_width = 600;
unsigned int window_height = 600;
unsigned int min_height = 600;

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

    Cat cat1 = Cat(300, 300);
    Cat cat2 = Cat(200, 200);

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), "Testing SFML");
    
    // Initialize view with an internal 1:1 resolution
    sf::View view(sf::FloatRect({0.f, 0.f}, {600.f, 600.f}));
    window.setFramerateLimit(60);
    sf::Clock clock;

    const unsigned int min_width = 600;
    const unsigned int min_height = 600; // Keep min size 1:1 for consistency
    
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
            else if (const auto* resize = event->getIf<sf::Event::Resized>()) {
                unsigned int new_width = resize->size.x;
                unsigned int new_height = resize->size.y;
                bool need_resize = false;

                // Enforce absolute minimum window limits
                if (new_width < min_width) { new_width = min_width; need_resize = true; }
                if (new_height < min_height) { new_height = min_height; need_resize = true; }

                if (need_resize) {
                    window.setSize({new_width, new_height});
                }

                // Calculate Letterbox / Pillarbox viewport coordinates
                // Target aspect ratio is 1.0 (1:1)
                float windowRatio = static_cast<float>(new_width) / static_cast<float>(new_height);
                float viewRatio = 1.0f; 
                
                float viewportX = 0.f;
                float viewportY = 0.f;
                float viewportWidth = 1.f;
                float viewportHeight = 1.f;

                if (windowRatio > viewRatio) {
                    // Window is wider than 1:1 -> Pillarbox (bars on left/right)
                    viewportWidth = viewRatio / windowRatio;
                    viewportX = (1.f - viewportWidth) / 2.f;
                } else {
                    // Window is taller than 1:1 -> Letterbox (bars on top/bottom)
                    viewportHeight = windowRatio / viewRatio;
                    viewportY = (1.f - viewportHeight) / 2.f;
                }

                // Apply the viewport rectangle (normalized between 0.0 and 1.0)
                view.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportWidth, viewportHeight}));
                window.setView(view);
            }
        }
        float time = clock.getElapsedTime().asSeconds();

        // Calculate a slight wiggle angle (oscillates between -15 and +15 degrees)
        float earWiggle = std::sin(time * 6.f) * 15.f; 

        // Ear Rotation
        SpritePart& head1 = cat1.sprite.getPart("torso").getChild("head");
        head1.getChild("leftEar").setRotation(sf::degrees(325.f + earWiggle));
        head1.getChild("rightEar").setRotation(sf::degrees(45.f - earWiggle));

        // Leg Rotation
        float hipAngle = std::sin(time * 4.f) * 20.f;
        float kneeAngle = (std::cos(time * 4.f) * 25.f) + 25.f;

        cat2.sprite.getPart("frontrighttopleg").setRotation(sf::degrees(30.f + hipAngle));
        cat2.sprite.getPart("frontrighttopleg").getChild("frontrightbottomleg").setRotation(sf::degrees(kneeAngle));

        // Breathing
        float breatheOffset = std::sin(time * 2.f) * 5.f;
        cat2.sprite.setPosition({200.f, 200.f + breatheOffset});

        window.clear(sf::Color::Black); // This color fills the background borders
        
        window.draw(cat1);
        window.draw(cat2);
        
        window.display();
    }
}
