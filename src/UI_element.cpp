#include "../include/UI_element.h"
#include <iostream> // Testing only

namespace GUI {

// ----------------------------------------------
// Utility function

sf::FloatRect combine_rect(sf::FloatRect rect1,sf::FloatRect rect2) {
    float mn_x = std::min(rect1.position.x, rect2.position.x);
    float mn_y = std::min(rect1.position.y, rect2.position.y);
    float mx_x = std::max(rect1.position.x + rect1.size.x, rect2.position.x + rect2.size.x);
    float mx_y = std::max(rect1.position.y + rect1.size.y, rect2.position.y + rect2.size.y);

    return sf::FloatRect({mn_x,mn_y},{mx_x - mn_x, mx_y - mn_y});
}

std::string int_to_string(int x) {
    std::string tmp = "";
    while (x > 0) {
        tmp = char('0' + x % 10) + tmp; // O(n^2) but value is small enough to not impact performance
        x /= 10;
    }
    if (tmp == "") tmp = "0";
    return tmp;
}

// ----------------------------------------------
// Element protected

void Element::press() {
    if (press_callback) press_callback();
}

void Element::release() {
    if (release_callback) release_callback();
}

void Element::hover() {
    if (hover_callback) hover_callback();
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
        else window->draw(*ptr);
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

Rectangle_Button::Rectangle_Button(float button_width,float button_height, sf::Color background_color, sf::Color text_color, unsigned int text_size) {
    rect->setSize({button_width,button_height});
    rect->setFillColor(background_color);
    
    text->setCharacterSize(text_size);
    text->setFillColor(text_color);

    parts.push_back(rect);
    parts.push_back(text);
    update_bound();
}

void Rectangle_Button::set_pos(float pos_x,float pos_y) {
    pos = {pos_x,pos_y};
    rect->setPosition({pos_x,pos_y});
    if (sprite) sprite->setPosition({pos_x, pos_y});
    sf::Vector2f rect_center = rect->getPosition() + rect->getSize() / 2.f;
    text->setPosition(rect_center + text_offset);
    update_bound();
}

void Rectangle_Button::set_pos(sf::Vector2f new_pos) {
    pos = new_pos;
    rect->setPosition(new_pos);
    if (sprite) sprite->setPosition(new_pos);
    sf::Vector2f rect_center = rect->getPosition() + rect->getSize() / 2.f;
    text->setPosition(rect_center + text_offset);
    update_bound();
}

void Rectangle_Button::set_text_string(const std::string &str) {
    text->setString(str);

    auto text_bound = text->getLocalBounds();
    text->setOrigin({text_bound.position.x + text_bound.size.x / 2.f, text_bound.position.y + text_bound.size.y / 2.f});
    sf::Vector2f rect_center = rect->getPosition() + rect->getSize() / 2.f;
    text->setPosition(rect_center + text_offset);

    update_bound();
}

void Rectangle_Button::set_texture(const std::filesystem::path &filename) {
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Failed to load texture of " << filename.filename() << '\n';
    }
    else {
        texture.setSmooth(true);
        sprite = new sf::Sprite(texture);
        sprite->setPosition(pos);
        parts.push_back(sprite);
    }
}

void Rectangle_Button::set_sprite(sf::Sprite *other_sprite) {
    sprite = other_sprite;
    parts.push_back(sprite);
}

void Rectangle_Button::press_virtual() {
    press();
}

void Rectangle_Button::poll_event(const std::optional<sf::Event> &e) {
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_pressed->position);
        if (mouse_pressed->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            press();
        }
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_release->position);
        if (mouse_release->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            release();
        }
    }
}

// ----------------------------------------------
// Board_Stone implementation

Board_Stone::~Board_Stone() {
    for (int i = 0; i < 3; i++) {
        delete stone_sprite[i][0];
        delete stone_sprite[i][1];
    }
}

