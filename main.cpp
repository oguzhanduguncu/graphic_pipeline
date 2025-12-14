#include <cstring>
#include <iostream>
#include <algorithm>
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

int main() {
    // clear
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x) {
            buffer[y][x]   = '.';
            depthBuf[y][x] = 1e9f;
        }

    int x0 = 2, y0 = 2, x1 = 42, y1 = 44;

    // draw
    drawAxes();
    drawLine(x0, y0, x1, y1, '#', fill);
    drawLine(x0, y0, x1, y1, 'o', wire);

    // present
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x)
            std::cout << buffer[y][x];
        std::cout << '\n';
    }

    return 0;
}
