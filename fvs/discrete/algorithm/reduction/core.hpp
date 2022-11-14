#pragma once

#include "../../subgraph.hpp"

#include "../diclique.hpp"
#include "../fvs.hpp"
#include "../neighborhood.hpp"

#include "node.hpp"

namespace fvs {

inline void
reduce_CORE(SubGraph& graph, index_t i, const IndexList& nhood, FVS& fvs)
{
    assert(neighborhood_is_clique(graph, i));
    assert(neighborhood(graph, i) == nhood);
    assert(std::is_sorted(nhood.begin(), nhood.end()));

    for (index_t j = nhood.size() - 1; j + 1 >= 1; j--) {
        fvs[graph.local2global(nhood[j])] = (nhood[j] != i);
        remove_node(graph, nhood[j]);
    }
}
}