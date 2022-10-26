#pragma once

#include "edge.hpp"
#include "util.hpp"

inline void
remove_isolated_node(UndirectedGraph& graph, index_t i)
{
    assert(graph.N() > 0);
    assert(graph.deg()[i] == 0);
    assert(graph.adj()[i].size() == 0);
    assert(graph.to_arc()[i].size() == 0);

    graph.deg().erase(graph.deg().begin() + i);
    graph.adj().erase(graph.adj().begin() + i);
    graph.to_arc().erase(graph.to_arc().begin() + i);
    graph.local2global().erase(graph.local2global().begin() + i);
    graph.local2globalInvert().erase(graph.local2globalInvert().begin() + i);

    graph.N(graph.N() - 1);
    update_indices(graph.heads(), i);
    update_indices(graph.tails(), i);

    for (auto& list : graph.adj())
        update_indices(list, i);
}

inline void
remove_node(UndirectedGraph& graph, index_t i)
{
    // assert(has_valid_data_structure(graph));

    while (graph.to_arc()[i].size() > 0)
        remove_edge(graph, graph.to_arc()[i][graph.to_arc()[i].size() - 1]);

    remove_isolated_node(graph, i);
}

inline void
remove_isolated_node(SubGraph& graph, index_t i)
{
    // assert(has_valid_data_structure(graph));
    assert(graph.N() > 0);
    assert(graph.indeg()[i] == 0);
    assert(graph.outdeg()[i] == 0);
    assert(graph.inadj()[i].size() == 0);
    assert(graph.outadj()[i].size() == 0);
    assert(graph.in2arc()[i].size() == 0);
    assert(graph.out2arc()[i].size() == 0);

    graph.indeg().erase(graph.indeg().begin() + i);
    graph.outdeg().erase(graph.outdeg().begin() + i);

    graph.inadj().erase(graph.inadj().begin() + i);
    graph.outadj().erase(graph.outadj().begin() + i);

    graph.in2arc().erase(graph.in2arc().begin() + i);
    graph.out2arc().erase(graph.out2arc().begin() + i);

    graph.local2global().erase(graph.local2global().begin() + i);

    graph.N(graph.N() - 1);

    update_indices(graph.heads(), i);
    update_indices(graph.tails(), i);
    for (auto& list : graph.inadj())
        update_indices(list, i);

    for (auto& list : graph.outadj())
        update_indices(list, i);
}

inline void
remove_node(SubGraph& graph, index_t i)
{
    // assert(has_valid_data_structure(graph));
    auto& in2arc = graph.in2arc()[i];
    while (in2arc.size() > 0)
        remove_edge(graph, in2arc[in2arc.size() - 1]);

    auto& out2arc = graph.out2arc()[i];
    while (out2arc.size() > 0)
        remove_edge(graph, out2arc[out2arc.size() - 1]);

    remove_isolated_node(graph, i);
}
