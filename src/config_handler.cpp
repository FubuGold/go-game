#include "../include/config_handler.h"

#include <fstream>
#include <iostream> // Debug only

namespace Config {
    sf::Image game_icon("assets/stones/theme_2_black_stone.png");
    bool isFullscreen = true;
    std::pair<int,int> resolution = {1920,1080};

    sf::Font font[] = {sf::Font("font/Jua-Regular.ttf"), sf::Font("font/Inter.ttf")};
    sf::Music bgm[] = {
        sf::Music("assets/audio/song/song_1.mp3"),
        sf::Music("assets/audio/song/song_2.mp3"),
        sf::Music("assets/audio/song/song_3.mp3")
    };

    sf::SoundBuffer sfx_buffer[] = {
        sf::SoundBuffer("assets/audio/sfx/menuclick.wav"),
        sf::SoundBuffer("assets/audio/sfx/theme_1_stoneplace.wav"),
        sf::SoundBuffer("assets/audio/sfx/saveclick.wav"),
        sf::SoundBuffer("assets/audio/sfx/kothreat.wav"),
        sf::SoundBuffer("assets/audio/sfx/stonecapture.wav"),
        sf::SoundBuffer("assets/audio/sfx/gamefinish.wav")
    };

    sf::Sound sfx[] = {
        sf::Sound(sfx_buffer[0]),
        sf::Sound(sfx_buffer[1]),
        sf::Sound(sfx_buffer[2]),
        sf::Sound(sfx_buffer[3]),
        sf::Sound(sfx_buffer[4]),
        sf::Sound(sfx_buffer[5])
    };

    bool music_on = true;

    sf::Texture theme_texture[] = {
        sf::Texture("assets/board_background/theme_1.png"),
        sf::Texture("assets/board_background/theme_2.png"),
        sf::Texture("assets/board_background/theme_3.png")
    };

    sf::Sprite theme[] = {
        sf::Sprite(theme_texture[0]),
        sf::Sprite(theme_texture[1]),
        sf::Sprite(theme_texture[2])
    };

    sf::SoundBuffer theme_sfx_buffer[3][2] = {
        {sf::SoundBuffer("assets/audio/sfx/theme_1_stoneplace.wav"), sf::SoundBuffer("assets/audio/sfx/theme_1_stoneplace.wav")},
        {sf::SoundBuffer("assets/audio/sfx/theme_2_stoneplace.wav"), sf::SoundBuffer("assets/audio/sfx/theme_2_stoneplace.wav")},
        {sf::SoundBuffer("assets/audio/sfx/theme_3_stoneplace_white.wav"), sf::SoundBuffer("assets/audio/sfx/theme_3_stoneplace_black.wav")}
    };

    sf::Sound theme_sfx[3][2] = {
        {sf::Sound(theme_sfx_buffer[0][0]), sf::Sound(theme_sfx_buffer[0][1])},
        {sf::Sound(theme_sfx_buffer[1][0]), sf::Sound(theme_sfx_buffer[1][1])},
        {sf::Sound(theme_sfx_buffer[2][0]), sf::Sound(theme_sfx_buffer[2][1])}
    };

    sf::Sprite selected_theme = theme[0];
    int selected_theme_number = 0;

    int selected_bgm = 0;
    int audio_volume[] = {100, 100, 100};

    float cal_volume(int id) {
        return 1.f * audio_volume[0] / 100.f * audio_volume[id];
    }

    void load_config() {
        json j;
        std::ifstream inp("data/config.json");
        inp >> j;

        // j.at("theme").get_to(selected_theme);
        j.at("bgm").get_to(selected_bgm);
        j.at("audio_volume").get_to(audio_volume);
        j.at("theme").get_to(selected_theme_number);
        j.at("isFullscreen").get_to(isFullscreen);
        j.at("resolution").get_to(resolution);

        bgm[selected_bgm].setVolume(cal_volume(1));
        bgm[selected_bgm].setLooping(true);
        bgm[selected_bgm].play();

        Config::selected_theme = Config::theme[selected_theme_number];

        return;
    }

    void save_config() {
        json j;

        // j["theme"] = selected_theme;
        j["bgm"] = selected_bgm;
        j["audio_volume"] = audio_volume;
        j["theme"] = selected_theme_number;
        j["isFullscreen"] = isFullscreen;
        j["resolution"] = resolution;

        std::ofstream out("data/config.json");
        out << j;
        return;
    }

    void change_music(int new_bgm) {
        if (new_bgm == selected_bgm) return;
        bgm[selected_bgm].stop();
        selected_bgm = new_bgm;
        bgm[selected_bgm].setLooping(true);
        bgm[selected_bgm].setVolume(cal_volume(1));
        bgm[selected_bgm].play();
    }

    void update_volume() {
        bgm[selected_bgm].setVolume(cal_volume(1));
        for (int i=0;i<6;i++) {
            sfx[i].setVolume(cal_volume(2));
        }
    }
}