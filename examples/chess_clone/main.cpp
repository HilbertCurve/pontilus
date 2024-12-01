/* date = November 08, 2024 12:31 AM */

#include <core/Application.h>
#include <core/Scene.h>
#include <ecs/Entity.h>
#include <renderer/SpriteRenderer.h>
#include <ecs/Transform.h>
#include "ChessScene.h"

using namespace Pontilus;

namespace ChessClone {
    ChessScene scene{};
}

int main() {
    Application *app = Application::get();
    app->getMainWindow().setScene(ChessClone::scene);
    app->run();
    return 0;
}