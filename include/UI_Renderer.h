#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <SFML/Graphics.hpp>
#include "UI_element.h"
#include <vector>

const sf::Font font = sf::Font("font/Jua-Regular.ttf");

namespace GUI {
    class Canvas {
    protected: 
        std::vector<Element*> element_l;
        sf::RenderWindow *window;
    public:
        virtual ~Canvas();

        /**
         * @brief Add a element to the canvas
         * 
         * @param new_element 
         */
        void add_element(Element *new_element);

        /**
         * @brief Set default window for all element inside
         * 
         * @param render_win_ptr address / pointer to the window
         */
        void set_window(sf::RenderWindow *render_win_ptr);
        
        /**
         * @brief Draw object on to the setted window
         * 
         */
        void draw();

        /**
         * @brief Update all component from event
         * 
         * This will clean all element and build up again
         * 
         * @param e 
         */
        void poll_event(const std::optional<sf::Event> &e);

        /**
         * @brief Clean up element
         * 
         */
        void clean_element();

        /**
         * @brief Recalculate the boundary box of each element
         * 
         */
        void recal_bound();

        /**
         * @brief Add all basic component of the current UI state.
         * 
         * The default does not add anything.
         * 
         * Need to override
         * 
         */
        virtual void setup();
    };

    class Menu_Canvas : public Canvas {
    private:
        float button_width = 377.f, button_height = 103.f;
    public:
        /**
         * @brief Reset the button size in menu (not recommend)
         * 
         * @param new_width 
         * @param new_height 
         */
        void reset_button_size(float new_width,float new_height);

        void setup() override;

        Rectangle_Button* create_button(sf::Vector2f pos, const std::string &text);
    };
}

#endif