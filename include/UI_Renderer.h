#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <SFML/Graphics.hpp>

const sf::Font font = sf::Font("font/Jua-Regular.ttf");

struct Rectangle_Button {
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
    Rectangle_Button button_exit, button_load_game, button_new_game, button_setting;
    float button_height, button_width;

public:

    GameMenu(float button_height = 103.f, float button_width = 377.f); //In case you want to change the default value, please change it in game_UI.h, not this
    
    /**
     * @brief Set the initial value object
     * 
     * @param button 
     * @param position 
     */
    void set_initial_value(Rectangle_Button &button, const sf::Vector2f &position);

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
     * @param window 
     * @return true 
     * @return false 
     */
    bool exit_button_active(const sf::RenderWindow &window);

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