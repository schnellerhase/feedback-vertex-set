#pragma once

#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/undirected_graph.hpp"

namespace fvs {

bool
neighborhood_is_clique(const UndirectedGraph& graph, index_t i)
{
    for (const index_t& from : graph.adj()[i]) {
        for (const index_t& to : graph.adj()[i]) {
            if (from == to)
                continue;

            if (!UndirectedGraph::has_edge(graph, from, to))
                return false;
        }
    }

    return true;
}

bool
neighborhood_is_clique_funnel(const UndirectedGraph& graph,
                              index_t v,
                              index_t u)
{
    for (const index_t& from : graph.adj()[v]) {
        for (const index_t& to : graph.adj()[v]) {
            if ((from == to) || (from == u) || (to == u))
                continue;

            if (!UndirectedGraph::has_edge(graph, from, to))
                return false;
        }
    }

    return true;
}

bool
neighborhood_is_clique(const SubGraph& graph, index_t i)
{
    if (graph.indeg()[i] != graph.outdeg()[i])
        return false;

    for (const index_t& to : graph.outadj()[i])
        if (!Graph::has_edge(graph, i, to) || !Graph::has_edge(graph, to, i))
            return false;

    for (const index_t& from : graph.inadj()[i]) {
        for (const index_t& to : graph.inadj()[i]) {
            if (from == to)
                continue;

            if (!Graph::has_edge(graph, from, to))
                return false;
        }
    }

    return true;
}

}