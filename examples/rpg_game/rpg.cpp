#include <ecs/Entity.h>

#include "Player.h"
#include "core/Application.h"
#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"

using namespace Pontilus;

class RPG : public Scene {
public:
    void init() {
        // make characters
        this->build<RpgGame::Player>();
    }

    void update(double) {}

    void clean() {}
};

int main() {
    auto *app = Application::get();
    auto *scene = new RPG();
    app->getMainWindow().setScene(*scene);
    app->run();
    return 0;
}
