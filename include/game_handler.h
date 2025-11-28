#ifndef GAME_HANDLER_H
#define GAME_HANDLER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <array>

#include "UI_renderer.h"

enum class GameState {
    Menu,
    Playing,
    Setting,
    Count
};

inline size_t state_to_int(GameState state) {
    return static_cast<size_t>(state);
}

class GameHandler {
private:
    sf::RenderWindow window;
    GameState game_state;
    std::array<GUI::Canvas*, static_cast<size_t>(GameState::Count)> canvas;
    unsigned int window_height, window_width;
    const std::string game_name = "Go game";

    /**
     * @brief Add components to the menu
     * 
     */
    void setup_menu();

public: 
    GameHandler(unsigned int window_width = 1920, unsigned int window_height = 1080);

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