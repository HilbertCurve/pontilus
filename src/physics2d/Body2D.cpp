#include "physics2d/Body2D.h"

namespace Pontilus
{
    namespace Physics2D
    {
        Circle::Circle(glm::vec2 c, float r) : center(c), radius(r) {}
        float Circle::lineCount()
        {
            return 100;
        }

        AABB::AABB(glm::vec2 min, glm::vec2 max) : min(min), max(max) {}
        float AABB::lineCount()
        {
            return 4;
        }

        Box2D::Box2D(glm::vec2 c, float w, float h, float r) : 
                     center(c), width(w), height(h), rotation(r) {}
        float Box2D::lineCount()
        {
            return 4;
        }

        template<unsigned int N>
        float Polygon<N>::lineCount()
        {
            return N;
        }

        Triangle::Triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
        {
            vertices[0] = p1;
            vertices[1] = p2;
            vertices[2] = p3;
        }
    }
}