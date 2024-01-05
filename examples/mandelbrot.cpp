// a plot of the mandelbrot set

#include <core/Application.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>

using namespace Pontilus;

Engine::ECS::GameObject *graph;
Engine::ECS::SpriteRenderer *r_graph;

Engine::Scene mainScene = {
    []() {
        graph = &mainScene.spawn();
        graph->init({0.0f, 0.0f, 0.0f}, 500, 500);
        r_graph = new Engine::ECS::SpriteRenderer({nullptr}, {1.0f, 1.0f, 1.0f, 1.0f});

        graph->addComponent(r_graph);
    },
    [](double) {
    },
    []() {
    }
};

int main() {
    Pontilus::setDefaultShader("../assets/shaders/complex_graph.vert", "../assets/shaders/mandelbrot.frag");

    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}

