#pragma once

#include "../../graph.hpp"

namespace fvs {

using FVS = VertexMarker;

FVS
greedy_reduction(const Graph& graph, index_t batch_size)
{
    IndexList sorted_nodes(graph.N());
    std::iota(sorted_nodes.begin(), sorted_nodes.end(), 0);
    std::sort(
      sorted_nodes.begin(), sorted_nodes.end(), [&graph](index_t i, index_t j) {
          return graph.indeg()[i] + graph.outdeg()[i] <
                 graph.indeg()[j] + graph.outdeg()[j];
      });

    FVS fvs(graph.N(), false);

    index_t count = 0;
    while (!Graph::is_acyclic(graph, fvs)) {
        for (index_t node = count * batch_size;
             node < std::min((count + 1) * batch_size, graph.N());
             node++)
            fvs[sorted_nodes[node]] = true;
        count++;
    }

    index_t i = std::min(count * batch_size, graph.N());
    while (i > 0) {
        i--;
        fvs[sorted_nodes[i]] = false;

        if (!Graph::is_acyclic(graph, fvs))
            fvs[sorted_nodes[i]] = true;
    }

    return fvs;
}
}