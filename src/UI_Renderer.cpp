#include "UI_Renderer.h"

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
    button_exit.set_text_string("EXIT");

    //Set other properties for all buttons
    set_initial_value(button_load_game, {102.f, 478.f});
    set_initial_value(button_new_game, {102.f, 627.f});
    set_initial_value(button_setting, {102.f, 776.f});
    set_initial_value(button_exit, {102.f, 925.f});
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

bool GameMenu::exit_button_active(const sf::RenderWindow &window) {
    return button_active(button_exit, window);
}

void GameMenu::draw_button(const Rectangle_Button &button, sf::RenderWindow &window) {
    window.draw(button.rect);
    window.draw(button.text);
}

void GameMenu::draw(sf::RenderWindow &window) {
    draw_button(button_load_game, window);
    draw_button(button_new_game, window);
    draw_button(button_setting, window);
    draw_button(button_exit, window);
}