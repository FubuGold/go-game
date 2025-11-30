#include "../include/game_handler.h"
#include <iostream> // Testing only

GameHandler::GameHandler(unsigned int window_width, unsigned int window_height) {
    std::cerr << "Game handler setup\n";
    this->window_width = window_width;
    this->window_height = window_height;
    
    window = sf::RenderWindow(sf::VideoMode::getDesktopMode(), game_name, sf::Style::Default, sf::State::Fullscreen);
    game_state = GameState::Menu;

    // Testing
    // canvas[0] = new GUI::Canvas();
    // GUI::Droplist *droplist = new GUI::Droplist(450,100);
    // droplist->set_pos(100,100);
    // droplist->add_element("Test 1",[](){
        //     std::cerr << "Test 1 pressed\n";
        // });
        // droplist->add_element("Test 2",[](){
            //     std::cerr << "Test 2 pressed\n";
            // });
            // droplist->add_element("Test 3",[](){
                //     std::cerr << "Test 3 pressed\n";
                // });
                
                // canvas[0]->add_element(droplist);
                
    canvas[0] = new GUI::Menu_Canvas();
    canvas[1] = new GUI::Canvas();
    canvas[2] = new GUI::Canvas();

    for (int i=0;i<state_to_int(GameState::Count);i++) {
        canvas[i]->set_window(&window);
        canvas[i]->setup();
    }
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
    canvas[state_to_int(game_state)]->recal_bound();
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
        size_t cur_state = state_to_int(game_state);
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (event->is<sf::Event::KeyPressed>()
                    && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::F11) {
                if (isFullscreen) {
                    window.create(sf::VideoMode({800, 600}), game_name, sf::Style::Default, sf::State::Windowed);
                }
                else {
                    window.create(video_mode, game_name, sf::Style::Default, sf::State::Fullscreen);
                }
                isFullscreen ^= 1;
            }
            else if (event.has_value()){
                canvas[cur_state]->poll_event(event);
            }
        }

        if (!window.isOpen()) break;

        resize_window(UI_view);

        window.clear({255, 223, 128});
        window.setView(UI_view);

        
        //Draw game
        canvas[cur_state]->draw();

        window.display();
    }

}
