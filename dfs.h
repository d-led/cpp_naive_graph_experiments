#pragma once

#include "tracking.h"
#include <stack>

template<typename Node>
struct dfs_strategy {
    tracking<Node> visited;
    std::stack<Node> container;

    void push(Node n) {
        if (!visited(n)) {
            container.push(n);
        }
    }

    template<typename TContainer>
    void push(TContainer&& children) {
        for(auto child = children.rbegin();
            child != children.rend();
            child++ )
                this->push(*child);
    }

    Node pop() {
        auto n ( container.top() );
        container.pop();
        return n;
    }

    bool empty() const {
        return container.empty();
    }
};
