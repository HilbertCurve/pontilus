// #pragma once

// #include "math.h"
// #include <glm/glm.hpp>

// #ifndef __pEPSILON
// # define __pEPSILON 0.0001f
// #endif

// namespace Pontilus
// {
//     namespace Math
//     {
//         // a line defined in slope-intercept form y = ax + b.
//         class Line
//         {
//             public:
//             Line() = default;
//             Line(glm::vec2 p1, glm::vec2 p2);
//             float m, b;
//         };
//         class LineSegment
//         {
//             public:
//             LineSegment() = default;
//             LineSegment(glm::vec2 p1, glm::vec2 p2) : a(p1), b(p2) { }
//             glm::vec2 a, b;
//         };

//         int signOrZero(float f);
//         int sign(float f);
//         bool between(float left, float f, float right);
//         bool between(glm::vec2 min, glm::vec2 point, glm::vec2 max);
//         float overlap(float start1, float end1, float start2, float end2);

//         glm::vec2 rotate(glm::vec2 v, glm::vec2 pivot, float theta);
//         glm::vec2 rotate(glm::vec2 v, float theta);

//         float dist(glm::vec2 v1, glm::vec2 v2);
//         float distSquared(glm::vec2 v1, glm::vec2 v2);

//         float lerp(float start, float end, float amt);
//         glm::vec2 lerp(glm::vec2 start, glm::vec2 end, float amt);

//         /**
//          * Continuous deformation of topological 0-sphere [start1, end1] to 
//          * [start2, end2] such that (val / (end1 - start1)), where val is
//          * in the reals, remains constant.
//          * 
//          * @return val post-transformation
//          */
//         float map(float start1, float end1, float start2, float end2, float val);

//         /**
//          * Finds the closest value f to the bound given by left and
//          * right. If f is between those two, returns f.
//          */
//         float closest(float left, float right, float f);

//         inline bool epsilon(float f1, float f2)
//         {
//             return abs(f1 - f2) <= __pEPSILON;
//         }
        
//         bool pointAboveLine(glm::vec2 p, Line &l);
//         glm::vec2 intersection(Line &l1, Line &l2);
//     }
// }