#include "../include/game_handler.h"

GameHandler::GameHandler(unsigned int window_height, unsigned int window_width) {
    this->window_height = window_height;
    this->window_width = window_width;

    window = sf::RenderWindow(sf::VideoMode::getDesktopMode(), game_name, sf::Style::Default, sf::State::Fullscreen);
    game_state = GameState::Menu;
}

void GameHandler::resize_window(sf::View &UI_view) {
    auto window_size = window.getSize();
    float window_ratio = static_cast<float>(window_size.x) / window_size.y;
    float UI_ratio = static_cast<float>(window_width) / window_height;

    float size_X = 1.f, size_Y = 1.f;
    float pos_X = 0.f, pos_Y = 0.f;

    if (window_ratio > UI_ratio) {
        size_X = UI_ratio / window_ratio;
        pos_X = (1.f - size_X) / 2.f;
    }
    else if (window_ratio < UI_ratio) {
        size_Y = window_ratio / UI_ratio;
        pos_Y = (1.f - size_Y) / 2.f;
    }

    UI_view.setViewport(sf::FloatRect({pos_X, pos_Y}, {size_X, size_Y}));
}

void GameHandler::run() {
    window.setFramerateLimit(60);

    sf::View UI_view;
    UI_view.setCenter({static_cast<float>(window_width) / 2.f, static_cast<float>(window_height) / 2.f});
    UI_view.setSize({static_cast<float>(window_width), static_cast<float>(window_height)});
    UI_view.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
    window.setView(UI_view);

    bool isFullscreen = true;
    sf::VideoMode video_mode = sf::VideoMode::getDesktopMode();

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (game_state == GameState::Menu) {
                if (event->is<sf::Event::KeyPressed>()) {
                    auto key = event->getIf<sf::Event::KeyPressed>();
                    if (key->code == sf::Keyboard::Key::F11) {
                        if (isFullscreen) {
                            window.create(sf::VideoMode({800, 600}), game_name, sf::Style::Default, sf::State::Windowed);
                        }
                        else {
                            window.create(video_mode, game_name, sf::Style::Default, sf::State::Fullscreen);
                        }
                        isFullscreen ^= 1;
                    }
                }
                else if (event->is<sf::Event::MouseButtonPressed>()) {
                    if (game_menu.exit_button_active(window)) {
                        window.close();
                    }
                }
            }
            else if (game_state == GameState::Playing) {

            }
            else if (game_state == GameState::Setting) {

            }
        }

        resize_window(UI_view);

        window.clear({255, 223, 128});
        window.setView(UI_view);

        //Draw game
        if (game_state == GameState::Menu) {
            game_menu.draw(window);
        }
        else if (game_state == GameState::Playing) {

        }
        else if (game_state == GameState::Setting) {
            
        }

        window.display();
    }
}