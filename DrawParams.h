//
// Created by oguzh on 14.12.2025.
//

#ifndef DRAWPARAMS_H
#define DRAWPARAMS_H
#include <any>


struct DrawParams {
    float depth;
    float depthBias;
};

struct Vec2 { int x, y; };

inline void sortByY(Vec2& a, Vec2& b, Vec2& c) {
    if (a.y > b.y) std::swap(a, b);
    if (a.y > c.y) std::swap(a, c);
    if (b.y > c.y) std::swap(b, c);
}




#endif //DRAWPARAMS_H
