/* date = January 10, 2025 4:35 PM */


#include "NPC.h"

#include <iostream>

#include "Player.h"
#include "TextBox.h"
#include "core/InputListener.h"
#include "core/Scene.h"
#include "ecs/Transform.h"
#include "renderer/SpriteRenderer.h"
#include "renderer/TextRenderer.h"

namespace RpgGame {
    int NPC::NPCComponent::update(double dt) {
        if (this->state)
            this->state->update(this, dt);
        else
            _pWarning("NPCComponent currently has no state.");
        return 0;
    }

    std::string NPC::NPCComponent::getTBEntityName() const {
        return "__" + std::to_string(parent->id) + "_npc_tb";
    }

    void NPC::NPCComponent::setTextChunk(const std::string &text, size_t idx) {
        this->text[idx] = text;
    }

    void NPC::NPCComponent::addTextChunk(const std::string &text) {
        this->text.push_back(text);
    }

    void NPC::NPCComponent::removeTextChunk(size_t idx) {
        this->text.erase(this->text.begin() + idx);
    }

    size_t NPC::NPCComponent::getNumTextChunks() const {
        return this->text.size();
    }

    void NPC::NPCWaiting::start(NPCComponent *parent) {
        auto scene = parent->parent->currentScene;
        auto player = scene->get("player")->getComponent<Player::Controller>();
        player->unfreeze();
    }

    void NPC::NPCWaiting::update(NPCComponent *parent, double) {
        auto scene = parent->parent->currentScene;
        auto player = scene->get("player");
        auto t = parent->parent->getComponent<Pontilus::ECS::Transform>();
        if (distance(player->getComponent<Pontilus::ECS::Transform>()->pos, t->pos) < 75.0) {
            if (Pontilus::IO::isKeyTriggered(GLFW_KEY_SPACE)) {
                parent->changeState<NPCTalking>();
            }
        }
    }

    NPC::NPCWaiting *NPC::NPCWaiting::get() {
        static auto _inst = new NPCWaiting();
        return _inst;
    }

    void NPC::NPCTalking::start(NPCComponent *parent) {
        auto scene = parent->parent->currentScene;
        auto player = scene->get("player")->getComponent<Player::Controller>();
        player->freeze();
        auto t = parent->parent->getComponent<Pontilus::ECS::Transform>();

        scene->build<TextBox>(parent->getTBEntityName());

        auto tb = scene->get(parent->getTBEntityName());
        auto tbTransform = tb->getComponent<Pontilus::ECS::Transform>();
        tbTransform->pos = t->pos + glm::vec3(0.0, 200.0, 0.0);
        tbTransform->whd = glm::vec3(400.0, 200.0, 0.0);
        //tb->getComponent<Pontilus::Renderer::TextRenderer>()->text = parent->getText();
        tb->getComponent<Pontilus::Renderer::TextRenderer>()->text.clear();
    }

    void NPC::NPCTalking::update(NPCComponent *parent, double dt) {
        auto scene = parent->parent->currentScene;

        timeTalking += dt;
        if (charIdx != parent->getTextChunk(currentTextChunk).length()) {
            charCountdown -= dt;
            if (charCountdown <= 0.0) {
                auto tb = scene->get(parent->getTBEntityName());
                tb->getComponent<Pontilus::Renderer::TextRenderer>()->text += parent->getTextChunk(currentTextChunk).at(charIdx);
                charIdx++;
                charCountdown = 0.1;
            }
        } else {
            if (Pontilus::IO::isKeyTriggered(GLFW_KEY_SPACE) && timeTalking > 1.0) {
                timeTalking = 0;
                charIdx = 0;
                currentTextChunk++;
                if (currentTextChunk == parent->getNumTextChunks()) {
                    scene->despawn(parent->getTBEntityName());
                    parent->changeState<NPCWaiting>();
                } else {
                    auto tb = scene->get(parent->getTBEntityName());
                    tb->getComponent<Pontilus::Renderer::TextRenderer>()->text.clear();
                }
            }
        }
    }

    NPC::NPCTalking * NPC::NPCTalking::get() {
        static auto _inst = new NPCTalking();
        return _inst;
    }

    NPC::NPCComponent::NPCComponent() {
        // currently causes a SEGFAULT because parent isn't set until this component is added to an entity.
        //this->changeState<NPCWaiting>();
    }

    void NPC::build(Pontilus::ECS::Entity *obj) {
        obj->addComponent(new Pontilus::ECS::Transform({0.0, 0.0, -1.0}, {50.0, 50.0, 50.0}, {0.0f, 0.0f, 0.0f}));
        obj->addComponent(new NPCComponent());
        obj->addComponent(new Pontilus::Renderer::SpriteRenderer(glm::vec4(1.0, 1.0, 1.0, 1.0)));
    }
} // RpgGame