#include "../include/UI_renderer.h"

#include <iostream> // Testing only

void Rectangle_Button::set_text_string(const std::string &str) {
    text.setString(str);
}

bool Rectangle_Button::isHover(const sf::RenderWindow &window) const {
    auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    return rect.getGlobalBounds().contains(mouse_pos);
}


bool Rectangle_Button::isClicked(const sf::RenderWindow &window) const {
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isHover(window);
}

GameMenu::GameMenu(float button_height, float button_width) {
    this->button_height = button_height;
    this->button_width = button_width;
    
    //Set text content for all buttons
    button_load_game.set_text_string("LOAD GAME");
    button_new_game.set_text_string("NEW GAME");
    button_setting.set_text_string("SETTING");
    button_exit_text.set_text_string("EXIT");

    //Set other properties for all buttons
    set_initial_value(button_load_game, {102.f, 478.f});
    set_initial_value(button_new_game, {102.f, 627.f});
    set_initial_value(button_setting, {102.f, 776.f});

    button_exit_text.set_pos(102.f,925.f);
    button_exit_text.set_press([this](){
        window->close();
    });
}

void GameMenu::set_window(sf::RenderWindow *render_win) {
    button_exit_text.set_window(render_win);
    window = render_win;
}

void GameMenu::set_initial_value(Rectangle_Button &button, const sf::Vector2f &position) {
    button.rect.setSize({button_width, button_height});
    button.rect.setPosition(position);
    button.rect.setFillColor(sf::Color::White);
    
    button.text.setCharacterSize(60);
    button.text.setFillColor(sf::Color::Black);
    button.text.setPosition({
        button.rect.getPosition().x + 21,
        button.rect.getPosition().y + 14
    });
}

bool GameMenu::button_active(const Rectangle_Button &button, const sf::RenderWindow &window) {
    return button.isClicked(window);
}

void GameMenu::exit_button_poll(const sf::Event &e) {
    button_exit_text.poll_event(e);
}

void GameMenu::draw_button(const Rectangle_Button &button, sf::RenderWindow &window) {
    window.draw(button.rect);
    window.draw(button.text);
}

void GameMenu::draw(sf::RenderWindow &window) {
    draw_button(button_load_game, window);
    draw_button(button_new_game, window);
    draw_button(button_setting, window);

    button_exit_text.draw();
}