Board_Stone::Board_Stone(sf::Vector2i board_pos, float button_width,float button_height, sf::Color background_color) {
    this->button_width = button_width;
    this->button_height = button_height;
    rect->setSize({button_width,button_height});
    rect->setFillColor(background_color);

    this->board_pos = board_pos;
    if (!textures[0][1].loadFromFile("assets/stones/theme_1_black_stone.png") 
    || !textures[0][0].loadFromFile("assets/stones/theme_1_white_stone.png")
    || !textures[1][1].loadFromFile("assets/stones/theme_2_black_stone.png")
    || !textures[1][0].loadFromFile("assets/stones/theme_2_white_stone.png")
    || !textures[2][1].loadFromFile("assets/stones/theme_3_black_stone.png")
    || !textures[2][0].loadFromFile("assets/stones/theme_3_white_stone.png")) {
        std::cerr << "Failed to load stone assets\n";
    }
    else {
        for (int i = 0; i < 3; i++) {
            textures[i][0].setSmooth(true);
            textures[i][1].setSmooth(true);
        }
    }

    for (int i = 0; i < 3; i++) {
        stone_sprite[i][0] = new sf::Sprite(textures[i][0]);
        stone_sprite[i][1] = new sf::Sprite(textures[i][1]);
    }

    parts.push_back(rect);
    update_bound();
}

void Board_Stone::poll_event(const std::optional<sf::Event> &e) {
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_pressed->position);
        if (mouse_pressed->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            press();
        }
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_release->position);
        if (mouse_release->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            release();
        }
    }
    else {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
        if (contain_pos(mouse_pos)) {
            hover();
        }
    }
}

// ----------------------------------------------
// Popup implementation

Popup::Popup(int duration, float rect_width, float rect_height, sf::Color background_color) {
    rect->setFillColor(background_color);
    rect->setSize({rect_width,rect_height});
    rect->setOutlineThickness(-4);
    rect->setOutlineColor(sf::Color::Black);
    parts.push_back(rect);
    disable = 1;
    this->duration = duration;
    update_bound();
}

Popup::~Popup() {
    delete rect;
    for (Element *p : element_l) {
        delete p;
    }
    element_l.clear();
}

void Popup::update_disable() {
    if (duration > 0) {
        std::chrono::system_clock::time_point cur_time = std::chrono::high_resolution_clock::now();
        milliseconds cur_d = duration_cast<milliseconds>(cur_time - enable_time);
        if (cur_d.count() >= duration) disable_popup();
    }
}

void Popup::enable_popup() {
    if (duration > 0) enable_time = std::chrono::high_resolution_clock::now();
    disable = 0;
}

void Popup::disable_popup() {
    disable = 1;
}

void Popup::set_window(sf::RenderWindow *render_win_ptr) {
    window = render_win_ptr;
    for (Element *p : element_l) {
        p->set_window(window);
    }
}

void Popup::set_pos(float pos_x,float pos_y) {
    rect->setPosition({pos_x,pos_y});
    update_bound();
}
void Popup::set_pos(sf::Vector2f new_pos) {
    rect->setPosition(new_pos);
    update_bound();
}

void Popup::add_element(Element *element_p) {
    element_l.push_back(element_p);
}
void Popup::add_drawable(sf::Drawable *drawable_p) {
    parts.push_back(drawable_p);
}

void Popup::draw() {
    update_disable();
    if (disable) return;
    Element::draw();
    for (Element *p : element_l) p->draw();
}

void Popup::poll_event(const std::optional<sf::Event> &e) {
    update_disable();
    if (disable) return;
    for (Element *p : element_l) p->poll_event(e);
}

// ----------------------------------------------
// Droplist implementation

