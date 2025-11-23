#ifndef GAME_UI_H
#define GAME_UI_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "UI_Renderer.h"

enum class GameState {
    Menu,
    Playing,
    Setting
};

class GameUI {
private:
    sf::RenderWindow window;
    GameState game_state;
    GameMenu game_menu;
    unsigned int window_height, window_width;
    const std::string game_name = "Go game";

public: 
    GameUI(unsigned int window_height = 1920, unsigned int window_width = 1080, float button_height = 103.f, float button_width = 377.f);

    /**
     * @brief Rescaling when window resize occurs
     * 
     * @param UI_view 
     */
    void resize_window(sf::View &UI_view);

    /**
     * @brief Run the game
     * 
     */
    void run();
};

#endif