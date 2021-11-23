#include "PMath.h"

using namespace glm;
namespace Pontilus
{
    namespace Math
    {
        Line::Line(vec2 p1, vec2 p2)
        {
            a = (p1.y - p2.y) / (p1.x - p2.x);
            b = p1.y - a * p1.x;
        }

        int signOrZero(float f)
        {
            return f > 0 ? 1 : (f == 0 ? 0 : -1);
        }
        int sign(float f)
        {
            return f >= 0 ? 1 : -1;
        }

        vec2 rotate(vec2 v, vec2 pivot, float theta)
        {
            vec2 tPoint = v - pivot;

            tPoint = rotate(tPoint, theta);

            return tPoint + pivot;
        }

        vec2 rotate(vec2 v, float theta)
        {
            float sinTheta = sin(theta);
            float cosTheta = sqrt(1 - sinTheta*sinTheta);

            mat2 rotMatrix = 
            {
                cosTheta, -sinTheta,
                sinTheta,  cosTheta
            };

            return rotMatrix * v;
        }

        float dist(vec2 v1, vec2 v2)
        {
            return sqrt(distSquared(v1, v2));
        }

        float distSquared(vec2 v1, vec2 v2)
        {
            float dx = v1.x - v2.x;
            float dy = v1.y - v2.y;

            return dx*dx + dy*dy;
        }

        bool pointAboveLine(vec2 p, Line &l)
        {
            return p.y > l.a * p.x + l.b;
        }
    }
}