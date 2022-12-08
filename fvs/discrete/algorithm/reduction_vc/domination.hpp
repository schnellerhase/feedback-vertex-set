#pragma once

#include "../../undirected_graph.hpp"
#include "../reduction/node.hpp"
#include "../vc.hpp"

namespace fvs {

using VC = VertexMarker;

inline bool
domination_check(const UndirectedGraph& graph,
                 index_t i,
                 const IndexList& nhood_i,
                 index_t j) // TODO faster with sorted neighbors
{
    assert(std::is_sorted(nhood_i.begin(), nhood_i.end()));

    for (const auto& neigh_j : graph.adj()[j]) {
        if (neigh_j == i)
            continue;

        auto ptr = std::lower_bound(nhood_i.begin(), nhood_i.end(), neigh_j);
        if (ptr == nhood_i.end() || *ptr != neigh_j)
            return false;
    }
    return true;
}

inline void
reduce_domination(UndirectedGraph& graph, index_t dominator, VC& vc)
{
    add_to_vc(vc, dominator, graph);
    remove_node(graph, dominator);
}
}