#include <cstdio>
#include <core/Application.h>
#include <ecs/SpriteRenderer.h>

using namespace Pontilus;
static Engine::Scene test;
static Engine::ECS::GameObject player;

static Engine::ECS::SpriteRenderer *tile;

class block : Engine::ECS::GameObject
{
    public:
    block();
};

block::block()
{
    Engine::ECS::SpriteRenderer tile;
    if (!tile.tex.source)
    {
        Graphics::IconMap im;
        Graphics::initIconMap("./assets/textures/test2.png", im, 8, 8, 0);
        tile.init(Graphics::getTexture(im, 0));
    }

    this->addComponent(tile);
}

int main()
{
    Pontilus::init();

    test.init = []() {
        player.init({0.0f, 0.0f, 0.0f}, {1.0, 1.0, 1.0, 1.0}, 20, 10);
        
    };

    Pontilus::loop();
}
