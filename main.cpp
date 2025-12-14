#include <cstring>
#include <iostream>

int main() {
    char buffer[64][64];
    for (int i = 0; i < 64; ++i) {
        std::memset(buffer[i], '.', 64);
    }

    int x0 = 2, y0 = 2, x1 = 62, y1 = 44;
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int steps = std::max(dx, dy);

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;

        int x = static_cast<int>(x0 + t * (x1 - x0));
        int y = static_cast<int>(y0 + t * (y1 - y0));

        if (x >= 0 && x < 64 && y >= 0 && y < 64) {
            buffer[y][x] = '#';
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