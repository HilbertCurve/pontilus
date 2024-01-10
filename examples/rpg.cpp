#include <core/Application.h>
#include <core/InputListener.h>
#include <core/Scene.h>
#include <ecs/Animation.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/StateMachine.h>
#include <ecs/TextRenderer.h>
#include <graphics/Camera.h>

#include <sys/stat.h>

using namespace Pontilus;

int main()
{
    Pontilus::init();
    Pontilus::loop();
    return 0;
}