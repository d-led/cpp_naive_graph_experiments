#pragma once

#include "tracking.h"
#include <queue>

template<typename Node>
struct bfs_strategy {
    tracking<Node> visited;
    std::queue<Node> container;

    void push(Node n) {
        if (!visited(n)) {
            container.push(n);
        }
    }

    template<typename TContainer>
    void push(TContainer&& children) {
        for(auto child = children.begin();
            child != children.end();
            child++ )
                this->push(*child);
    }

    Node pop() {
        auto n ( container.front() );
        container.pop();
        return n;
    }

    bool empty() const {
        return container.empty();
    }
};
