/* date = November 11, 2024 11:36 AM */

#ifndef BLOCK_H
#define BLOCK_H
#include "ecs/Entity.h"

namespace ChessClone {

    class ChessPiece : public Pontilus::ECS::Component, public Pontilus::Renderer::Renderable {
    public:
        enum Type {
            PAWN,
            BISHOP,
            KNIGHT,
            ROOK,
            KING,
            QUEEN,
        };

        ChessPiece();

        int update(double dt) override;
        int toRData(Pontilus::Renderer::rData &r) override;
    };
} // BlockBlast

#endif //BLOCK_H
