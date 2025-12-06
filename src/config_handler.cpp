#include "../include/config_handler.h"

#include <fstream>
#include <iostream> // Debug only

namespace Config {
    sf::Font font[] = {sf::Font("font/Jua-Regular.ttf"), sf::Font("font/Inter.ttf")};
    sf::Music bgm[] = {
        sf::Music("audio/song/song_1.mp3"),
        sf::Music("audio/song/song_2.mp3"),
        sf::Music("audio/song/song_3.mp3")
    };

    sf::SoundBuffer sfx_buffer[] = {
        sf::SoundBuffer("audio/sfx/menuclick.wav"),
        sf::SoundBuffer("audio/sfx/stoneplace.wav"),
        sf::SoundBuffer("audio/sfx/saveclick.wav")
    };

    sf::Sound sfx[] = {
        sf::Sound(sfx_buffer[0]),
        sf::Sound(sfx_buffer[1]),
        sf::Sound(sfx_buffer[2])
    };

    bool music_on = true;

    sf::Texture theme_texture[] = {
        sf::Texture("assets/theme/theme_1.png"),
        sf::Texture("assets/theme/theme_2.png"),
        sf::Texture("assets/theme/theme_1.png")
    };

    sf::Sprite theme[] = {
        sf::Sprite(theme_texture[0]),
        sf::Sprite(theme_texture[1]),
        sf::Sprite(theme_texture[2])
    };

    sf::Sprite selected_theme = theme[1];

    int selected_bgm = 0;
    int audio_volume[] = {100, 100, 100};

    float cal_music_vol(int id) {
        return 1.f * audio_volume[0] / 100.f * audio_volume[id];
    }

    void load_config() {
        json j;
        std::ifstream inp("data/config.json");
        inp >> j;

        // j.at("theme").get_to(selected_theme);
        j.at("bgm").get_to(selected_bgm);
        j.at("audio_volume").get_to(audio_volume);

        bgm[selected_bgm].setVolume(cal_music_vol(1));
        bgm[selected_bgm].setLooping(true);
        bgm[selected_bgm].play();

        return;
    }

    void save_config() {
        json j;

        // j["theme"] = selected_theme;
        j["bgm"] = selected_bgm;
        j["audio_volume"] = audio_volume;

        std::ofstream out("data/config.json");
        out << j;
        return;
    }

    void change_music(int new_bgm) {
        if (new_bgm == selected_bgm) return;
        bgm[selected_bgm].stop();
        selected_bgm = new_bgm;
        bgm[selected_bgm].setLooping(true);
        bgm[selected_bgm].setVolume(cal_music_vol(1));
        bgm[selected_bgm].play();
    }

    void change_theme(int new_theme) {

    }

    void update_volume() {
        bgm[selected_bgm].setVolume(cal_music_vol(1));
        sfx[0].setVolume(cal_music_vol(2));
        sfx[1].setVolume(cal_music_vol(2));
        sfx[2].setVolume(cal_music_vol(2));
    }
}