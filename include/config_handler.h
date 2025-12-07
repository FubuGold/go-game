#if !defined(CONFIG_H)
#define CONFIG_H

#include "custom_util.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD,
    NONE
};

namespace Config {
    extern sf::Font font[2];
    extern sf::Music bgm[3];

    extern sf::SoundBuffer sfx_buffer[5];
    extern sf::Sound sfx[5];
    extern bool music_on;

    extern sf::Texture theme_texture[3];
    extern sf::Sprite theme[3];
    extern sf::SoundBuffer theme_sfx_buffer[3][2];
    extern sf::Sound theme_sfx[3][2];

    // Config
    extern sf::Sprite selected_theme;
    extern int selected_theme_number;
    extern int selected_bgm;
    extern int audio_volume[3]; // General - Music - SFX

    float cal_music_vol(int id);

    void load_config();
    void save_config();

    void change_music(int new_bgm);
    void change_theme(int new_theme);
    void update_volume();
}

#endif // CONFIG_H

