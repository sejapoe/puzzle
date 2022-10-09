#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include "unicorn.h"
#include "part.h"
#include "side.h"
#include "parts.h"

#define WHITE "[47m[30m"
#define BLACK "[40m[37m"
#define GRAY "[90m"
#define RESET "[0m"

static const int SIZE = 32;
std::vector<std::vector<int>> counter(SIZE, std::vector<int>(SIZE, 0));

void printOneSide(int x) {
    std::cout << (x < 0 ? '-' : ' ') << abs(x) << ' ';
}

void printWithPimps(const std::vector<std::vector<part>> &parts) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (parts[i][j].color) {
                case 0:
                    std::cout << BLACK;
                    break;
                case 1:
                    std::cout << WHITE;
                    break;
                case 2:
                    std::cout << GRAY;
                    break;
            }
            std::cout << "   ";
            printOneSide(parts[i][j].sides[UP]);
            std::cout << "   ";
            std::cout << RESET;
        }
        std::cout << "\n";
        for (int j = 0; j < parts.size(); j++) {
            switch (parts[i][j].color) {
                case 0:
                    std::cout << BLACK;
                    break;
                case 1:
                    std::cout << WHITE;
                    break;
                case 2:
                    std::cout << GRAY;
                    break;
            }
            printOneSide(parts[i][j].sides[LEFT]);
            std::cout << " " << parts[i][j].color << " ";
            printOneSide(parts[i][j].sides[RIGHT]);
            std::cout << RESET;
        }
        std::cout << "\n";
        for (int j = 0; j < parts.size(); j++) {
            switch (parts[i][j].color) {
                case 0:
                    std::cout << BLACK;
                    break;
                case 1:
                    std::cout << WHITE;
                    break;
                case 2:
                    std::cout << GRAY;
                    break;
            }
            std::cout << "   ";
            printOneSide(parts[i][j].sides[DOWN]);
            std::cout << "   ";
            std::cout << RESET;
        }
        std::cout << "\n";
    }
}

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
    printWithPimps(v);
    std::shuffle(res.begin(), res.end(), g);
    return res;
}

void printByColors(const std::vector<std::vector<part>> &parts) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (parts[i][j].color) {
                case 0:
                    std::cout << BLACK;
                    break;
                case 1:
                    std::cout << WHITE;
                    break;
                case 2:
                    std::cout << GRAY;
                    break;
            }
            std::string s = std::to_string(counter[i][j]);
            if (s.length() == 0) s = "   ";
            if (s.length() == 1) s = "  " + s;
            if (s.length() == 2) s = " " + s;
            std::cout << s << RESET;
        }
        std::cout << "\n";
    }
}

void moveLinesUp(int count) {
    if (count > 0) {
        for (int i = 0; i < count; i++) {
            std::cout << "\x1b[1A"; // Move cursor up one
        }
        std::cout << "\r"; // Resume the cursor at beginni  ng of line
    }
}

bool _solve(const int img[32][32], std::vector<part> parts, std::vector<std::vector<part>> v, int index,
            const std::vector<std::pair<int, int>> &queue);

bool _solve(const int img[32][32], std::vector<part> parts, std::vector<std::vector<part>> v, int index,
            const std::vector<std::pair<int, int>> &queue) {
    if (index != 0) moveLinesUp(SIZE);
    printByColors(v);
//    printWithPimps(v);
    if (index == SIZE * SIZE) {
        return true;
    }
    /*  std::vector<part*> vec;
      if (x != 0) {
          if (y != 0) {
              auto iterator = _parts.byLeftUp.find({-v[y][x - 1].sides[RIGHT], -v[y - 1][x].sides[DOWN]});
              if (iterator == _parts.byLeftUp.end()) return false;
              vec = iterator->second;
          } else {
              auto iterator = _parts.byLeft.find(-v[y][x - 1].sides[RIGHT]);
              if (iterator == _parts.byLeft.end()) return false;
              vec = iterator->second;
          }
      } else {
          if (y != 0) {
              auto iterator = _parts.byUp.find(-v[y - 1][x].sides[DOWN]);
              if (iterator == _parts.byUp.end()) return false;
              vec = iterator->second;
          } else {
              vec = _parts.all;
          }
      }
      for (auto p : vec) {
          if (p->color == img[y][x]) {
              v[y][x] = part(*p);
              counter[y][x]++;
  //            p->color = 2;
              int newX = x + 1;
              int newY = y;
  //            if (newX == -1) {
  //                newX = newY;
  //                newY = 0;
  //            }
              if (newX == SIZE) {
                  newX = 0;
                  newY++;
              }
              std::cout << "           " << p;
              if (_solve(img, _parts  , v, newX, newY)) {
                  return true;
              }
              p->color = v[y][x].color;
          }
      }
  */

    int y = queue[index].first;
    int x = queue[index].second;

    for (int i = 0; i < parts.size(); i++) {
        part &p = parts[i];
        if (p.color == img[y][x]
            && (x == 0        || v[y][x - 1].sides[RIGHT] == 0 || p.sides[LEFT]  == -v[y][x - 1].sides[RIGHT])
            && (y == 0        || v[y - 1][x].sides[DOWN]  == 0 || p.sides[UP]    == -v[y - 1][x].sides[DOWN])
            && (x == SIZE - 1 || v[y][x + 1].sides[LEFT]  == 0 || p.sides[RIGHT] == -v[y][x + 1].sides[LEFT])
            && (y == SIZE - 1 || v[y + 1][x].sides[UP]    == 0 || p.sides[DOWN]  == -v[y + 1][x].sides[UP])) {
            v[y][x] = p;
            counter[y][x]++;
            std::vector<part> partsWithout = parts;
            partsWithout.erase(partsWithout.begin() + i);
            if (_solve(img, parts, v, index + 1, queue)) {
                return true;
            }
        }
    }
    return false;
}

template<typename F>
void measureTime(std::ostream &out, F &&lambda) {
    auto start = std::chrono::high_resolution_clock::now();
    lambda();
    auto stop = std::chrono::high_resolution_clock::now();
    out << "\n" << (stop - start).count() << "\n";
}

bool solve(const int img[32][32], const std::vector<part> &allParts, int n) {
    std::vector<std::vector<part>> v(SIZE, std::vector(SIZE, part()));
//    for (auto i = 0; i < 32; i++) {
    std::vector<std::pair<int, int>> queue;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            queue.emplace_back( y, x );
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());

//    std::shuffle(queue.begin(), queue.end(), g);
    std::reverse(queue.begin(), queue.end());

    if (_solve(img, allParts, v, 0, queue)) {
        return true;
    };
//    }
    return false;
}

int main() {
    std::cout << "Введите количество уникальных пимпочек (1-200): ";
    int N;
    std::cin >> N;
    if (N < 1 || N > 200) {
        throw std::invalid_argument("Неверное количество пимпочек: " + std::to_string(N));
    }
    auto shuffledParts = desolve(UNICORN, N);
    measureTime(std::cout, [&shuffledParts] {
        auto p = parts(shuffledParts);
        int s = 0;
        for (auto i : p.byUp) {
            s += i.second.size();
        }
        std::cout << "Avg by up: " << (double) s / p.byUp.size() << "\n";
        s = 0;
        for (auto i : p.byLeftUp) {
            s += i.second.size();
        }
        std::cout << "Avg by left up: " << (double) s / p.byLeftUp.size() << "\n";
    });
//    return 0;
    if (solve(UNICORN, shuffledParts, N)) {
        std::cout << "SOLVED";
    };
    return 0;
}
