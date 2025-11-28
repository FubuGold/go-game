#include "../include/config_handler.h"

#include <fstream>

namespace Config {
    sf::Font font = sf::Font("font/Jua-Regular.ttf");

    // TO IMPLEMENT
    void load_config() {
        json j;
        std::ifstream inp("data/config.json");
        inp >> j;


        return;
    }

    // TO IMPLEMENT
    void save_config() {
        json j;
        // adgjhasjdfg 


        std::ofstream out("data/config.json");
        out << j;
        return;
    }
}