#pragma once

template<typename Strategy,typename Node,typename Predicate>
Node visit(Node g,Predicate found) {
    Strategy strat;

    strat.push(g);

    while (!strat.empty()) {
        auto v = strat.pop();

        if (found(v))
            return v;

        // add children
        strat.push(v->get_connections());
    }

    return Node();
}
