#include "utils/PMath.h"

using namespace glm;
namespace Pontilus
{
    namespace Math
    {
        Line::Line(vec2 p1, vec2 p2)
        {
            m = (p1.y - p2.y) / (p1.x - p2.x);
            b = p1.y - m * p1.x;
        }

        int signOrZero(float f)
        {
            return f > 0 ? 1 : (f == 0 ? 0 : -1);
        }
        int sign(float f)
        {
            return f >= 0 ? 1 : -1;
        }

        bool between(float left, float f, float right)
        {
            return f > left && f < right;
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

        float lerp(float start, float end, float amt)
        {
            return end * amt + start * (1.0f - amt);
        }

        vec2 lerp(vec2 start, vec2 end, float amt)
        {
            vec2 ret;
            ret.x = lerp(start.x, end.x, amt);
            ret.y = lerp(start.y, end.y, amt);
            return ret;
        }

        float closest(float left, float right, float f)
        {
            if (between(left, f, right)) return f;
            else if (f < left) return left;
            else return right;
        }

        bool pointAboveLine(vec2 p, Line &l)
        {
            return p.y > l.b * p.x + l.b;
        }

        vec2 intersection(Line &l1, Line &l2)
        {
            float b, c, e, f;
            b = -l1.m;
            c = l1.b;
            e = -l2.m;
            f = l2.b;
            mat2 m = 
            {
                b, 1,
                e, 1
            };

            return inverse(m) * glm::vec2(c, f);
        }
    }
}