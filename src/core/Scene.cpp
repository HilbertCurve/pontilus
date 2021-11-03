#include "Scene.h"
#include "Application.h"
#include "GameObject.h"
#include "Rend.h"

namespace Pontilus
{
    namespace Engine
    {
        static GameObject g1, g2, g3, g4, g5;
        static Scene s = 
        {
            []()
            {
                g1 = {};
                initGameObject(g1, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 3.0f, 3.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g1.tex);
                s.objs.push_back(g1);
                
                g2 = {};
                initGameObject(g2, { 3.0f, 3.0f, 0.0f }, { 0.7f, 0.7f, 0.7f, 0.7f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g2.tex);
                s.objs.push_back(g2);

                g3 = {};
                initGameObject(g3, { -3.0f, -3.0f, 0.0f }, { 0.7f, 0.7f, 0.7f, 0.7f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g3.tex);
                s.objs.push_back(g3);

                g4 = {};
                initGameObject(g4, { -3.0f, 3.0f, 0.0f }, { 0.7f, 0.7f, 0.7f, 0.7f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g4.tex);
                s.objs.push_back(g4);

                g5 = {};
                initGameObject(g5, { 3.0f, -3.0f, 0.0f }, { 0.7f, 0.7f, 0.7f, 0.7f }, 5.0f, 5.0f);
                Graphics::initTexture("./assets/textures/ghostSwole.png", g5.tex);
                s.objs.push_back(g5);

                for (int i = 0; i < s.objs.size(); i++)
                {
                    gameStateToRend(s.objs[i], rDataPool, i);
                }
            },
            [](double dt)
            {
                float acceleration = -3;
                static float velocity = 5;

                velocity += acceleration * dt;

                g1.pos.y += velocity * dt;
                gameStateToRend(g1, rDataPool, 0, Graphics::PONT_POS);
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