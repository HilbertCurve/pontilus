// a plot of the mandelbrot set

#include <core/Application.h>
#include <core/Scene.h>
#include <ecs/GameObject.h>
#include <ecs/SpriteRenderer.h>

using namespace Pontilus;

Engine::ECS::GameObject graph = Engine::ECS::GameObject();
Engine::ECS::SpriteRenderer r_graph = Engine::ECS::SpriteRenderer();

Engine::Scene mainScene = {
    []() {
        graph.init({0.0f, 0.0f, 0.0f}, 500, 500);
        r_graph.init({nullptr}, {1.0f, 1.0f, 1.0f, 1.0f});

        graph.addComponent(r_graph);

        mainScene.addObj(&graph);

        updateSceneGraphics(mainScene);
    },
    [](double dt) {
        updateSceneGraphics(mainScene);
    },
    []() {

    }
};

int main() {
    Pontilus::setDefaultShader("../assets/shaders/complex_graph.vert", "../assets/shaders/complex_graph.frag");

    Pontilus::init();
    Pontilus::setCurrentScene(mainScene);
    Pontilus::loop();
}

