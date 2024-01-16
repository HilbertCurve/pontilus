#include "editor.h"

#include <core/InputListener.h>
#include <ecs/Transform.h>

namespace Platformer
{
    Editor *Editor::_inst = nullptr;

    int Editor::update(double) {
        using namespace Pontilus;
        // the Editor needs a transform to know where to draw itself
        ECS::Transform *tptr = (ECS::Transform *)this->parent->getComponent(typeid(ECS::Transform));
        if (!tptr) {
            __pWarning("Editor component needs a Transform!");
            return 1;
        }

        // float scroll = Pontilus::IO::mouseScroll().y;
        glm::vec2 cursor = Pontilus::IO::mousePos();

        // to get from screen space to world space, we use this special function
        glm::vec3 pos = Pontilus::screenToWorldCoords(cursor);

        // to get from raw position to rounded-off position, we use the round function
        float roundWidth = this->currentTileMap->width();
        float roundHeight = this->currentTileMap->height();
        glm::vec3 roundPos = glm::vec3(
            round(pos.x/roundWidth)*roundWidth,
            round(pos.y/roundHeight)*roundHeight,
            pos.z);

        // this becomes the object's position
        tptr->pos = roundPos;

        // if the mouse is pressed, we place a block
        if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_1)) {
            // get coord in tilemap space
            glm::ivec2 tilePos = glm::ivec2((int) roundPos.x / roundWidth, (int) roundPos.y / roundHeight);
            this->currentTileMap->setTile(tilePos, this->valToInsert);
        } else if (IO::isButtonPressed(GLFW_MOUSE_BUTTON_2)) {
            glm::ivec2 tilePos = glm::ivec2((int) roundPos.x / roundWidth, (int) roundPos.y / roundHeight);
            this->currentTileMap->removeTile(tilePos);
        }

        return 0;
    }

    void Editor::edit(TileMap &t) {
        this->currentTileMap = &t;
        this->valToInsert = 1;
    }
}