#include "../include/UI_renderer.h"

#include <iostream> // Testing only

namespace GUI {

// ---------------------------------------------------
// Canvas implementation

Canvas::~Canvas() {
    window = nullptr;
    for (Element *p: element_l) {
        delete p;
    }
    element_l.clear();
}

void Canvas::add_element(Element *new_element) {
    if (window && !new_element->check_window()) new_element->set_window(window);
    element_l.push_back(new_element);
}

void Canvas::set_window(sf::RenderWindow *render_win_ptr) {
    window = render_win_ptr;
    for (Element *p : element_l) {
        p->set_window(render_win_ptr);
    }
}

void Canvas::draw() {
    for (Element *p : element_l) {
        p->draw();
    }
}

void Canvas::poll_event(const std::optional<sf::Event> &e) {
    for (Element *p : element_l) {
        p->poll_event(e);
    }
}

void Canvas::clean_element() {
    for (Element *p : element_l) {
        delete p;
    }
    element_l.clear();
}

void Canvas::recal_bound() {
    for (Element *p : element_l) {
        p->update_bound();
    }
}

void Canvas::setup() {};

// ---------------------------------------------------
// Menu implementation

void Menu_Canvas::reset_button_size(float new_width,float new_height) {
    this->button_width = new_width;
    this->button_height = new_height;

    clean_element();
    setup();
}

Rectangle_Button* Menu_Canvas::create_button(sf::Vector2f pos, const std::string &text) {
    Rectangle_Button *res = new Rectangle_Button;
    res->set_window(window);
    res->set_pos(pos);
    res->set_text_string(text);
    res->rect->setFillColor(sf::Color::White);
    res->text->setFillColor(sf::Color::Black);
    return res;
}

void debug_rect(sf::FloatRect rect,const std::string &message) {
    std::cerr << message << ' ' << rect.position.x << ' ' << rect.position.y << ' ';
    std::cerr << rect.size.x << ' ' << rect.size.y << '\n';
}

void Menu_Canvas::setup() {
    Rectangle_Button *tmp = create_button({102.f, 478.f}, "LOAD GAME");
    tmp->set_press([tmp]() {
        std::cerr << "1. Load button pressed\n";
        // debug_rect(tmp->bound,"Bound");
        // debug_rect(tmp->rect->getGlobalBounds(),"Out rect");
    });
    add_element(tmp);

    tmp = create_button({102.f, 627.f}, "NEW GAME");
    tmp->set_press([]() {
        std::cerr << "2. New button pressed\n";
    });
    add_element(tmp);

    tmp = create_button({102.f, 776.f}, "SETTING");
    tmp->set_press([]() {
        std::cerr << "3. Setting button pressed\n";
    });
    add_element(tmp);

    tmp = create_button({102.f, 925.f}, "EXIT");
    tmp->set_press([tmp]() {
        // std::cerr << "Exit pressed\n";
        if (tmp->window->isOpen()) tmp->window->close();
    });
    add_element(tmp);
}

}
