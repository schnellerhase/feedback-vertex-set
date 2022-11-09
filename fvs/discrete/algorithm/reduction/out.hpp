#pragma once

#include "../fvs.hpp"

#include "edge.hpp"
#include "node.hpp"
#include "util.hpp"

namespace fvs
{
inline bool
reduce_OUT(SubGraph& graph, index_t i, FVS& fvs)
{
    assert(Graph::has_valid_data_structure(graph));
    assert(graph.outdeg()[i] == 1);
    assert(graph.outadj()[i].size() == 1);
    assert(graph.out2arc()[i].size() == 1);

    index_t pre = graph.outadj()[i][0];
    const index_t& e = graph.out2arc()[i][0];

    assert(pre != i);

    graph.indeg()[pre] += graph.indeg()[i];
    graph.indeg()[i] = 0;

    for (const auto& m : graph.in2arc()[i])
        if (graph.heads()[m] == i)
            graph.heads()[m] = pre;

    for (const auto& k : graph.inadj()[i]) {
        auto& outadj = graph.outadj()[k];
        auto ptr = std::find(outadj.begin(), outadj.end(), i);
        assert(ptr != outadj.end());
        outadj[std::distance(outadj.begin(), ptr)] = pre;
    }

    graph.in2arc()[pre].insert(graph.in2arc()[pre].end(),
                               graph.in2arc()[i].begin(),
                               graph.in2arc()[i].end());
    auto permutation = sort_permutation(graph.in2arc()[pre]);
    apply_permutation(graph.in2arc()[pre], permutation);

    graph.in2arc()[i] = IndexList{};

    graph.inadj()[pre].insert(graph.inadj()[pre].end(),
                              graph.inadj()[i].begin(),
                              graph.inadj()[i].end());
    apply_permutation(graph.inadj()[pre], permutation);

    graph.inadj()[i] = IndexList{};

    remove_edge(graph, e);
    remove_isolated_node(graph, i);

    pre -= (pre > i) * 1;

    for (const auto& k : graph.inadj()[pre]) {
        auto count =
          std::count(graph.inadj()[pre].begin(), graph.inadj()[pre].end(), k);

        if (count <= 1)
            continue;

        for (auto& f : graph.in2arc()[pre]) {
            if (graph.tails()[f] == k) {
                remove_edge(graph, f);
                break;
            }
        }
    }

    if (node_has_self_loop(graph, pre)) {
        fvs[graph.local2global(pre)] = true;
        remove_node(graph, pre);
        return true;
    }

    return false;
}
}