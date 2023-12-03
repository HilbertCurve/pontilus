#pragma once

#include <stdio.h>
#include <audio/AudioMaster.h>
#include <core/Application.h>
#include <core/Scene.h>
#include <core/InputListener.h>
#include <ecs/GameObject.h>
#include <ecs/Component.h>
#include <ecs/StateMachine.h>
#include <ecs/SpriteRenderer.h>
#include <ecs/Animation.h>
#include <ecs/Body2D.h>
#include <ecs/AudioListener.h>
#include <ecs/AudioSource.h>
#include <ecs/TextRenderer.h>
#include <graphics/Camera.h>
#include <physics2d/CollisionDetection.h>
#include <utils/PMath.h>
#include <utils/Utils.h>
#include <library/Colors.h>
#include <library/TileMap.h>


static Pontilus::Library::TileMap tilemap = Pontilus::Library::TileMap();