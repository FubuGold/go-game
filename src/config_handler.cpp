#include "../include/config_handler.h"

#include <fstream>

namespace Config {
    sf::Font font[] = {sf::Font("font/Jua-Regular.ttf"), sf::Font("font/Inter.ttf")};
    
    int prev_save = 0;
    int selected_theme = 0;
    int selected_background = 0;
    int audio[3] = {100,100,100};

    void load_config() {
        json j;
        std::ifstream inp("data/config.json");
        inp >> j;

        j.at("prev_save").get_to(prev_save);
        j.at("theme").get_to(selected_theme);
        j.at("background").get_to(selected_background);
        j.at("audio").get_to(audio);

        return;
    }

    void save_config() {
        json j;

        j["prev_save"] = prev_save;
        j["theme"] = selected_theme;
        j["background"] = selected_background;
        j["audio"] = audio;

        std::ofstream out("data/config.json");
        out << j;
        return;
    }
}