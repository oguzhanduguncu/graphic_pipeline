#include <cstring>
#include <iostream>

int main() {
    char buffer[64][64];
    float depth[64][64];
    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            buffer[y][x] = '.';
            depth[y][x] = 1e9f;
        }
    }

    float z_line = 0.5f;
    int x0 = 2, y0 = 2, x1 = 42, y1 = 44;
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i) {

        float t = static_cast<float>(i) / steps;

        int x = static_cast<int>(x0 + t * (x1 - x0));
        int y = static_cast<int>(y0 + t * (y1 - y0));

        if (x >= 0 && x < 64 && y >= 0 && y < 64) {
            if (z_line < depth[y][x]) {
                depth[y][x] = z_line;
                buffer[y][x] = '#';
            }
        }
    }
    float z_axis = 1.0f;
    int y = 32;
    for (int x = 0; x < 64; ++x) {
        if (z_axis < depth[y][x]) {
            depth[y][x] = z_axis;
            buffer[y][x] = '-';
        }
    }
    int x = 32;
    for (int y = 0; y < 64; ++y) {
        if (z_axis < depth[y][x]) {
            depth[y][x] = z_axis;
            buffer[y][x] = '|';
        }
    }

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            std::cout << buffer[i][j];
        }
        std::cout << "\n";
    }

    return 0;
}