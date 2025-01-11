/* date = January 05, 2025 11:44 PM */


#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "ecs/EntityBuilder.h"
#include "renderer/Font.h"

namespace RpgGame {

class TextBox : public Pontilus::ECS::EntityBuilder {
public:
    void build(Pontilus::ECS::Entity *obj) override;

    static Pontilus::Renderer::Font *font;
};

} // RpgGame

#endif //TEXTBOX_H
