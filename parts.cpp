//
// Created by sejapoe on 08.10.22.
//

#include "parts.h"
#include "side.h"

#include <utility>
#include <algorithm>

template <typename T>
void emplaceTo(std::map<T, std::vector<part*>> &m, T key, part* p) {
    if (!m.contains(key)) {
        m.insert({ key, std::vector<part*>() });
    }
    m.find(key)->second.push_back(p);
}

parts::parts(const std::vector<part> & allParts) : allParts(allParts) {
    for (part &p : this->allParts) {
        int left = p.sides[LEFT];
        int up = p.sides[UP];
        emplaceTo(byLeft, left, &p);
        emplaceTo(byUp, up, &p);
        emplaceTo(byLeftUp, { left, up }, &p);
        all.emplace_back(&p);
    }
}

parts::parts(const parts &other) : parts(other.allParts) {
//    allParts = other.allParts;
//    byLeft = other.byLeft;
//    byUp = other.byUp;
//    byLeftUp = other.byLeftUp;
}

void parts::removeFromAll(part *p) {
//    auto byLeftV = this->byLeft.find(p->sides[LEFT]);
//    if (byLeftV != this->byLeft.end()) {
//        byLeftV->second.erase(std::find(byLeftV->second.begin(), byLeftV->second.end(), p));
//    }
//    auto byUpV = this->byUp.find(p->sides[UP]);
//    if (byUpV != this->byUp.end()) {
//        byUpV->second.erase(std::find(byUpV->second.begin(), byUpV->second.end(), p));
//    }
//    auto byLeftUpV = this->byLeftUp.find({p->sides[LEFT], p->sides[UP]});
//    if (byLeftUpV != this->byLeftUp.end()) {
//        byLeftUpV->second.erase(std::find(byLeftUpV->second.begin(), byLeftUpV->second.end(), p));
//    }
    erase_if(allParts, [&p] (auto el) {
        return &el == p;
    });
}
