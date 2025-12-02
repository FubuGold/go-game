#if !defined(CONFIG_H)
#define CONFIG_H

#include "custom_util.h"
#include <SFML/Graphics.hpp>

namespace Config {
    extern sf::Font font[2];

    // Config
    extern int prev_save;
    extern int selected_font;
    extern int selected_theme;
    extern int selected_background;
    extern int audio[3]; // General - Music - SFX

    void load_config();
    void save_config();
}

#endif // CONFIG_H

