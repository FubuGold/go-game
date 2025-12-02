#include "../include/UI_element.h"
#include <iostream> // Testing only

namespace GUI {

sf::FloatRect combine_rect(sf::FloatRect rect1,sf::FloatRect rect2) {
    float mn_x = std::min(rect1.position.x, rect2.position.x);
    float mn_y = std::min(rect1.position.y, rect2.position.y);
    float mx_x = std::max(rect1.position.x + rect1.size.x, rect2.position.x + rect2.size.x);
    float mx_y = std::max(rect1.position.y + rect1.size.y, rect2.position.y + rect2.size.y);

    return sf::FloatRect({mn_x,mn_y},{mx_x - mn_x, mx_y - mn_y});
}

// ----------------------------------------------
// Element protected

void Element::press() {
    if (press_callback) press_callback();
}

void Element::release() {
    if (release_callback) release_callback();
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
        ptr = nullptr;
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

bool Element::check_window() {
    return window != nullptr;
}

void Element::draw() {
    for (sf::Drawable* ptr: parts) {
        if (!window) std::cerr << "Nullptr detected\n";
        window->draw(*ptr);
    }
}

void Element::poll_event(const std::optional<sf::Event> &e) {}

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
        else continue;

        if (!first) {
            rect = cur;
            first = true;
        }
        else {
            rect = combine_rect(rect,cur);
        }
    }
    bound = rect;
}

// ----------------------------------------------
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
    pos = {pos_x,pos_y};
    rect->setPosition({pos_x,pos_y});
    text->setPosition(rect->getPosition() + text_offset);
    update_bound();
}

void Rectangle_Button::set_pos(sf::Vector2f new_pos) {
    pos = new_pos;
    rect->setPosition(new_pos);
    text->setPosition(rect->getPosition() + text_offset);
    update_bound();
}

void Rectangle_Button::set_text_string(const std::string &str) {
    text->setString(str);
    update_bound();
}

void Rectangle_Button::poll_event(const std::optional<sf::Event> &e) {
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        if (mouse_pressed->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            press();
        }
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        if (mouse_release->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            release();
        }
    }
}

// ----------------------------------------------
// Droplist implementation

Droplist::Droplist(float button_width, float button_height, const std::string &title_text) {
    this->button_width = button_width;
    this->button_height = button_height;
    title_button = new Rectangle_Button(button_width, button_height);
    title_button->set_text_string(title_text);
    title_button->set_press([this](){
        std::cerr << "Title pressed\n";
        is_expanded = !is_expanded;
    });
    title_button->rect->setFillColor(sf::Color::Black);
    title_button->text->setFillColor(sf::Color::White);
}

Droplist::~Droplist() {
    for (Rectangle_Button *p: drop_list) {
        delete p;
    }
    delete title_button;
    drop_list.clear();
}

void Droplist::update_bound() {
    title_button->update_bound();
    bound = title_button->bound;
    for (Rectangle_Button *p: drop_list) {
        p->update_bound();
        bound = combine_rect(bound,p->bound);
    }
}

void Droplist::set_window(sf::RenderWindow *render_win_p) {
    std::cerr << "Droplist set window called\n";
    title_button->set_window(render_win_p);
    for (Rectangle_Button *p: drop_list) {
        p->set_window(render_win_p);
    }
    window = render_win_p;
}

void Droplist::add_element(const std::string &text, callback_t func) {
    count++;
    Rectangle_Button *tmp = new Rectangle_Button(button_width,button_height);
    tmp->set_press([this,func](){
        if (is_expanded) func();
    });
    tmp->set_text_string(text);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(-2);
    sf::Vector2f prev_pos;
    if (!drop_list.empty()) {
        prev_pos = drop_list.back()->get_pos();
    }
    else {
        prev_pos = title_button->get_pos();
    }
    prev_pos.y += button_height;
    tmp->set_pos(prev_pos);
    drop_list.push_back(tmp);
    update_bound();
}

void Droplist::set_pos(float pos_x,float pos_y) {
    pos = {pos_x,pos_y};
    title_button->set_pos(pos);
    sf::Vector2f prev_pos = {pos_x,pos_y};
    for (Rectangle_Button *p: drop_list) {
        prev_pos.y += button_height;
        p->set_pos(prev_pos);
    }
    update_bound();
}

void Droplist::set_pos(sf::Vector2f new_pos) {
    pos = new_pos;
    title_button->set_pos(pos);
    sf::Vector2f prev_pos = new_pos;
    for (Rectangle_Button *p: drop_list) {
        prev_pos.y += button_height;
        p->set_pos(prev_pos);
    }
    update_bound();
}

void Droplist::draw() {
    title_button->draw();
    if (is_expanded) {
        for (Rectangle_Button *p: drop_list) {
            p->draw();
        }
    }
}

void Droplist::poll_event(const std::optional<sf::Event> &e) {
    title_button->poll_event(e);
    for (Rectangle_Button *p: drop_list) {
        p->poll_event(e);
    }
}

// ----------------------------------------------
// Horizontal Slider implementation

H_Slider::H_Slider(float width,float height,int max_value, int steps, sf::Color main_color, sf::Color progress_color) {
    this->width = width;
    this->height = height;
    this->steps = steps;
    this->max_value = max_value;

    main_bar = new sf::RectangleShape();
    main_bar->setSize({width,height});
    main_bar->setFillColor(main_color);
    parts.push_back(main_bar);
    main_bar->setOutlineThickness(1);

    progress_bar = new sf::RectangleShape();
    progress_bar->setSize({width,height});
    progress_bar->setFillColor(progress_color);
    parts.push_back(progress_bar);

    update_bound();
}

void H_Slider::set_pos(float pos_x,float pos_y) {
    main_bar->setPosition({pos_x,pos_y});
    progress_bar->setPosition({pos_x,pos_y});
    pos = {pos_x,pos_y};
    update_bound();
}
void H_Slider::set_pos(sf::Vector2f new_pos) {
    main_bar->setPosition(new_pos);
    progress_bar->setPosition(new_pos);
    pos = new_pos;
    update_bound();
}

void H_Slider::press() {
    is_pressed = true;
    press_callback();
}

void H_Slider::release() {
    is_pressed = false;
    release_callback();
}

void H_Slider::update_value() {
    float mouse_pos_x = (window->mapPixelToCoords(sf::Mouse::getPosition(*window))).x;
    sf::Vector2f main_width = {pos.x,pos.x + width};
    if (main_width.x <= mouse_pos_x && mouse_pos_x <= main_width.y + 10) {
        cur_num_step = (mouse_pos_x - main_width.x) * steps / width;
        value = std::min(max_value,max_value * cur_num_step / steps);
        std::cerr << value << ' ' << mouse_pos_x << ' ' << main_width.x << '\n';
        std::cerr << (mouse_pos_x - main_width.x + 0.5) << ' ' << 1.0 * steps / width << '\n';
    }
}

void H_Slider::update_slider() {
    progress_bar->setSize({width / steps * value,height});
}

void H_Slider::poll_event(const std::optional<sf::Event> &e) {
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        if (mouse_pressed->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            press();
            update_value();
            update_slider();
        }
    }
    else if (e->getIf<sf::Event::MouseMoved>() && is_pressed) {
        update_value();
        update_slider();
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        if (mouse_release->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            release();
        }
    }
}

}