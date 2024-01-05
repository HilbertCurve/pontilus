#pragma once

#include <ecs/GameObject.h>

using namespace Pontilus::Engine::ECS;

class Button : public GameObject
{
    bool toggled, highlighted;
    
};