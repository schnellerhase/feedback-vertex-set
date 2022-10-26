#pragma once

#include "../subgraph.hpp"
#include "diclique.hpp"

index_t
double_edges_count(const SubGraph& graph)
{
    index_t count = 0;
    for (index_t i = 0; i < graph.N(); i++) {
        const auto& outadj = graph.outadj()[i];
        const auto& inadj = graph.inadj()[i];
        for (const auto& out : outadj) {
            if (out <= i)
                continue;

            if (std::find(inadj.begin(), inadj.end(), out) != inadj.end())
                count++;
        }
    }

    return count;
}

inline IndexList
two_cycle_edges(const SubGraph& graph)
{
    IndexList edges;
    for (index_t m = 0; m < graph.M(); m++)
        if (Graph::has_edge(graph, graph.heads()[m], graph.tails()[m]))
            edges.push_back(m);

    return edges;
}

std::pair<IndexList, IndexList>
find_all_none_2_cycle_edges(const SubGraph& graph, index_t exp_count)
{
    // TODO: switch to usage of list of edge indices?

    IndexList heads;
    heads.reserve(exp_count);
    IndexList tails;
    tails.reserve(exp_count);

    // TODO: we know hwo many edges we expect -> quit if we found all
    for (index_t m = 0; m < graph.M(); m++) {
        index_t tail = graph.tails()[m];
        index_t head = graph.heads()[m];
        const auto& outadj_head = graph.outadj()[head];
        if (std::find(outadj_head.begin(), outadj_head.end(), tail) ==
            outadj_head.end()) {
            heads.push_back(head);
            tails.push_back(tail);
        }
    }

    return std::make_pair(std::move(tails), std::move(heads));
}
