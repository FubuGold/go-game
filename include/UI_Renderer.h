#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <SFML/Graphics.hpp>
#include "UI_element.h"
#include <vector>

const sf::Font font = sf::Font("font/Jua-Regular.ttf");

namespace GUI {
    class Canvas {
    private: 
        std::vector<Element*> element_p;
    public:
        void add_element(Element *new_element) {
            element_p.push_back(new_element);
        }
        
        void draw() {
            for (Element* ptr : element_p) {
                ptr->draw();
            }
        }

        void poll_event(const sf::Event &e) {
            for (Element* ptr : element_p) {
                ptr->poll_event(e);
            }
        }
    };

    
}

class Rectangle_Button {
public:
    sf::RectangleShape rect;
    sf::Text text = sf::Text(font);

    /**
     * @brief Set the text string object
     * 
     * @param str 
     */
    void set_text_string(const std::string &str);

    /**
     * @brief Check mouse hovering on the button
     * 
     * @param window 
     * @return true 
     * @return false 
     */
    bool isHover(const sf::RenderWindow &window) const;

    /**
     * @brief Check mouse clicking on the button
     * 
     * @param window 
     * @return true 
     * @return false 
     */
    bool isClicked(const sf::RenderWindow &window) const;
};

class GameMenu {
private:
    Rectangle_Button button_load_game, button_new_game, button_setting;
    GUI::Rectangle_Button button_exit_text;
    float button_height, button_width;

    sf::RenderWindow *window;

public:

    GameMenu(float button_height = 103.f, float button_width = 377.f);
    
    /**
     * @brief Set the initial value object
     * 
     * @param button 
     * @param position 
     */
    void set_initial_value(Rectangle_Button &button, const sf::Vector2f &position);

    /**
     * @brief Set the window for elements
     * 
     * @param render_win 
     */
    void set_window(sf::RenderWindow *render_win);

    /**
     * @brief Check if the input button is clicked
     * 
     * @param button 
     * @param window 
     * @return true 
     * @return false 
     */
    bool button_active(const Rectangle_Button &button, const sf::RenderWindow &window);

    /**
     * @brief Check if the exit button is clicked
     * 
     * @param e The event received
     */
    void exit_button_poll(const sf::Event &e);

    /**
     * @brief Draw the button to the window
     * 
     * @param button 
     * @param window 
     */
    void draw_button(const Rectangle_Button &button, sf::RenderWindow &window);

    /**
     * @brief Responsible for drawing all necessary elements in the game menu
     * 
     * @param window 
     */
    void draw(sf::RenderWindow &window);
};

#endif