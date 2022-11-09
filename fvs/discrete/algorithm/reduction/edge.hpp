#pragma once

#include "util.hpp"

#include "../../undirected_graph.hpp"
#include "../diclique.hpp"

inline void
remove_edge(UndirectedGraph& graph, index_t e)
{
    // assert(has_valid_data_structure(graph));

    assert(graph.M() > 0);

    const index_t& i = graph.tails()[e];
    const index_t& j = graph.heads()[e];

    assert(graph.deg()[i] > 0);
    assert(graph.deg()[j] > 0);

    graph.deg()[i] -= 1;
    graph.deg()[j] -= 1;

    {
        auto ptr = std::lower_bound(
          graph.to_arc()[j].begin(), graph.to_arc()[j].end(), e);
        assert(ptr != graph.to_arc()[j].end());
        assert(*ptr == e);
        auto idx = std::distance(graph.to_arc()[j].begin(), ptr);

        graph.to_arc()[j].erase(graph.to_arc()[j].begin() + idx);
        graph.adj()[j].erase(graph.adj()[j].begin() + idx);
    }

    {
        auto ptr = std::lower_bound(
          graph.to_arc()[i].begin(), graph.to_arc()[i].end(), e);
        assert(ptr != graph.to_arc()[i].end());
        assert(*ptr == e);
        auto idx = std::distance(graph.to_arc()[i].begin(), ptr);

        graph.to_arc()[i].erase(graph.to_arc()[i].begin() + idx);
        graph.adj()[i].erase(graph.adj()[i].begin() + idx);
    }

    erase_index(graph.tails(), e);
    erase_index(graph.heads(), e);

    for (auto& list : graph.to_arc())
        update_indices(list, e);

    graph.M(graph.M() - 1);
}

inline void
add_edge(UndirectedGraph& graph, index_t tail, index_t head)
{
    // assert(has_valid_data_structure(graph));

    if (UndirectedGraph::has_edge(graph, tail, head))
        return;

    graph.tails().push_back(tail);
    graph.heads().push_back(head);

    graph.deg()[tail] += 1;
    graph.deg()[head] += 1;

    graph.to_arc()[tail].push_back(graph.M());
    graph.to_arc()[head].push_back(graph.M());

    graph.adj()[tail].push_back(head);
    graph.adj()[head].push_back(tail);

    graph.M(graph.M() + 1);
}

inline void
remove_edge(SubGraph& graph, index_t e)
{
    // assert(has_valid_data_structure(graph));

    assert(graph.M() > 0);

    const index_t& i = graph.tails()[e];
    const index_t& j = graph.heads()[e];

    assert(graph.outdeg()[i] > 0);
    assert(graph.indeg()[j] > 0);

    graph.outdeg()[i] -= 1;
    graph.indeg()[j] -= 1;

    {
        auto ptr = std::lower_bound(
          graph.in2arc()[j].begin(), graph.in2arc()[j].end(), e);
        assert(ptr != graph.in2arc()[j].end());
        assert(*ptr == e);
        auto idx = std::distance(graph.in2arc()[j].begin(), ptr);

        graph.in2arc()[j].erase(graph.in2arc()[j].begin() + idx);
        graph.inadj()[j].erase(graph.inadj()[j].begin() + idx);
    }

    {
        auto ptr = std::lower_bound(
          graph.out2arc()[i].begin(), graph.out2arc()[i].end(), e);
        assert(ptr != graph.out2arc()[i].end());
        assert(*ptr == e);
        auto idx = std::distance(graph.out2arc()[i].begin(), ptr);

        graph.out2arc()[i].erase(graph.out2arc()[i].begin() + idx);
        graph.outadj()[i].erase(graph.outadj()[i].begin() + idx);
    }

    erase_index(graph.tails(), e);
    erase_index(graph.heads(), e);

    for (auto& list : graph.in2arc())
        update_indices(list, e);

    for (auto& list : graph.out2arc())
        update_indices(list, e);

    graph.M(graph.M() - 1);
}
