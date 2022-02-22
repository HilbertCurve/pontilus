#include "physics2d/Body2D.h"
#include "graphics/rData.h"
#include "physics2d/Physics2DController.h"

namespace Pontilus
{
    namespace Physics2D
    {
        void Body2D::init()
        {
            addBody(this);
        }
        Circle::Circle(glm::vec2 c, float r) : radius(r)
        {
            this->center = c;
        }

        void Circle::update(double dt)
        {
            this->center += this->velocity * (float) dt;
        }
        /*
        int Circle::toRData(Graphics::rData &r, unsigned int rOffset)
        {
            using namespace Graphics;
            __pAssert(!(rOffset >= r.vertCount / 2 + 100), "rData not big enough to hold game states!");

                int stride = rOffset * getLayoutLen(r) * 2;
                glm::vec3 orientation;
                float theta = 0;

                for (int i = 0; i < 100; i++)
                {

                    orientation = { cos(theta), sin(theta), 0.0f };

                    off_len result = getAttribMetaData(r, PONT_POS);
                    if (result.second >= 3 * sizeof(float))
                    {
                        parent->pos += orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};

                        // TODO: just use memcpy, bonehead.
                        for (int j = 0; j < 3; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&parent->pos)[j];
                        }

                        parent->pos -= orientation - glm::vec3{parent->width / 2, parent->height / 2, 0.0f};
                    }
                    
                    result = getAttribMetaData(r, PONT_COLOR);
                    if (result.second >= 4 * sizeof(float))
                    {
                        for (int j = 0; j < 4; j++)
                        {
                            ((float *)((char *)r.data + result.first + stride))[j] = ((float *)&parent->color)[j];
                        }             
                    }

                    stride += getLayoutLen(r);
                    theta += M_PI / 100;
                }

                r.isDirty = true;
                
                return stride / (4 * getLayoutLen(r));
        }
        */

        AABB::AABB(glm::vec2 min, glm::vec2 max) : min(min), max(max)
        {
            this->center = (max + min) / 2.0f;
        }

        void AABB::update(double dt)
        {
            this->angularVelocity = 0.0f;
            this->min += this->velocity * (float) dt;
            this->max += this->velocity * (float) dt;

            this->center = (this->min + this->max) / 2.0f;
        }

        Box2D::Box2D(glm::vec2 c, float w, float h, float r) : 
                     width(w), height(h), rotation(r)
        {
            this->center = c;
        }

        void Box2D::update(double dt)
        {
            this->center += this->velocity * (float) dt;
            this->rotation += this->angularVelocity * (float) dt;
        }

        template<unsigned int N>
        void Polygon<N>::update(double dt)
        {
            // not implemented yet
            return;
        }

        Triangle::Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
        {
            vertices[0] = p1;
            vertices[1] = p2;
            vertices[2] = p3;
        }
    }
}