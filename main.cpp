#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "DrawParams.h"

constexpr int W = 64;
constexpr int H = 64;

constexpr float Z_SCENE = 0.5f;
constexpr float Z_BIAS  = 1e-4f;

DrawParams fill {
    .depth = Z_SCENE,
    .depthBias = 0.0f
};

DrawParams wire {
    .depth = Z_SCENE,
    .depthBias = -Z_BIAS
};

DrawParams axis {
    .depth = Z_SCENE,
    .depthBias = +Z_BIAS
};

char  buffer[H][W];
float depthBuf[H][W];

void tryWritePixel(int x, int y, char symbol, const DrawParams& params) {
    float z = params.depth + params.depthBias;
    if (z < depthBuf[y][x]) {
        depthBuf[y][x] = z;
        buffer[y][x] = symbol;
    }
}

void drawLine(int x0, int y0, int x1, int y1, char symbol, const DrawParams& params) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        int x = static_cast<int>(x0 + t * (x1 - x0));
        int y = static_cast<int>(y0 + t * (y1 - y0));

        if (x >= 0 && x < W && y >= 0 && y < H) {
            tryWritePixel(x, y, symbol, params);
        }
    }
}

void drawAxes() {
    int mid = H / 2;

    for (int x = 0; x < W; ++x)
        tryWritePixel(x, mid, '-', axis);

    for (int y = 0; y < H; ++y)
        tryWritePixel(mid, y, '|', axis);
}

bool isTopLeftEdge(const Vec2& a, const Vec2& b) {
    if (a.y < b.y) return true;
    if (a.y > b.y) return false;

    return a.x > b.x;
}

bool intersectY(const Vec2& p0, const Vec2& p1, int y, float& outX) {
    int yMin = std::min(p0.y, p1.y);
    int yMax = std::max(p0.y, p1.y);

    if (y < yMin || y >= yMax)
        return false;
    if (p0.y == p1.y)
        return false;

    float t = float(y - p0.y) / float(p1.y - p0.y);
    outX = p0.x + t * (p1.x - p0.x);
    return true;
}

void drawFilledTriangle(Vec2 a, Vec2 b, Vec2 c,
                        char symbol,
                        const DrawParams& params)
{
    sortByY(a, b, c);

    for (int y = a.y; y <= c.y; ++y) {
        float xs[2];
        int count = 0;

        float x;
        if (intersectY(a, b, y, x)) xs[count++] = x;
        if (intersectY(b, c, y, x)) xs[count++] = x;
        if (intersectY(a, c, y, x)) xs[count++] = x;

        if (count < 2) continue;

        int x0 = int(std::ceil(std::min(xs[0], xs[1])));
        int x1 = int(std::floor(std::max(xs[0], xs[1])));

        for (int x = x0; x <= x1; ++x) {
            if (x >= 0 && x < W && y >= 0 && y < H)
                tryWritePixel(x, y, symbol, params);
        }
    }
}

void drawWireTriangle(const Vec2& a,
                      const Vec2& b,
                      const Vec2& c,
                      char symbol,
                      const DrawParams& params)
{
    drawLine(a.x, a.y, b.x, b.y, symbol, params);
    drawLine(b.x, b.y, c.x, c.y, symbol, params);
    drawLine(c.x, c.y, a.x, a.y, symbol, params);
}

int main() {
    // clear
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            buffer[y][x]   = '.';
            depthBuf[y][x] = 1e9f;
        }

    int x0 = 2, y0 = 2, x1 = 42, y1 = 44;

    // draw line
  //  drawAxes();
  //  drawLine(x0, y0, x1, y1, '#', fill);
  //  drawLine(x0, y0, x1, y1, 'o', wire);


    //drawTriangular
    Vec2 A {10, 10};
    Vec2 B {50, 15};
    Vec2 C {30, 45};

    drawAxes();

    drawFilledTriangle(A, B, C, '#', fill);
    drawWireTriangle  (A, B, C, 'o', wire);


    // present
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x)
            std::cout << buffer[y][x];
        std::cout << '\n';
    }

    return 0;
}
