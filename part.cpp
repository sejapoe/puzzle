//
// Created by sejapoe on 08.10.22.
//

#include "part.h"

part::part(int color, int up, int left, int down, int right) : color(color), sides({ up, left, down, right }) {}

part::part() : color(2), sides(std::vector<int>(4, 0)) {}

part::part(const part &o): color(o.color), sides({ o.sides[0], o.sides[1], o.sides[2], o.sides[3] }) {}
