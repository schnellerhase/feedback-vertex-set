#pragma once

#include "../fvs.hpp"

#include "edge.hpp"
#include "node.hpp"
#include "util.hpp"

namespace fvs {

inline bool
reduce_IN(SubGraph& graph, index_t i, FVS& fvs)
{
    assert(Graph::has_valid_data_structure(graph));
    assert(graph.indeg()[i] == 1);
    assert(graph.inadj()[i].size() == 1);
    assert(graph.in2arc()[i].size() == 1);

    index_t pre = graph.inadj()[i][0];
    const index_t& e = graph.in2arc()[i][0];

    assert(pre != i);

    graph.outdeg()[pre] += graph.outdeg()[i];
    graph.outdeg()[i] = 0;

    for (const auto& m : graph.out2arc()[i])
        if (graph.tails()[m] == i)
            graph.tails()[m] = pre;

    for (const auto& k : graph.outadj()[i]) {
        auto& inadj = graph.inadj()[k];
        auto ptr = std::find(inadj.begin(), inadj.end(), i);
        assert(ptr != inadj.end());
        inadj[std::distance(inadj.begin(), ptr)] = pre;
    }

    graph.out2arc()[pre].insert(graph.out2arc()[pre].end(),
                                graph.out2arc()[i].begin(),
                                graph.out2arc()[i].end());

    auto permutation = sort_permutation(graph.out2arc()[pre]);
    apply_permutation(graph.out2arc()[pre], permutation);
    graph.out2arc()[i].clear();

    graph.outadj()[pre].insert(graph.outadj()[pre].end(),
                               graph.outadj()[i].begin(),
                               graph.outadj()[i].end());
    apply_permutation(graph.outadj()[pre], permutation);
    graph.outadj()[i].clear();

    remove_edge(graph, e);
    remove_isolated_node(graph, i);

    pre -= (pre > i) * 1;

    for (const auto& k : graph.outadj()[pre]) {
        auto count =
          std::count(graph.outadj()[pre].begin(), graph.outadj()[pre].end(), k);
        assert(count <= 2);

        if (count <= 1)
            continue;

        for (const auto& f : graph.out2arc()[pre]) {
            if (graph.heads()[f] == k) {
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