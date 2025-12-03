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
    tmp->set_press([&]() {
        std::cerr << "MENU: Setting button pressed\n";
        if (gamestate) *gamestate = GameState::Setting;
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

void Gameplay_Canvas::draw_stone() {
    std::cerr << stones.size() << '\n';
    // std::cerr << stones.back()->cur_sprite << '\n';
    Board_Stone *test = stones.back();
    // test->draw();
    return;
    for (Board_Stone *cur : stones) {
        return;
        if (!cur) {
            std::cerr << "alo wtf\n";
            continue;
        }
        if (current_board.get_state(cur->board_pos.x, cur->board_pos.y) != '.') {
            window->draw(*cur->stone_sprite[cur->cur_sprite]);
        }
    }
}

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

    tmp = new Rectangle_Button(1000, 1000, {194, 113, 0});
    create_button({460, 40}, "", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(900, 900, {255, 199, 46});
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({510, 90}, "", tmp);
    add_element(tmp);

    for (int i = 1; i <= 17; i++) { //Vertical lines
        tmp = new Rectangle_Button(4, 900, sf::Color::Black);
        create_button({static_cast<float>(510 + i * 50), 90}, "", tmp);
        add_element(tmp);
    }

    for (int i = 1; i <= 17; i++) { //Horizontal lines
        tmp = new Rectangle_Button(900, 4, sf::Color::Black);
        create_button({510, static_cast<float>(90 + i * 50)}, "", tmp);
        add_element(tmp);
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Board_Stone *cur_stone = new Board_Stone({i, j});
            cur_stone->set_press([i, j, cur_stone]() {
                if (add_move(Move(i, j, current_board.get_turn() ? 'X' : 'O'))) {
                    cur_stone->cur_sprite = current_board.get_turn();
                }
            });
            cur_stone->set_pos({static_cast<float>(510 + 50 * j), static_cast<float>(90 + 50 * i)});
            cur_stone->set_window(window);
            stones.push_back(cur_stone);
        }
    }
}

// ---------------------------------------------------
// Setting implementation

void Setting_Canvas::setup() {
    Rectangle_Button *tmp = new Rectangle_Button(1920, 185, sf::Color(0xF8,0xCC,0x4B), sf::Color::Black, 96);
    add_element(tmp);
    
    tmp = new Rectangle_Button(312,116, sf::Color::Transparent, sf::Color::Black, 96);
    tmp->text->setFont(Config::font[1]);
    tmp->text->setStyle(sf::Text::Bold | sf::Text::Italic);
    create_button({51,35},"Setting",tmp);
    add_element(tmp);

    // All setting. Position will calculated locally to the box
    sf::Vector2f tmp_pos = {393,250};
    // The box
    tmp = new Rectangle_Button(1042, 731, sf::Color(0x94,0xCC,0x47));
    tmp->set_pos(tmp_pos);
    add_element(tmp);

    tmp = new Rectangle_Button(143, 63, sf::Color::Transparent, sf::Color(0xBC,0x48,0x00), 50);
    create_button(tmp_pos + sf::Vector2f(37,24),"AUDIO",tmp);
    add_element(tmp);
    
    H_Slider *slider = new H_Slider(
        512, 31, 100, 100, "General", {255, 255, 255}, sf::Color::Red, 40
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,99));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,90));
    slider->set_name_pos(tmp_pos + sf::Vector2f(104,90));
    slider->set_change([slider](){
        std::cerr << "Current general value is: " << slider->value << '\n';
    });
    add_element(slider);

    slider = new H_Slider(
        512, 31, 100, 100, "Music", {255, 255, 255}, sf::Color::Red, 40
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,161));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,152));
    slider->set_name_pos(tmp_pos + sf::Vector2f(145,152));
    slider->set_change([slider](){
        std::cerr << "Current music value is: " << slider->value << '\n';
    });
    add_element(slider);

    slider = new H_Slider(
        512, 31, 100, 100, "SFX", {255, 255, 255}, sf::Color::Red, 40
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,223));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,214));
    slider->set_name_pos(tmp_pos + sf::Vector2f(165,214));
    slider->set_change([slider](){
        std::cerr << "Current SFX value is: " << slider->value << '\n';
    });
    add_element(slider);

    tmp = new Rectangle_Button(143, 63, sf::Color::Transparent, sf::Color(0xBC,0x48,0x00), 50);
    create_button(tmp_pos + sf::Vector2f(37,285),"OTHER",tmp);
    add_element(tmp);

    Droplist *droplist = new Droplist(
        406, 75, "Theme", {247, 222, 57}, sf::Color::Black, sf::Color(0xBA,0xBA,0xBA), sf::Color::Black
    );
    droplist->title_button->rect->setOutlineColor(sf::Color::Black);
    droplist->title_button->rect->setOutlineThickness(-4);
    droplist->set_pos(tmp_pos + sf::Vector2f(84,379));
    droplist->add_element("Theme 1",[](){
        std::cerr << "Theme 1 selected\n";
    });
    droplist->add_element("Theme 2",[](){
        std::cerr << "Theme 2 selected\n";
    });
    droplist->add_element("Theme 3",[](){
        std::cerr << "Theme 3 selected\n";
    });
    add_element(droplist);
    droplist = new Droplist(
        406, 75, "Song", {247, 222, 57}, sf::Color::Black, sf::Color(0xBA,0xBA,0xBA), sf::Color::Black
    );
    droplist->title_button->rect->setOutlineColor(sf::Color::Black);
    droplist->title_button->rect->setOutlineThickness(-4);
    droplist->set_pos(tmp_pos + sf::Vector2f(555,379));
    droplist->add_element("Song 1",[](){
        std::cerr << "Song 1 selected\n";
    });
    droplist->add_element("Song 2",[](){
        std::cerr << "Song 2 selected\n";
    });
    droplist->add_element("Song 3",[](){
        std::cerr << "Song 3 selected\n";
    });
    add_element(droplist);

    tmp = new Rectangle_Button(377, 103, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({1487, 943}, "BACK", tmp);
    tmp->set_press([&]() {
        std::cerr << "NEW GAME: Back button pressed\n";
        *gamestate = GameState::Menu;
    });
    add_element(tmp);
}

}
