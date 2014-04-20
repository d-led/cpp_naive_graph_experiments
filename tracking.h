#pragma once

#include <unordered_set>

template<typename Node>
struct tracking {
    typedef std::unordered_set<Node> visited_type;
    visited_type visited;

    bool operator()(Node n) {
        if (visited.find(n)==visited.end()) {
            visited.insert(n);
            return false;
        }
        return true;
    }
};
