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
        // This is from the outside, created outside
        callback_t hover_callback, press_callback, release_callback;
        
        
        std::vector<sf::Drawable*> parts;
        
        void press();
        void release();
        
        bool contain_pos(float x,float y);
        bool contain_pos(sf::Vector2f point);
        
    public:
        sf::RenderWindow *window = nullptr;
        sf::Vector2f pos;
        sf::FloatRect bound; // Global bounding box

        virtual ~Element();

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
        /**
         * @brief Set the pos object
         * 
         * @param new_pos 
         */
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
         * @param render_win_ptr 
         */
        virtual void set_window(sf::RenderWindow *render_win_ptr);

        /**
         * @brief Check if window is setted
         * 
         * @return true Existed
         * @return false Existed
         */
        bool check_window();
        
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
        virtual void poll_event(const std::optional<sf::Event> &e);
        
        /**
         * @brief Update bounding box (for event handling)
         * 
         */
        void update_bound();
    };
    
    class Rectangle_Button : public Element {
    public:
        sf::RectangleShape *rect = new sf::RectangleShape();
        sf::Text *text = new sf::Text(Config::font[0]);
        sf::Vector2f text_offset = {0.f, 0.f};
        sf::Texture texture;
        sf::Sprite *sprite = nullptr;
        
        Rectangle_Button(float button_width = 337.f, 
            float button_height = 103.f, 
            sf::Color background_color = sf::Color::White, 
            sf::Color text_color = sf::Color::Black, 
            unsigned int text_size = 60);

        ~Rectangle_Button();

        /**
         * @brief Set the text string object
         * 
         * @param str 
         */
        void set_text_string(const std::string &str);

        /**
         * @brief Set the texture object
         * 
         * @param filename 
         */
        void set_texture(const std::filesystem::path &filename);

        /**
         * @brief Set the pos object
         * 
         * @param pos_x 
         * @param pos_y 
         */
        void set_pos(float pos_x,float pos_y) override;
        /**
         * @brief Set the pos object
         * 
         * @param new_pos 
         */
        void set_pos(sf::Vector2f new_pos) override;

        void poll_event(const std::optional<sf::Event> &e) override;
    };

    class Droplist : public Element {
    private:
        std::vector<Rectangle_Button*> drop_list;
        void update_bound();
        float button_width,button_height;

    public:
        Rectangle_Button *title_button;
        int count = 0;
        bool is_expanded = 0;

        Droplist(float button_width = 400, float button_height = 150,const std::string &title_text = "Droplist");
        ~Droplist();

        /**
         * @brief Add a element to the list
         * 
         * @param text 
         * @param func 
         */
        void add_element(const std::string &text, callback_t func);

        void set_window(sf::RenderWindow *render_win_ptr) override;

        /**
         * @brief Set the pos object
         * 
         * @param pos_x 
         * @param pos_y 
         */
        void set_pos(float pos_x,float pos_y) override;
        /**
         * @brief Set the pos object
         * 
         * @param new_pos 
         */
        void set_pos(sf::Vector2f new_pos) override;

        /**
         * @brief Draw the droplist. Does not draw the list element if is not expanded
         * 
         */
        void draw() override;

        void poll_event(const std::optional<sf::Event> &e) override;

    };
}

#endif // UI_ELEMENT_H