Droplist::Droplist(
            float button_width,
            float button_height,
            const std::string &title_text,
            sf::Color main_color,
            sf::Color main_text_color,
            sf::Color sub_color,
            sf::Color sub_text_color,
            int text_size
        ) {
    this->button_width = button_width;
    this->button_height = button_height;
    this->sub_color = sub_color;
    this->sub_text_color = sub_text_color;
    this->text_size = text_size;

    title_button = new Rectangle_Button(button_width, button_height);
    title_button->text->setCharacterSize(text_size);
    title_button->set_text_string(title_text);
    title_button->set_press([this](){
        Config::sfx[0].play();
        is_expanded = !is_expanded;
    });
    title_button->rect->setFillColor(main_color);
    title_button->text->setFillColor(main_text_color);
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
    title_button->set_window(render_win_p);
    for (Rectangle_Button *p: drop_list) {
        p->set_window(render_win_p);
    }
    for (Element *p: extra) {
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
    tmp->text->setCharacterSize(text_size);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(-4);
    tmp->rect->setFillColor(sub_color);
    tmp->text->setFillColor(sub_text_color);
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
    tmp->set_text_string(text);
    update_bound();
}

void Droplist::add_extra(Element *p) {
    extra.push_back(p);
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
        for (Element *p: extra) {
            // std::cerr << p << '\n';
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

H_Slider::H_Slider(
            float width,
            float height,
            int max_value, 
            int steps,
            const std::string &name,
            sf::Color main_color, 
            sf::Color progress_color,
            int text_size,
            sf::Color text_color,
            int start_value
        ){
    this->width = width;
    this->height = height;
    this->steps = steps;
    this->max_value = max_value;
    this->value = start_value;

    main_bar = new sf::RectangleShape();
    main_bar->setSize({width,height});
    main_bar->setFillColor(main_color);
    main_bar->setOutlineThickness(1);
    parts.push_back(main_bar);

    progress_bar = new sf::RectangleShape();
    progress_bar->setSize({width,height});
    progress_bar->setFillColor(progress_color);
    parts.push_back(progress_bar);

    slider_name->setString(name);
    slider_name->setFillColor(text_color);
    slider_name->setCharacterSize(text_size);
    parts.push_back(slider_name);

    value_display->setString(int_to_string(max_value));
    value_display->setFillColor(text_color);
    value_display->setCharacterSize(text_size);
    parts.push_back(value_display);

    update_display();
    update_bound();
}

void H_Slider::set_pos(float pos_x,float pos_y) {
    main_bar->setPosition({pos_x,pos_y});
    progress_bar->setPosition({pos_x,pos_y});
    slider_name->setPosition({pos_x - 40, pos_y});
    value_display->setPosition({pos_x + width + 10, pos_y});
    pos = {pos_x,pos_y};
    update_bound();
}
void H_Slider::set_pos(sf::Vector2f new_pos) {
    main_bar->setPosition(new_pos);
    progress_bar->setPosition(new_pos);
    slider_name->setPosition(new_pos + sf::Vector2f(-40, 0));
    value_display->setPosition(new_pos + sf::Vector2f(width + 10, 0));
    pos = new_pos;
    update_bound();
}

void H_Slider::set_change(callback_t callback) {
    change_callback = callback;
}

void H_Slider::press() {
    is_pressed = true;
    if (press_callback) press_callback();
}

void H_Slider::release() {
    is_pressed = false;
    if (press_callback) release_callback();
}

void H_Slider::update_value() {
    float mouse_pos_x = (window->mapPixelToCoords(sf::Mouse::getPosition(*window))).x;
    sf::Vector2f main_width = {pos.x,pos.x + width};
    if (main_width.x <= mouse_pos_x && mouse_pos_x <= main_width.y + 10) {
        cur_num_step = (mouse_pos_x - main_width.x) * steps / width;
        value = std::min(max_value,max_value * cur_num_step / steps);
    }
    if (change_callback) change_callback();
}

void H_Slider::update_display() {
    progress_bar->setSize({width / steps * value,height});
    value_display->setString(int_to_string(value));
}

void H_Slider::set_display_value_pos(sf::Vector2f new_pos) {
    value_display->setPosition(new_pos);
}

void H_Slider::set_name_pos(sf::Vector2f new_pos) {
    slider_name->setPosition(new_pos);
}

bool H_Slider::contain_pos(float x,float y) {
    return main_bar->getGlobalBounds().contains({x,y});
}
bool H_Slider::contain_pos(sf::Vector2f point) {
    return main_bar->getGlobalBounds().contains(point);
}
// Use custom contain_pos to check only the slider
void H_Slider::poll_event(const std::optional<sf::Event> &e) {
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_pressed->position);
        if (mouse_pressed->button == sf::Mouse::Button::Left && contain_pos(mouse_pos)) {
            press();
            update_value();
            update_display();
        }
    }
    else if (e->getIf<sf::Event::MouseMoved>() && is_pressed) {
        update_value();
        update_display();
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release = e->getIf<sf::Event::MouseButtonReleased>()) {
        sf::Vector2f mouse_pos = window->mapPixelToCoords(mouse_release->position);
        if (mouse_release->button == sf::Mouse::Button::Left) {
            release();
        }
    }
}

// ----------------------------------------------
// Dynamic Text implementation

Dynamic_Text::Dynamic_Text(int *link_var, sf::Color text_color, int text_size) {
    linked_var = link_var;
    text->setFillColor(text_color);
    text->setCharacterSize(text_size);
    parts.push_back(text);
}

void Dynamic_Text::set_pos(float pos_x,float pos_y) {
    pos = {pos_x,pos_y};
    text->setPosition(pos);
}
void Dynamic_Text::set_pos(sf::Vector2f new_pos) {
    pos = new_pos;
    text->setPosition(pos);
}

void Dynamic_Text::draw() {
    text->setString(int_to_string(*linked_var));
    Element::draw();
}

}