#pragma once

#include "fvs/discrete/algorithm/diclique.hpp"
#include "fvs/discrete/algorithm/neighborhood.hpp"
#include "fvs/discrete/algorithm/reduction/node.hpp"
#include "fvs/discrete/algorithm/vc.hpp"
#include "fvs/discrete/undirected_graph.hpp"

namespace fvs {

inline void
reduce_SIMPLICIAL(UndirectedGraph& graph,
                  index_t i,
                  const IndexList& nhood,
                  VC& vc)
{
    assert(neighborhood_is_clique(graph, i));
    assert(neighborhood(graph, i) == nhood);
    assert(std::is_sorted(nhood.begin(), nhood.end()));

    for (index_t j = nhood.size() - 1; j + 1 >= 1; j--) {
        if (nhood[j] != i)
            add_to_vc(vc, nhood[j], graph);
        else
            add_invert_to_vc(vc, nhood[j], graph);
        remove_node(graph, nhood[j]);
    }
}
}