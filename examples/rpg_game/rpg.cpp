#include "Enemy.h"
#include "NPC.h"
#include "Player.h"
#include "TextBox.h"
#include "core/Application.h"
#include "core/InputListener.h"
#include "ecs/Transform.h"
#include "renderer/TextRenderer.h"

using namespace Pontilus;

namespace RpgGame {
class Rpg : public Scene {
public:
    void init() override {
        // make characters
        this->build<Player>("player");
        //this->build<Enemy>("enemy0");
        //this->build<Enemy>("enemy1");
        //this->build<Enemy>("enemy2");
        this->build<NPC>("npc");
        this->get("npc")->getComponent<NPC::NPCComponent>()->changeState<NPC::NPCWaiting>();
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("hey there!!"));
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("i bet you're wondering where you are."));
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("well, you're in my world. i created this."));
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("it took a couple years, and i've learned a lot since..."));
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("starting this project, but it finally payed off, i think."));
        this->get("npc")->getComponent<NPC::NPCComponent>()->addTextChunk(std::string("i hope you enjoy!"));
        //this->build<TextBox>("tb");
        //this->getCamera()->setMode(Renderer::Camera::PERSPECTIVE);
        //const auto tb = this->get("tb");
        //tb->getComponent<Renderer::TextRenderer>()->text = "Hey there!";
        //tb->getComponent<ECS::Transform>()->whd = {200.0, 100.0, 0.0};
        const auto player = this->get("player");
        player->getComponent<ECS::Transform>()->pos = glm::vec3(100.0, -60.0, -1.0);
    }

    void update(double dt) override {
        //this->get("tb")->getComponent<Renderer::TextRenderer>()->text += "a";

    }

    void clean() override {}
};
}

int main() {
    auto *app = Application::get();
    auto *scene = new RpgGame::Rpg();
    app->getMainWindow().setScene(*scene);
    app->run();
    return 0;
}
