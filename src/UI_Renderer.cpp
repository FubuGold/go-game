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
    if (const sf::Event::MouseButtonPressed* mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>())
        std::cerr << "Poll event complete\n";
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
    Rectangle_Button *tmp = new Rectangle_Button(626, 127, {0xFD,0xD3,0x4B}, sf::Color::Red,55);
    Popup *popup = new Popup(1000,626,127,sf::Color::Transparent);
    sf::Text *fail_load = new sf::Text(Config::font[0]);
    popup->set_pos(647,457);
    create_button({647,457},"No saved game to load",tmp);
    tmp->rect->setOutlineThickness(-4);
    tmp->rect->setOutlineColor(sf::Color::Black);
    popup->add_element(tmp);
    // Added at the bottom (the last on to at) to render the highest layer

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 377.f}, "LOAD GAME", tmp);
    tmp->set_press([this, popup]() {
        Config::sfx[0].play();
        std::cerr << "MENU: Load button pressed\n";
        if (current_board.load_game()) {
            *gamestate = GameState::Gameplay;
        }
        else {
            popup->enable_popup();
        }
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 528.f}, "NEW GAME", tmp);
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "MENU: New button pressed\n";
        if (gamestate) *gamestate = GameState::NewGame;
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 679.f}, "SETTING", tmp);
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "MENU: Setting button pressed\n";
        if (gamestate) *gamestate = GameState::Setting;
    });
    add_element(tmp);

    tmp = new Rectangle_Button(button_width, button_height);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({102.f, 830.f}, "EXIT", tmp);
    tmp->set_press([tmp]() {
        Config::sfx[0].play();
        std::cerr << "MENU: Exit button pressed\n";
        if (tmp->window->isOpen()) tmp->window->close();
    });
    add_element(tmp);

    tmp = create_sprite({102, 119}, "assets/game_title.png");
    add_element(tmp);

    tmp = new Rectangle_Button(468.f, 60.f, sf::Color::Transparent, sf::Color::Black, 36);
    create_button({1452, 1020}, "Game version: Sigma_7.2.7", tmp);
    add_element(tmp);

    tmp = create_sprite({1291, 261}, "assets/stones/theme_1_black_stone.png");
    add_element(tmp);

    tmp = create_sprite({851, 480}, "assets/stones/theme_1_black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1511, 480}, "assets/stones/theme_1_black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1291, 699}, "assets/stones/theme_1_black_stone.png");
    add_element(tmp);

    tmp = create_sprite({1291, 480}, "assets/stones/theme_1_white_stone.png");
    add_element(tmp);

    tmp = create_sprite({1071, 480}, "assets/stones/theme_1_white_stone.png");
    add_element(tmp);

    add_element(popup);
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
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "NEW GAME: Back button pressed\n";
        *gamestate = GameState::Menu;
    });
    add_element(tmp);

    tmp = create_sprite({86, 274}, "assets/2_players.png");
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "NEW GAME: 2 players button pressed\n";
        *gamestate = GameState::Gameplay;
        current_board.reset();
    });
    add_element(tmp);

    tmp = create_sprite({1001, 274}, "assets/vs_computer.png");
    tmp->set_press([this]() {
        Config::sfx[0].play();
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
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "AI MODE: Back button pressed\n";
        *gamestate = GameState::NewGame;
    });
    add_element(tmp);
    
    tmp = new Rectangle_Button(877, 120, sf::Color::Transparent, sf::Color::Black, 96);
    create_button({51, 904}, "CHOOSE DIFFICULTY", tmp);
    add_element(tmp);

    tmp = create_sprite({94, 252}, "assets/easy.png");
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "AI MODE: Easy button pressed\n";
        *gamestate = GameState::Gameplay;
        current_board.reset();
        current_board.board_diff = Difficulty::EASY;
    });
    add_element(tmp);

    tmp = create_sprite({718, 252}, "assets/normal.png");
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "AI MODE: Normal button pressed\n";
        *gamestate = GameState::Gameplay;
        current_board.reset();
        current_board.board_diff = Difficulty::MEDIUM;
    });
    add_element(tmp);

    tmp = create_sprite({1339, 252}, "assets/hard.png");
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "AI MODE: Hard button pressed\n";
        *gamestate = GameState::Gameplay;
        current_board.reset();
        current_board.board_diff = Difficulty::HARD;
    });
    add_element(tmp);
}

