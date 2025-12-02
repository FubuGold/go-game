#include "../include/UI_renderer.h"

#include <iostream> // Testing only

namespace GUI {

// ---------------------------------------------------
// Canvas implementation

Canvas::~Canvas() {
    window = nullptr;
    gamestate = nullptr;
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

void Canvas::set_gamesate(GameState *gamestate_ptr) {
    gamestate = gamestate_ptr;
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

void Canvas::create_button(sf::Vector2f pos, const std::string &text, Rectangle_Button *rect) {
    rect->set_pos(pos);
    rect->set_text_string(text);
}

Rectangle_Button* Canvas::create_sprite(sf::Vector2f pos, const std::filesystem::path &filename) {
    sf::Texture tmp;
    if (!tmp.loadFromFile(filename)) {
        std::cerr << "Failed to load texture of " << filename.filename() << '\n';
        return nullptr;
    }
    
    auto size_of_texture = tmp.getSize();

    Rectangle_Button *res = new Rectangle_Button(size_of_texture.x, size_of_texture.y, sf::Color::Transparent);
    res->set_texture(filename);
    res->set_pos(pos);
    return res;
}

void Canvas::setup() {}

// ---------------------------------------------------
// Menu implementation

void Menu_Canvas::reset_button_size(float new_width,float new_height) {
    this->button_width = new_width;
    this->button_height = new_height;

    clean_element();
    setup();
}

void debug_rect(sf::FloatRect rect,const std::string &message) {
    std::cerr << message << ' ' << rect.position.x << ' ' << rect.position.y << ' ';
    std::cerr << rect.size.x << ' ' << rect.size.y << '\n';
}

void Menu_Canvas::setup() {
    Rectangle_Button *tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 377.f}, "LOAD GAME", tmp);
    tmp->set_press([tmp]() {
        std::cerr << "MENU: Load button pressed\n";
        // debug_rect(tmp->bound,"Bound");
        // debug_rect(tmp->rect->getGlobalBounds(),"Out rect");
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 528.f}, "NEW GAME", tmp);
    tmp->set_press([&]() {
        std::cerr << "MENU: New button pressed\n";
        if (gamestate) *gamestate = GameState::NewGame;
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 679.f}, "SETTING", tmp);
    tmp->set_press([]() {
        std::cerr << "MENU: Setting button pressed\n";
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 830.f}, "EXIT", tmp);
    tmp->set_press([tmp]() {
        std::cerr << "MENU: Exit button pressed\n";
        if (tmp->window->isOpen()) tmp->window->close();
    });
    add_element(tmp);

    tmp = create_sprite({102, 119}, "assets/game_title.png");
    add_element(tmp);

    tmp = new Rectangle_Button(468.f, 60.f, sf::Color::Transparent, sf::Color::Black, 36);
    create_button({1452, 1020}, "Game version: Sigma_7.2.7", tmp);
    add_element(tmp);

    tmp = create_sprite({1291, 261}, "assets/black_stone.png");
    add_element(tmp);

    tmp = create_sprite({851, 480}, "assets/black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1511, 480}, "assets/black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1291, 699}, "assets/black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1291, 480}, "assets/white_stone.png");
    add_element(tmp);

    tmp = create_sprite({1071, 480}, "assets/white_stone.png");
    add_element(tmp);
}

// ---------------------------------------------------
// NewGame implementation

void NewGame_Canvas::setup() {
    Rectangle_Button *tmp = new Rectangle_Button(1920, 185, {248, 204, 75});
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({0, 0}, "", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(494, 116, sf::Color::Transparent, sf::Color::Black, 96);
    tmp->text->setFont(Config::font[1]);
    tmp->text->setStyle(sf::Text::Bold | sf::Text::Italic);
    create_button({51, 41}, "NEW GAME", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(891, 120, sf::Color::Transparent, sf::Color::Black, 96);
    create_button({51, 903}, "CHOOSE GAME MODE", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(377, 103, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({1487, 943}, "BACK", tmp);
    tmp->set_press([&]() {
        std::cerr << "NEW GAME: Back button pressed\n";
        *gamestate = GameState::Menu;
    });
    add_element(tmp);

    tmp = create_sprite({86, 274}, "assets/2_players.png");
    tmp->set_press([&]() {
        std::cerr << "NEW GAME: 2 players button pressed\n";
        *gamestate = GameState::Gameplay;
    });
    add_element(tmp);

    tmp = create_sprite({1001, 274}, "assets/vs_computer.png");
    tmp->set_press([&]() {
        std::cerr << "NEW GAME: VS computer button pressed\n";
        *gamestate = GameState::AImode;
    });
    add_element(tmp);
}

// ---------------------------------------------------
// AImode implementation

void AImode_Canvas::setup() {
    Rectangle_Button *tmp = new Rectangle_Button(1920, 185, {248, 204, 75});
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({0, 0}, "", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(386, 116, sf::Color::Transparent, sf::Color::Black, 96);
    tmp->text->setFont(Config::font[1]);
    tmp->text->setStyle(sf::Text::Bold | sf::Text::Italic);
    create_button({51, 41}, "AI MODE", tmp);
    add_element(tmp);

    
    tmp = new Rectangle_Button(377, 103, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({1487, 943}, "BACK", tmp);
    tmp->set_press([&]() {
        std::cerr << "AI MODE: Back button pressed\n";
        *gamestate = GameState::NewGame;
    });
    add_element(tmp);
    
    tmp = new Rectangle_Button(877, 120, sf::Color::Transparent, sf::Color::Black, 96);
    create_button({51, 904}, "CHOOSE DIFFICULTY", tmp);
    add_element(tmp);

    tmp = create_sprite({94, 252}, "assets/easy.png");
    tmp->set_press([]() {
        std::cerr << "AI MODE: Easy button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({718, 252}, "assets/normal.png");
    tmp->set_press([]() {
        std::cerr << "AI MODE: Normal button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({1339, 252}, "assets/hard.png");
    tmp->set_press([]() {
        std::cerr << "AI MODE: Hard button pressed\n";
    });
    add_element(tmp);
}

// ---------------------------------------------------
// Gameplay implementation

void Gameplay_Canvas::setup() {
    Rectangle_Button *tmp = create_sprite({38, 399}, "assets/save.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([]() {
        std::cerr << "Gameplay: Save button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({38, 534}, "assets/back.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        //Should ask the user if they want to save before leaving
        std::cerr << "Gameplay: Back button pressed\n";
        *gamestate = GameState::Menu;
    });
    add_element(tmp);

    //Will update this later, after we add music to the game
    tmp = create_sprite({38, 669}, "assets/music_on.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        std::cerr << "Gameplay: Music button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({38, 669}, "assets/music_off.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        std::cerr << "Gameplay: Music button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({1785, 399}, "assets/redo.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        std::cerr << "Gameplay: Redo button pressed\n";
    });
    add_element(tmp);

    tmp = create_sprite({1785, 534}, "assets/undo.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        std::cerr << "Gameplay: Undo button pressed\n";
    });
    add_element(tmp);
}

}
