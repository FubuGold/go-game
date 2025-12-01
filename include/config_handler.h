#if !defined(CONFIG_H)
#define CONFIG_H

#include "custom_util.h"
#include <SFML/Graphics.hpp>

namespace Config {
    extern sf::Font font[2];

    void load_config();
    void save_config();
}

#endif // CONFIG_H