// ---------------------------------------------------
// Gameplay implementation

void Gameplay_Canvas::draw_stone() {
    for (Board_Stone *cur : stones) {
        // std::cerr << cur->board_pos.x << ' ' << cur->board_pos.y << ' ' << current_board.get_state(cur->board_pos.x, cur->board_pos.y) << '\n';
        if (current_board.get_state(cur->board_pos.x, cur->board_pos.y) != '.') {
            // std::cerr << "drawing stone\n";
            char tmp = current_board.get_state(cur->board_pos.x, cur->board_pos.y);
            // std::cerr << tmp << '\n';
            window->draw(*(cur->stone_sprite[Config::selected_theme_number][tmp == 'X']));
        }
    }
    if (current_board.pass >= 2) {
        end_game->enable_popup();
        end_game->draw();
        return;
    }
    if (hover_x == -1 || hover_y == -1) return;
    int new_id = hover_x * BOARD_SIZE + hover_y;
    Board_Stone *cur = stones[new_id];
    if (current_board.get_state(cur->board_pos.x, cur->board_pos.y) == '.') {
        sf::Sprite tmp = *(cur->stone_sprite[Config::selected_theme_number][current_board.get_turn()]);
        tmp.setColor({255,255,255,127});
        window->draw(tmp); 
    }
}

void Gameplay_Canvas::poll_event(const std::optional<sf::Event> &e) {
    if (current_board.pass >= 2) {
        end_game->poll_event(e);
    }
    else Canvas::poll_event(e);
    
}

