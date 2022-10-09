//
// Created by sejapoe on 08.10.22.
//

#ifndef PUZZLE_PART_H
#define PUZZLE_PART_H


#include <vector>

class part {
public:
    part();
    part(int color, int up, int right, int down, int left);
    part(const part& p);

public:
    int color{};
    std::vector<int> sides;
};


#endif //PUZZLE_PART_H
