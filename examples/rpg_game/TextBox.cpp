/* date = January 05, 2025 11:44 PM */


#include "TextBox.h"

#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/TextRenderer.h"

namespace RpgGame {
    void TextBox::build(Pontilus::ECS::Entity *obj) {
        if (!font) {
            font = new Pontilus::Renderer::Font("../assets/fonts/JetBrainsMono-Medium.ttf", 40);
        }
        obj->addComponent(new Pontilus::ECS::Transform({0.0, 0.0, -1.0}, {500.0, 500.0, 50.0}, {0.0, 0.0, 0.0}));
        auto *text = new Pontilus::Renderer::TextRenderer("abcdefghijklmnopqrstuvwxyz I like to type out words :A:A:A:A:A:A", *font, glm::vec4(1.0, 1.0, 1.0, 1.0));
        obj->addComponent(text);
        obj->addComponent(new Pontilus::Renderer::SpriteRenderer(glm::vec4(1.0, 1.0, 1.0, 0.3)));
    }
    Pontilus::Renderer::Font *TextBox::font = nullptr;
} // RpgGame