void Gameplay_Canvas::setup() {
    hover_x = -1, hover_y = -1;
    current_board.pass = 0;

    Popup *save_popup = new Popup(1000,218,82, {0xFF,0xB4,0x4c});
    save_popup->set_pos(38,297);
    sf::Text *save_text = new sf::Text(Config::font[0]);
    save_text->setString("Game saved");
    save_text->setFillColor(sf::Color::Red);
    save_text->setPosition({38+25,297+22});
    save_popup->add_drawable(save_text);
    add_element(save_popup);

    Rectangle_Button *tmp = create_sprite({38, 399}, "assets/gameplay_buttons/save.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([save_popup]() {
        Config::sfx[2].play();
        std::cerr << "Gameplay: Save button pressed\n";
        save_popup->enable_popup();
        current_board.save_game();
    });
    add_element(tmp);

    tmp = create_sprite({38, 534}, "assets/gameplay_buttons/back.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        //Should ask the user if they want to save before leaving
        Config::sfx[0].play();
        std::cerr << "Gameplay: Back button pressed\n";
        current_board.reset();
        *gamestate = GameState::Menu;
        current_board.board_diff = Difficulty::NONE;
    });
    add_element(tmp);

    tmp = create_sprite({38, 669}, "assets/gameplay_buttons/music_off.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    add_element(tmp);
    
    tmp = create_sprite({38, 669}, "assets/gameplay_buttons/music_on.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([tmp]() {
        Config::sfx[0].play();
        std::cerr << "Gameplay: Music button pressed\n";

        if (Config::music_on) {
            Config::bgm[Config::selected_bgm].pause();
            tmp->sprite->setScale({0.f, 0.f});
        }
        else {
            Config::bgm[Config::selected_bgm].play();
            tmp->sprite->setScale({1.f, 1.f});
        }
        Config::music_on ^= 1;
    });
    add_element(tmp);

    tmp = create_sprite({38, 804}, "assets/gameplay_buttons/reset.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([&]() {
        Config::sfx[0].play();
        std::cerr << "Gameplay: Reset button pressed\n";
        Difficulty tmp = current_board.board_diff;
        current_board.reset();
        current_board.board_diff = tmp;
    });
    add_element(tmp);

    end_game = new Popup(-1,1170,540,{0xFF,0xF9,0xB7});
    sf::Vector2f local_pos = {375,265};
    end_game->set_pos(local_pos);

    tmp = new Rectangle_Button(668,120,sf::Color::Transparent,sf::Color::Red,96);
    create_button(local_pos + sf::Vector2f(251,41),"GAME FINISHED",tmp);
    end_game->add_element(tmp);
    tmp = new Rectangle_Button(221,75,sf::Color::Transparent,{0,0xB3,0x0C},60);
    create_button(local_pos + sf::Vector2f(309,161),"RESULT:",tmp);
    end_game->add_element(tmp);
    tmp = new Rectangle_Button(168,75,sf::Color::Transparent,{0x52,0x44,0xA2},60);
    create_button(local_pos + sf::Vector2f(177,252),"Score:",tmp);
    end_game->add_element(tmp);
    tmp = new Rectangle_Button(180,75,sf::Color::Transparent,sf::Color::Black,60);
    create_button(local_pos + sf::Vector2f(421,252),"BLACK",tmp);
    end_game->add_element(tmp);
    tmp = new Rectangle_Button(171,75,sf::Color::Transparent,sf::Color::Black,60);
    create_button(local_pos + sf::Vector2f(421,332),"WHITE",tmp);
    end_game->add_element(tmp);

    sf::Text *result_string = new sf::Text(Config::font[0]);
    result_string->setPosition(local_pos + sf::Vector2f(546,157));
    result_string->setFillColor({0x14,0x3B,0xFF});
    result_string->setCharacterSize(60);
    end_game->add_drawable(result_string);
    int *black_score = new int(0), *white_score = new int(0);

    Dynamic_Text *score_text = new Dynamic_Text(black_score,{0xBF,0,0xFF},60);
    score_text->set_pos(local_pos + sf::Vector2f(665,252));
    end_game->add_element(score_text);
    score_text = new Dynamic_Text(white_score,{0xBF,0,0xFF},60);
    score_text->set_pos(local_pos + sf::Vector2f(665,332));
    end_game->add_element(score_text);
    
    tmp = new Rectangle_Button(292, 85, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button(local_pos + sf::Vector2f(858,432), "BACK", tmp);
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "GAMEPLAY: Back button pressed\n";
        *gamestate = GameState::Menu;
        end_game->disable_popup();
    });
    end_game->add_element(tmp);

    tmp = new Rectangle_Button(292, 85, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button(local_pos + sf::Vector2f(24,432), "NEW GAME", tmp);
    tmp->set_press([this]() {
        Config::sfx[0].play();
        std::cerr << "GAMEPLAY: New game button pressed\n";
        *gamestate = GameState::NewGame;
        end_game->disable_popup();
    });
    end_game->add_element(tmp);
    // Add to the list at the end to draw it on the top layer

    tmp = create_sprite({1785, 264}, "assets/gameplay_buttons/pass.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([=]() {
        Config::sfx[0].play();
        std::cerr << "Gameplay: Pass button pressed\n";
        current_board.add_move(Move());
        current_board.update_turn();
        current_board.pass++;
        if (current_board.pass == 2) {
            int black,white;
            std::tie(black,white) = scoring(current_board);
            *black_score = black;
            *white_score = white;
            if (black > white) {
                result_string->setString("BLACK WON");
            }
            else if (black < white) {
                result_string->setString("WHITE WON");
            }
            else {
                result_string->setString("TIE");
            }
            // end_game->enable_popup();
        }
        else {
            if (current_board.board_diff != Difficulty::NONE) {
            auto new_move = ai_move(current_board.board_diff); //For debugging if needed
            std::cerr << "GAMEPLAY: AI made a move: (" << new_move.pos_x << ", " << new_move.pos_y << ")\n";
            if (new_move.stone_type == '.') {
                std::cerr << "Out of valid move\n";
                current_board.pass++;
                current_board.update_turn();
                return;
            }
            bool ko_threat = check_ko_threat(new_move);
            if (ko_threat) Config::sfx[3].play();
            else Config::sfx[1].play();
            std::cerr << "Adding AI move\n";
            add_move(new_move);
            std::cerr << "Complete adding AI move\n";
            current_board.update_turn();
            while(window->pollEvent()) {}
        }
        }
    });
    add_element(tmp);

    tmp = create_sprite({1785, 399}, "assets/gameplay_buttons/redo.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([]() {
        Config::sfx[0].play();
        std::cerr << "Gameplay: Redo button pressed\n";
        if (!current_board.check_empty_undo_list()) {
            current_board.redo_move();
        }
        if (!current_board.check_empty_undo_list() && current_board.board_diff != Difficulty::NONE) {
            current_board.redo_move();
        }
    });
    add_element(tmp);

    tmp = create_sprite({1785, 534}, "assets/gameplay_buttons/undo.png");
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_press([]() {
        Config::sfx[0].play();
        std::cerr << "Gameplay: Undo button pressed\n";
        if (!current_board.check_empty_move_list()) {
            current_board.undo_move();
        }
        if (!current_board.check_empty_move_list() && current_board.board_diff != Difficulty::NONE) {
            current_board.undo_move();
        }
    });
    add_element(tmp);

    //White score display
    tmp = new Rectangle_Button(379, 235);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({38, 40}, "", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(268, 97, sf::Color::Transparent, sf::Color::Black, 80);
    tmp->text->setFont(Config::font[1]);
    tmp->text->setStyle(sf::Text::Bold);
    create_button({94, 53}, "WHITE", tmp);
    add_element(tmp);

    tmp = create_sprite({266, 164}, "assets/stones/theme_1_black_stone.png");
    tmp->sprite->setScale({0.375f, 0.375f});
    add_element(tmp);

    Dynamic_Text *d_text = new Dynamic_Text(&current_board.captured_black, sf::Color::Red,70);
    d_text->set_pos(130,158);
    add_element(d_text);

    //Black score display
    tmp = new Rectangle_Button(379, 235, {103, 55, 0});
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({1502, 804}, "", tmp);
    add_element(tmp);

    tmp = new Rectangle_Button(273, 97, sf::Color::Transparent, sf::Color::White, 80);
    tmp->text->setFont(Config::font[1]);
    tmp->text->setStyle(sf::Text::Bold);
    create_button({53 + 1502, 13 + 804}, "BLACK", tmp);
    add_element(tmp);

    tmp = create_sprite({228 + 1502, 124 + 804}, "assets/stones/theme_1_white_stone.png");
    tmp->sprite->setScale({0.375f, 0.375f});
    add_element(tmp);

    d_text = new Dynamic_Text(&current_board.captured_white, {255, 7, 164}, 70);
    d_text->set_pos(92 + 1502, 118 + 804);
    add_element(d_text);

    //Board setups below
    tmp = new Rectangle_Button(1000, 1000, {194, 113, 0});
    create_button({460, 40}, "", tmp);
    add_element(tmp);

    for (auto &tex : Config::theme_texture) {
        tex.setSmooth(true);
    }

    for (auto &p : Config::theme) {
        p.setPosition({510, 90});
    }

    tmp = new Rectangle_Button(900, 900, sf::Color::Transparent);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    tmp->set_sprite(&Config::selected_theme);
    tmp->set_press([tmp]() {
        // std::cerr << tmp->sprite << ' ' << &Config::selected_theme << '\n';
    });
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

    Popup *invalid_move = new Popup(1000,420,83, {0xFF,0xB4,0x4c});
    invalid_move->set_pos(1480,140);
    sf::Text *text = new sf::Text(Config::font[0]);
    text->setString("You can't make this action");
    text->setFillColor(sf::Color::Red);
    text->setPosition({1480+25,140+22});
    invalid_move->add_drawable(text);
    add_element(invalid_move);

    Popup *ko_threat_popup = new Popup(2000,274,72, {0xFF,0xB4,0x4c});
    ko_threat_popup->set_pos(160,409);
    text = new sf::Text(Config::font[0]);
    text->setString("KO threat!");
    text->setFillColor(sf::Color::Red);
    text->setPosition({160+63,409+16});
    ko_threat_popup->add_drawable(text);
    add_element(ko_threat_popup);

    Popup *capture_popup = new Popup(2000,274,72, {0xFF,0xB4,0x4c});
    capture_popup->set_pos(160,490);
    text = new sf::Text(Config::font[0]);
    text->setString("Stone captured!");
    text->setFillColor(sf::Color::Red);
    text->setPosition({160+24,490+16});
    capture_popup->add_drawable(text);
    add_element(capture_popup);

    tmp = new Rectangle_Button(229,114,{0xFF,0xB4,0x4c});
    tmp->rect->setOutlineThickness(4.f);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->set_pos(182,715);
    add_element(tmp);

    tmp = new Rectangle_Button(186,40,sf::Color::Transparent,sf::Color::Red,32);
    create_button({182 + 22, 715 + 17},"Current turn",tmp);
    add_element(tmp);

    Rectangle_Button *turn_text = new Rectangle_Button(79,40,sf::Color::Transparent,sf::Color::Black,32);
    create_button({182 + 75, 715 + 57},"Black",turn_text);
    add_element(turn_text);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Board_Stone *cur_stone = new Board_Stone({i, j});
            // cur_stone->rect->setOutlineThickness(-1); // Debug
            // element_l.push_back(cur_stone); // This will handle bound and events
            add_element(cur_stone);
            cur_stone->set_pos({static_cast<float>(510 + 50 * j) - cur_stone->button_width / 2.f, static_cast<float>(90 + 50 * i) - cur_stone->button_height / 2.f});
            
            for (int k = 0; k < 3; k++) {
                sf::FloatRect cur_bound = cur_stone->stone_sprite[k][0]->getLocalBounds();
                cur_stone->stone_sprite[k][0]->setOrigin({cur_bound.size.x / 2.f, cur_bound.size.y / 2.f});
                cur_bound = cur_stone->stone_sprite[k][1]->getLocalBounds();
                cur_stone->stone_sprite[k][1]->setOrigin({cur_bound.size.x / 2.f, cur_bound.size.y / 2.f});

                cur_stone->stone_sprite[k][0]->setPosition({static_cast<float>(510 + 50 * j + 1), static_cast<float>(90 + 50 * i + 1)});
                cur_stone->stone_sprite[k][1]->setPosition({static_cast<float>(510 + 50 * j + 1), static_cast<float>(90 + 50 * i + 1)});
                
                cur_stone->stone_sprite[k][0]->setScale({0.225, 0.225});
                cur_stone->stone_sprite[k][1]->setScale({0.225, 0.225});
            }

            cur_stone->set_window(window);
            cur_stone->set_press([this,i,j,cur_stone,invalid_move,capture_popup,ko_threat_popup,turn_text]() {
                std::cerr << "GAMEPLAY: Intersection (" << i << ", " << j << ") pressed\n";
                bool ko_threat = check_ko_threat(Move(i, j, current_board.get_turn() ? 'X' : 'O'));
                int tmp1 = current_board.get_turn() ? current_board.captured_white : current_board.captured_black;
                if (add_move(Move(i, j, current_board.get_turn() ? 'X' : 'O'))) {
                    int tmp2 = current_board.get_turn() ? current_board.captured_white : current_board.captured_black;
                    if (ko_threat) Config::sfx[3].play();
                    else if (tmp2 > tmp1) {
                        capture_popup->enable_popup();
                        Config::sfx[4].play();
                    }
                    else {
                        Config::theme_sfx[Config::selected_theme_number][current_board.get_turn()].play();
                    }
                    
                    std::cerr << "Ko threat: " << ko_threat << '\n';
                    std::cerr << "GAMEPLAY: Board add move\n";
                    
                    // Move pop-up
                    if (ko_threat) ko_threat_popup->enable_popup();
                    

                    window->draw(*(cur_stone->stone_sprite[Config::selected_theme_number][current_board.get_turn()]));
                    window->display();
                    
                    current_board.update_turn();
                    current_board.pass = 0;

                    if (current_board.get_turn()) {
                        turn_text->text->setString("Black");
                        turn_text->text->setFillColor(sf::Color::Black);
                        turn_text->set_pos({182 + 75, 715 + 57});
                    }
                    else {
                        turn_text->text->setString("White");
                        turn_text->text->setFillColor(sf::Color::White);
                        turn_text->set_pos({182 + 75, 715 + 57});
                    }

                    if (current_board.board_diff != Difficulty::NONE) {
                        auto new_move = ai_move(current_board.board_diff); //For debugging if needed
                        std::cerr << "GAMEPLAY: AI made a move: (" << new_move.pos_x << ", " << new_move.pos_y << ")\n";
                        if (new_move.stone_type == '.') {
                            std::cerr << "Out of valid move\n";
                            current_board.pass++;
                            current_board.update_turn();
                            return;
                        }

                        ko_threat = check_ko_threat(Move(new_move));

                        std::cerr << "Adding AI move\n";
                        tmp1 = current_board.get_turn() ? current_board.captured_white : current_board.captured_black;
                        add_move(new_move);
                        tmp2 = current_board.get_turn() ? current_board.captured_white : current_board.captured_black;
                        std::cerr << "Complete adding AI move\n";
                        
                        if (ko_threat) Config::sfx[3].play();
                        else if (tmp2 > tmp1) {
                            capture_popup->enable_popup();
                            Config::sfx[4].play();
                        }
                        else {
                            Config::theme_sfx[Config::selected_theme_number][current_board.get_turn()].play();
                        }
                        

                        current_board.update_turn();
                        while(window->pollEvent()) {}

                        if (current_board.get_turn()) {
                            turn_text->text->setString("Black");
                            turn_text->text->setFillColor(sf::Color::Black);
                            turn_text->set_pos({182 + 75, 715 + 57});
                        }
                        else {
                            turn_text->text->setString("White");
                            turn_text->text->setFillColor(sf::Color::White);
                            turn_text->set_pos({182 + 75, 715 + 57});
                        }
                    }
                }
                else {
                    invalid_move->enable_popup();
                }
            });

            cur_stone->set_hover([this,i,j]() {
                hover_x = i, hover_y = j;
            });
            stones.push_back(cur_stone);
        }
    }
    add_element(end_game);
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
        512, 31, 100, 100, "General", {255, 255, 255}, sf::Color::Red, 40, sf::Color::Black, Config::audio_volume[0]
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,99));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,90));
    slider->set_name_pos(tmp_pos + sf::Vector2f(104,90));
    slider->set_change([slider](){
        Config::sfx[0].play();
        std::cerr << "Current general value is: " << slider->value << '\n';
        Config::audio_volume[0] = slider->value;
        Config::update_volume();
    });
    add_element(slider);

    slider = new H_Slider(
        512, 31, 100, 100, "Music", {255, 255, 255}, sf::Color::Red, 40, sf::Color::Black, Config::audio_volume[1]
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,161));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,152));
    slider->set_name_pos(tmp_pos + sf::Vector2f(145,152));
    slider->set_change([slider](){
        Config::sfx[0].play();
        std::cerr << "Current music value is: " << slider->value << '\n';
        Config::audio_volume[1] = slider->value;
        Config::update_volume();
    });
    add_element(slider);

    slider = new H_Slider(
        512, 31, 100, 100, "SFX", {255, 255, 255}, sf::Color::Red, 40, sf::Color::Black, Config::audio_volume[2]
    );
    slider->set_pos(tmp_pos + sf::Vector2f(265,223));
    slider->set_display_value_pos(tmp_pos + sf::Vector2f(804,214));
    slider->set_name_pos(tmp_pos + sf::Vector2f(165,214));
    slider->set_change([slider](){
        Config::sfx[0].play();
        std::cerr << "Current SFX value is: " << slider->value << '\n';
        Config::audio_volume[2] = slider->value;
        Config::update_volume();
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
        Config::sfx[0].play();
        Config::selected_theme_number = 0;
        Config::selected_theme = Config::theme[0];
        std::cerr << "Theme 1 selected\n";
    });
    droplist->add_element("Theme 2",[](){
        Config::sfx[0].play();
        Config::selected_theme_number = 1;
        Config::selected_theme = Config::theme[1];
        std::cerr << "Theme 2 selected\n";
    });
    droplist->add_element("Theme 3",[](){
        Config::sfx[0].play();
        Config::selected_theme_number = 2;
        Config::selected_theme = Config::theme[2];
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
        Config::sfx[0].play();
        std::cerr << "Song 1 selected\n";
        Config::change_music(0);
    });
    droplist->add_element("Song 2",[](){
        Config::sfx[0].play();
        std::cerr << "Song 2 selected\n";
        Config::change_music(1);
    });
    droplist->add_element("Song 3",[](){
        Config::sfx[0].play();
        std::cerr << "Song 3 selected\n";
        Config::change_music(2);
    });
    add_element(droplist);

    tmp = new Rectangle_Button(377, 103, {255, 183, 106}, sf::Color::Black, 60);
    tmp->rect->setOutlineColor(sf::Color::Black);
    tmp->rect->setOutlineThickness(4.f);
    create_button({1487, 943}, "BACK", tmp);
    tmp->set_press([&]() {
        Config::sfx[0].play();
        std::cerr << "NEW GAME: Back button pressed\n";
        Config::save_config();
        *gamestate = GameState::Menu;
    });
    add_element(tmp);
}

}
