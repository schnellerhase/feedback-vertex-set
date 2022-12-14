#pragma once

#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/undirected_graph.hpp"

namespace fvs {

UndirectedGraph
extract_undirected_graph(const SubGraph& graph)
{
    assert(SubGraph::is_undirected_graph(graph));

    index_t N = graph.N();
    auto M = static_cast<index_t>(graph.M() / 2);
    IndexList tails;
    tails.reserve(M);
    IndexList heads;
    heads.reserve(N);

    for (index_t m = 0; m < graph.M(); m++) {
        index_t tail = graph.tails()[m];
        index_t head = graph.heads()[m];
        if (tail < head) {
            tails.push_back(tail);
            heads.push_back(head);
        }
    }

    assert(tails.size() == M);
    assert(heads.size() == M);

    return { N, tails, heads, graph.local2global() };
}

}