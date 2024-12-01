/* date = November 08, 2024 12:41 AM */


#ifndef GRID_H
#define GRID_H
#include <glm/vec4.hpp>

#include "ecs/Component.h"
#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"

namespace ChessClone {
    class ChessScene : public Pontilus::Scene /*: public Pontilus::ECS::Component , public Pontilus::Renderer::Renderable */ {
    public:
        // bool operator==(Component &c) override;
        //
        // int update(double dt) override;
        //
        // bool isSingleton() override { return true; }
        // int toRData(Pontilus::Renderer::rData &r) override;

        ChessScene() = default;

        void init() override;
        void update(double) override;
        void clean() override;
    };
} // BlockBlast

#endif //GRID_H
