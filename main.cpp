#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "unicorn.h"
#include "part.h"
#include "side.h"

#define WHITE "[47m[30m"
#define BLACK "[40m[37m"
#define GRAY "[90m"
#define RESET "[0m"

static const int SIZE = 24;
std::vector<std::vector<int>> counter(SIZE, std::vector<int>(SIZE, 0));

std::vector<part> desolve(const int img[32][32], int n) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution dist(1, n);
    std::uniform_int_distribution dist2(1, 2);

    std::vector<std::vector<part>> v(SIZE);
    std::vector<part> res;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            v[i].emplace_back(
                img[i][j],
                i == 0 ? pow(-1, dist2(g)) * dist(g) : -v[i - 1][j].sides[DOWN],

                pow(-1, dist2(g)) * dist(g),
                pow(-1, dist2(g)) * dist(g),
                j == 0 ? pow(-1, dist2(g)) * dist(g) : -v[i][j - 1].sides[RIGHT]);
            res.push_back(v[i][j]);
        }
    };
    std::shuffle(res.begin(), res.end(), g);
    return res;
}

void printByColors(const std::vector<std::vector<part const *>> &parts) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (parts[i][j] == nullptr) {
                std::cout << GRAY << "   " << RESET;
            } else {
                switch (parts[i][j]->color) {
                    case 0:std::cout << BLACK;
                        break;
                    case 1:std::cout << WHITE;
                        break;
                    case 2:std::cout << GRAY;
                        break;
                }
                std::string s = std::to_string(counter[i][j]);
                if (s.length() == 0) s = "   ";
                if (s.length() == 1) s = "  " + s;
                if (s.length() == 2) s = " " + s;
                std::cout << s << RESET;
            }
        }
        std::cout << "\n";
    }
}

void moveLinesUp(int count) {
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            std::cout << "\x1b[1A"; // Move cursor up one
        }
        std::cout << "\r"; // Resume the cursor at beginning of line
    }
}

bool solve(const int img[32][32], std::vector<part const *> parts, std::vector<std::vector<part const *>> v, int index,
           const std::vector<std::pair<int, int>> &queue);

bool solve(const int img[32][32], std::vector<part const *> parts, std::vector<std::vector<part const *>> v, int index,
           const std::vector<std::pair<int, int>> &queue) {
    if (index != 0) moveLinesUp(SIZE);
    printByColors(v); // отображение
    if (index == SIZE * SIZE) {
        return true;
    }

    int y = queue[index].first;
    int x = queue[index].second;

    for (int i = 0; i < parts.size(); i++) {
        part const *p = parts[i];
        if (p->color == img[y][x]
            && (x == 0 || v[y][x - 1] == nullptr || p->sides[LEFT] == -v[y][x - 1]->sides[RIGHT])
            && (y == 0 || v[y - 1][x] == nullptr || p->sides[UP] == -v[y - 1][x]->sides[DOWN])
            && (x == SIZE - 1 || v[y][x + 1] == nullptr || p->sides[RIGHT] == -v[y][x + 1]->sides[LEFT])
            && (y == SIZE - 1 || v[y + 1][x] == nullptr || p->sides[DOWN] == -v[y + 1][x]->sides[UP])) {
            v[y][x] = p;
            counter[y][x]++;
            for (int j = index + 1; j < SIZE * SIZE; j++) {
                if (counter[queue[j].first][queue[j].second] == 0) break;
                counter[queue[j].first][queue[j].second] = 0;
            }
            std::vector<part const *> partsWithout = parts;
            partsWithout.erase(partsWithout.begin() + i);
            if (solve(img, parts, v, index + 1, queue)) {
                return true;
            }
        }
    }
    return false;
}

void generateQueue(std::vector<std::pair<int, int>> &queue, int n) {
    if (n >= 6) {
        for (int y = 0; y < SIZE; y += 8) {
            if (y % 16 == 0) {
                for (int x = 0; x < SIZE; x += 2) {
                    for (int y1 = 0; y1 < 8; ++y1) {
                        queue.emplace_back(y + y1, x);
                        queue.emplace_back(y + y1, x + 1);
                    }
                }
            } else {
                for (int x = SIZE - 2; x >= 0; x -= 2) {
                    for (int y1 = 0; y1 < 8; ++y1) {
                        queue.emplace_back(y + y1, x + 1);
                        queue.emplace_back(y + y1, x);
                    }
                }
            }
        }
    } else if (n >= 6) {
        // idk
    } else {
        for (int y = 0; y < SIZE; y++) {
            if (y % 2 == 0) {
                for (int x = 0; x < SIZE; x++) {
                    queue.emplace_back(y, x);
                }
            } else {
                for (int x = SIZE - 1; x >= 0; x--) {
                    queue.emplace_back(y, x);
                }
            }

        }
    }
}

bool solve(const int img[32][32], const std::vector<part> &allParts, int n) {
    std::vector<std::vector<part const *>> v(SIZE, std::vector<part const *>(SIZE, nullptr));
    std::vector<std::pair<int, int>> queue;
    queue.reserve(SIZE * SIZE);
    generateQueue(queue, n);
    std::vector<const part *> parts(allParts.size());
    for (int i = 0; i < allParts.size(); i++) {
        parts[i] = &allParts[i];
    }

    std::reverse(queue.begin(), queue.end());

    if (solve(img, parts, v, 0, queue)) {
        return true;
    };
    return false;
}

template<typename F>
long measureTime(F &&lambda) {
    auto start = std::chrono::high_resolution_clock::now();
    lambda();
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
}

int main() {
    std::cout << "Введите количество уникальных пимпочек (1-200): ";
    int N;
    std::cin >> N;
    if (N < 1 || N > 200) {
        throw std::invalid_argument("Неверное количество пимпочек: " + std::to_string(N));
    }
    auto shuffledParts = desolve(UNICORN, N);
    long time = measureTime([&shuffledParts, &N] {
        solve(UNICORN, shuffledParts, N);
    });

    std::cout << time << std::endl;
}
