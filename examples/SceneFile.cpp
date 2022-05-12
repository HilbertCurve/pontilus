#include "SceneFile.h"

namespace Pontilus {
    struct Marcher {
        char prev = '\0';
        char curr;
        char next;

        std::string filepath;
    };

}
