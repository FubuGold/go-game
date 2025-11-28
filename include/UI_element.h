#if !defined(UI_ELEMENT_H)
#define UI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "config_handler.h"

namespace GUI {

    enum State {
        ST_DEFAULT,
        ST_PRESSED,
        ST_HOVERED
    };

    using callback_t = std::function<void()>;

    class Element {
    
    protected:
        sf::RenderWindow *window;
        callback_t hover_callback, press_callback, release_callback;

        State cur_state;
    
        std::vector<sf::Drawable*> parts;

        void press();
        void release();

        void change_state(State state);

        bool contain_pos(float x,float y);
        bool contain_pos(sf::Vector2f point);

    public:
        sf::Vector2f pos;
        sf::FloatRect bound; // Global bounding box

        ~Element();

        /**
         * @brief Set callback function when hover on
         * 
         * @param callback 
         */
        void set_hover(callback_t callback);

        /**
         * @brief Set callback function when pressed
         * 
         * @param callback 
         */
        void set_press(callback_t callback);

        /**
         * @brief Set callback function when released
         * 
         * @param callback 
         */
        void set_release(callback_t callback);

        /**
         * @brief Set the pos object
         * 
         * @param pos_x 
         * @param pos_y 
         */
        virtual void set_pos(float pos_x,float pos_y);
        virtual void set_pos(sf::Vector2f new_pos);
        
        /**
         * @brief Get the position object
         * 
         * @return sf::Vector2f 
         */
        sf::Vector2f get_pos();
        
        /**
         * @brief Get the size object
         * 
         * @return sf::Vector2f 
         */
        sf::Vector2f get_size();
        
        /**
         * @brief Set the window object
         * 
         * @param window 
         */
        void set_window(sf::RenderWindow *render_win_ptr);
        
        /**
         * @brief Draw the element
         * 
         */
        virtual void draw();
        
        /**
         * @brief Update status when receive event
         * 
         * @param e 
         */
        virtual void poll_event(const sf::Event &e);
        
        /**
         * @brief Update bounding box (for event handling)
         * 
         */
        void update_bound();
    };
    
    class Rectangle_Button : public Element {
    public:
        sf::RectangleShape *rect = new sf::RectangleShape();
        sf::Text *text = new sf::Text(Config::font);
        
        Rectangle_Button(float button_width = 337.f,float button_height = 103.f);
        void set_text_string(const std::string &str);

        void set_pos(float pos_x,float pos_y) override;
        void set_pos(sf::Vector2f new_pos) override;

        void poll_event(const sf::Event &e) override;
    };
}

#endif // UI_ELEMENT_H