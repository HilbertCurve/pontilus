#include "library/Colors.h"

namespace Pontilus {
    namespace Colors {
        Color rgb(int r, int g, int b) {
            return glm::vec4{(float) r, (float) g, (float) b, 255} / 255.0f;
        }

        Color rgba(int r, int g, int b, int a) {
            return glm::vec4{(float) r, (float) g, (float) b, (float) a} / 255.0f;
        }

        Color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
        Color ORANGE = rgb(255,140,0);
        Color MAGENTA = {1.0f, 0.0f, 1.0f, 1.0f};
        Color LIGHT_BLUE = rgb(85, 255, 255);
        Color YELLOW = {1.0f, 1.0f, 0.0f, 1.0f};
        Color LIME = rgb(85, 255, 85);
        Color PINK = rgb(255, 85, 255);
        Color DARK_GREY = rgb(85, 85, 85);
        Color LIGHT_GREY = rgb(170, 170, 170);
        Color CYAN = rgb(0, 170, 170);
        Color PURPLE = {1.0f, 0.0f, 1.0f, 1.0f};
        Color BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
        Color DARK_BLUE = rgb(0, 0, 170);
        Color BROWN = rgb(205, 127, 50);
        Color GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
        Color DARK_GREEN = rgb(0, 170, 0);
        Color RED = {1.0f, 0.0f, 0.0f, 1.0f};
        Color DARK_RED = rgb(170, 0, 0);
        Color BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    }
}

