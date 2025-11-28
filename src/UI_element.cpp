#include "../include/UI_element.h"
#include <iostream> // Testing only

namespace GUI {

// Element protected

void Element::press() {
    if (press_callback) press_callback();
}

void Element::release() {
    if (release_callback) release_callback();
}

void Element::change_state(State state) {
    if (cur_state == state || (state == ST_HOVERED)) return;
    
    if (state == ST_PRESSED) {
        if (cur_state == ST_DEFAULT) press();
    }
    else {
        if (cur_state == ST_PRESSED) release();
    }

    cur_state = state;
}

bool Element::contain_pos(float x,float y) {
    return bound.contains({x,y});
}
bool Element::contain_pos(sf::Vector2f point) {
    return bound.contains(point);
}

// Element external
Element::~Element() {
    for (sf::Drawable* ptr: parts) {
        delete ptr;
    }
    parts.clear();
}

void Element::set_hover(callback_t callback) {
    hover_callback = callback;
}

void Element::set_press(callback_t callback) {
    press_callback = callback;
}

void Element::set_release(callback_t callback) {
    release_callback = callback;
}

void Element::set_pos(float pos_x,float pos_y) {
    pos = {pos_x,pos_y};
}
void Element::set_pos(sf::Vector2f new_pos) {
    pos = new_pos;
}

sf::Vector2f Element::get_pos() {
    return pos;
}

sf::Vector2f Element::get_size() {
    return bound.size;
} 

void Element::set_window(sf::RenderWindow *render_win_ptr) {
    window = render_win_ptr;
}

void Element::draw() {
    for (sf::Drawable* ptr: parts) {
        if (!window) std::cerr << "Nullptr detected\n";
        window->draw(*ptr);
    }
}

void Element::poll_event(const sf::Event &e) {}

sf::FloatRect combine_rect(sf::FloatRect rect1,sf::FloatRect rect2) {
    float mn_x = std::min(rect1.position.x, rect1.position.x);
    float mn_y = std::min(rect1.position.y, rect2.position.y);
    float mx_x = std::max(rect1.position.x, rect1.position.x);
    float mx_y = std::max(rect1.position.y, rect2.position.y);

    return sf::FloatRect({mn_x,mn_y},{mx_x - mn_x, mx_y - mn_y});
}

void Element::update_bound() {
    sf::FloatRect rect;
    bool first = false;
    for (sf::Drawable* ptr: parts) {
        sf::FloatRect cur;
        if (auto shape = dynamic_cast<sf::Shape*>(ptr)) {
            cur = shape->getGlobalBounds();
        }
        else if (auto shape = dynamic_cast<sf::Text*>(ptr)) {
            cur = shape->getGlobalBounds();
        }
        else if (auto shape = dynamic_cast<sf::Sprite*>(ptr)) {
            cur = shape->getGlobalBounds();
        }
        
        if (!first) rect = cur;
        else {
            rect = combine_rect(rect,cur);
        }
    }
    bound = rect;
}

// Rectangle button external

Rectangle_Button::Rectangle_Button(float button_width,float button_height) {
    rect->setSize({button_width,button_height});
    rect->setFillColor(sf::Color::White);
    
    text->setCharacterSize(60);
    text->setFillColor(sf::Color::Black);

    parts.push_back(rect);
    parts.push_back(text);
    update_bound();
}

void Rectangle_Button::set_pos(float pos_x,float pos_y) {
    rect->setPosition({pos_x,pos_y});
    text->setPosition({
        rect->getPosition().x + 21,
        rect->getPosition().y + 14
    });
    update_bound();
}

void Rectangle_Button::set_pos(sf::Vector2f new_pos) {
    rect->setPosition(new_pos);
    text->setPosition({
        rect->getPosition().x + 21,
        rect->getPosition().y + 14
    });
    update_bound();
}

void Rectangle_Button::set_text_string(const std::string &str) {
    text->setString(str);
    update_bound();
}

void Rectangle_Button::poll_event(const sf::Event &e) {
    sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
    if (contain_pos(mouse_pos)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            press();
        }
    }
}

}