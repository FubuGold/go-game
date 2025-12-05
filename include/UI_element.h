#if !defined(UI_ELEMENT_H)
#define UI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <chrono>
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
        
        virtual void press();
        virtual void release();
        virtual void hover();
        
        virtual bool contain_pos(float x,float y);
        virtual bool contain_pos(sf::Vector2f point);
        
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
        // Two pointer are deleted in the destructor of Element
        sf::RectangleShape *rect = new sf::RectangleShape();
        sf::Text *text = new sf::Text(Config::font[0]);
        sf::Vector2f text_offset = {0.f, 0.f};
        sf::Texture texture;
        sf::Sprite *sprite = nullptr;
        
        Rectangle_Button(
            float button_width = 337.f, 
            float button_height = 103.f, 
            sf::Color background_color = sf::Color::White, 
            sf::Color text_color = sf::Color::Black, 
            unsigned int text_size = 60
        );

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

        void press_virtual();

        void poll_event(const std::optional<sf::Event> &e) override;
    };

    class Board_Stone : public Rectangle_Button {
    public:
        float button_width, button_height;
        sf::Vector2i board_pos; //The coordination of this button on the board
        sf::Texture textures[2];
        sf::Sprite *stone_sprite[2] = {nullptr, nullptr};
        
        ~Board_Stone();

        Board_Stone(
            sf::Vector2i board_pos = {},
            float button_width = 40.f, 
            float button_height = 40.f, 
            sf::Color background_color = sf::Color::Transparent
        );

        void poll_event(const std::optional<sf::Event> &e) override;
    };

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    class Popup : public Element {
    private:
        std::chrono::system_clock::time_point enable_time;
        void update_disable();
    public:
        sf::RectangleShape *rect = new sf::RectangleShape();
        bool disable;
        int duration;
        std::vector<Element*> element_l;

        Popup(
            int duration = -1, // ms
            float rect_width = 300.f,
            float rect_height = 300.f,
            sf::Color background_color = sf::Color::White
        );

        ~Popup();

        void enable_popup();

        void disable_popup();

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
         * @brief Add new element to popup
         * 
         * DOES NOT set the position for the element. Please manually set the position.
         * 
         * @param drawable_p 
         */
        void add_element(Element *element_p);

        /**
         * @brief Add new sf::Drawable to popup
         * 
         * DOES NOT set the position for the element. Please manually set the position.
         * 
         * @param drawable_p 
         */
        void add_drawable(sf::Drawable *drawable_p);

        void draw() override;
        
        void poll_event(const std::optional<sf::Event> &e) override;
    };

    class Droplist : public Element {
    private:
        std::vector<Rectangle_Button*> drop_list;
        void update_bound();
        float button_width,button_height;

    public:
        Rectangle_Button *title_button;
        sf::Color sub_color, sub_text_color;
        int count = 0;
        bool is_expanded = 0;

        Droplist(
            float button_width = 400,
            float button_height = 150,
            const std::string &title_text = "Droplist",
            sf::Color main_color = sf::Color::Black,
            sf::Color main_text_color = sf::Color::White,
            sf::Color sub_color = sf::Color::White,
            sf::Color sub_text_color = sf::Color::Black
        );
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

    /**
     * @brief Horizontal Slider with range [0,max value]
     * 
     * Horizontal slider with the range [0,max value]. This will sent the value when release
     * 
     */
    class H_Slider : public Element {
    private:
        bool is_pressed = 0;

        void press() override;
        void release() override;
        
        bool contain_pos(float x,float y) override;
        bool contain_pos(sf::Vector2f point) override;

        void update_value();
        void update_display();
    public:
        sf::RectangleShape *main_bar, *progress_bar;
        sf::Text *value_display = new sf::Text(Config::font[0]);
        sf::Text *slider_name = new sf::Text(Config::font[0]);
        callback_t change_callback;
        int value = 0;
        int steps;
        int cur_num_step = 0;
        int max_value;
        float steps_width;
        float width, height;

        H_Slider(
            float width = 500.f,
            float height = 70.f,
            int max_value = 100, 
            int steps = 100,
            const std::string &name = "",
            sf::Color main_color = sf::Color::White, 
            sf::Color progress_color = sf::Color::Cyan,
            int text_size = 60,
            sf::Color text_color = sf::Color::Black,
            int start_value = 0
        );

        void set_change(callback_t callback);

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
         * @brief Override the default text position
         * 
         */
        void set_display_value_pos(sf::Vector2f new_pos);

        /**
         * @brief Override the default name position
         * 
         */
        void set_name_pos(sf::Vector2f new_pos);

        void poll_event(const std::optional<sf::Event> &e) override;
    };

    class Dynamic_Text : public Element {
    private:
        int *linked_var = nullptr;
        sf::Text *text = new sf::Text(Config::font[0]);
    public:
        Dynamic_Text(
            int *link_var = nullptr,
            sf::Color text_color = sf::Color::Black,
            int text_size = 10
        );

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

        void draw() override;
    };
}

#endif // UI_ELEMENT_H