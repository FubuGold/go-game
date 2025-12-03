#if !defined(CONFIG_H)
#define CONFIG_H

#include "game_logic.h"
#include "custom_util.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Config {
    extern sf::Font font[2];
    extern sf::Music bgm[3];

    // Config
    extern int prev_save;
    extern int selected_font;
    extern int selected_theme;
    extern int selected_bgm;
    extern int audio_volume[3]; // General - Music - SFX

    void load_config();
    void save_config();

    void change_music(int new_bgm);
    void change_theme(int new_theme);
    void update_volume();
}

#endif // CONFIG_H

