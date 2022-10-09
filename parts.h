//
// Created by sejapoe on 08.10.22.
//

#ifndef PUZZLE_PARTS_H
#define PUZZLE_PARTS_H


#include <vector>
#include <map>
#include "part.h"

class parts {
    std::vector<part> allParts;
public:
    std::map<int, std::vector<part*>> byLeft;
    std::map<int, std::vector<part*>> byUp;
    std::map<std::pair<int, int>, std::vector<part*>> byLeftUp;

    std::vector<part*> all;

    explicit parts(const std::vector<part> & allParts);

    parts(const parts& other);

    void removeFromAll(part *p);
};


#endif //PUZZLE_PARTS_H
