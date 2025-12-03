#include "../include/config_handler.h"

#include <fstream>
#include <iostream> // Debug only

namespace Config {
    sf::Font font[] = {sf::Font("font/Jua-Regular.ttf"), sf::Font("font/Inter.ttf")};
    sf::Music bgm[3] = {
        sf::Music("audio/song/song_1.mp3"),
        sf::Music("audio/song/song_2.mp3"),
        sf::Music("audio/song/song_3.mp3")
    };
    
    int prev_save = 0;
    int selected_theme = 0;
    int selected_bgm = 0;
    int audio_volume[3] = {100,100,100};

    float cal_music_vol() {
        return 1.f * audio_volume[0] / 100.f * audio_volume[1];
    }

    void load_config() {
        json j;
        std::ifstream inp("data/config.json");
        inp >> j;

        j.at("prev_save").get_to(prev_save);
        j.at("theme").get_to(selected_theme);
        j.at("bgm").get_to(selected_bgm);
        j.at("audio_volume").get_to(audio_volume);

        bgm[selected_bgm].setVolume(cal_music_vol());
        bgm[selected_bgm].setLooping(true);
        bgm[selected_bgm].play();

        return;
    }

    void save_config() {
        json j;

        j["prev_save"] = prev_save;
        j["theme"] = selected_theme;
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
        bgm[selected_bgm].setVolume(cal_music_vol());
        bgm[selected_bgm].play();
    }

    void change_theme(int new_theme) {

    }

    void update_volume() {
        bgm[selected_bgm].setVolume(cal_music_vol());
    }
}