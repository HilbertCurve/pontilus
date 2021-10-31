#include "Scene.h"
#include "Application.h"
#include "GameObject.h"

namespace Pontilus
{
    namespace Engine
    {
        static GameObject g1, g2;
        static Scene s = 
        {
            []()
            {
                g1 = {};
                initGameObject(g1, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);
                //Graphics::initTexture("./assets/textures/ghostSwole.png", g1.tex);
                s.objs.push_back(g1);
                
                g2 = {};
                initGameObject(g2, { 3.0f, 3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f, 5.0f);
                //Graphics::initTexture("./assets/textures/cookie.png", g2.tex);
                s.objs.push_back(g2);


                /*
                Graphics::initRend(Renderer::r, 4);

                gameStateToRend(s.objs, Renderer::r, 0);

                s.parent = &window;

                Renderer::g = &s.objs[0];
                */
                gameStateToRend(s.objs[0], rDataPool, 0);
                gameStateToRend(s.objs[1], rDataPool, 1);
            },
            [](double dt)
            {
// nothing right now.
            }
        };

        void init()
        {
            s.init();
        }

        void update(double dt)
        {
            s.update(dt);
        }

        Scene *getScene()
        {
            return &s;
        }
    }
}