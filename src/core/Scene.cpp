#include "Scene.h"
#include "Application.h"
#include "GameObject.h"

namespace Pontilus
{
    namespace Engine
    {
        static GameObject g1, g2, g3, g4;
        static Scene s = 
        {
            []()
            {
                g1 = {};
                initGameObject(g1, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g1.tex);
                s.objs.push_back(g1);
                
                g2 = {};
                initGameObject(g2, { 3.0f, 3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/cookie.png", g2.tex);
                s.objs.push_back(g2);

                g3 = {};
                initGameObject(g3, { -3.0f, -3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g3.tex);
                s.objs.push_back(g3);

                g4 = {};
                initGameObject(g4, { -3.0f, 3.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g4.tex);
                s.objs.push_back(g4);

                for (int i = 0; i < s.objs.size(); i++)
                {
                    gameStateToRend(s.objs[i], rDataPool, i);
                }
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