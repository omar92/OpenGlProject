#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::Image img;
    if (img.loadFromFile("defult.png")) {
        std::cout << "Loaded defult.png successfully!" << std::endl;
        img.saveToFile("test_out.png");
        return 0;
    } else {
        std::cout << "Failed to load defult.png" << std::endl;
        return 1;
    }
}
