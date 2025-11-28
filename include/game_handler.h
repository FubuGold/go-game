#ifndef GAME_HANDLER_H
#define GAME_HANDLER_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "UI_renderer.h"

enum class GameState {
    Menu,
    Playing,
    Setting
};

class GameHandler {
private:
    sf::RenderWindow window;
    GameState game_state;
    GameMenu game_menu;
    unsigned int window_height, window_width;
    const std::string game_name = "Go game";

public: 
    GameHandler(unsigned int window_height = 1080, unsigned int window_width = 1920);

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