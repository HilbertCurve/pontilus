/* date = May 5th, 2022 6:38 pm */

#include <glm/glm.hpp>

namespace Pontilus {
    typedef glm::vec4 Color;
    namespace Colors {
        extern Color WHITE;
        extern Color ORANGE;
        extern Color MAGENTA;
        extern Color LIGHT_BLUE;
        extern Color YELLOW;
        extern Color LIME;
        extern Color PINK;
        extern Color DARK_GREY;
        extern Color LIGHT_GREY;
        extern Color CYAN;
        extern Color PURPLE;
        extern Color BLUE;
        extern Color DARK_BLUE;
        extern Color BROWN;
        extern Color GREEN;
        extern Color DARK_GREEN;
        extern Color RED;
        extern Color DARK_RED;
        extern Color BLACK;

        Color rgb(int r, int g, int b);
        Color rgba(int r, int g, int b, int a);
    }
}
