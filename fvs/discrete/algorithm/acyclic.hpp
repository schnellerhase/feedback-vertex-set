#pragma once

#include <queue>

#include "fvs/discrete/util/types.hpp"
#include "fvs/discrete/graph.hpp"

namespace fvs {

bool is_acyclic(const Graph& graph)
{
    // TODO assert graph does not have self loops

    if (graph.N() == 0)
        return true;

    std::queue<index_t> sources;
    for (index_t i = 0; i < graph.N(); i++)
        if (graph.indeg()[i] == 0)
            sources.push(i);

    if (sources.empty())
        return false;

    IndexList indeg(graph.indeg());

    index_t visited_nodes = 0;
    while (!sources.empty()) {
        index_t u = sources.front();
        sources.pop();
        visited_nodes++;

        for (const index_t v : graph.outadj()[u]) {
            indeg[v]--;
            if (indeg[v] == 0)
                sources.push(v);
        }
    }

    return (visited_nodes == graph.N());
}

bool is_acyclic(const Graph& graph, const FVS& fvs)
{
    auto size_fvs = std::count(fvs.begin(), fvs.end(), true);

    if (graph.N() - size_fvs == 0)
        return true;

    IndexList indeg(graph.N(), 0);
    for (index_t u = 0; u < graph.N(); ++u)
        if (!fvs[u])
            for (const index_t v : graph.outadj()[u])
                ++indeg[v];

    std::queue<index_t> sources;
    for (index_t i = 0; i < graph.N(); i++)
        if (!fvs[i] && indeg[i] == 0)
            sources.push(i);

    index_t visited_nodes = 0;
    while (!sources.empty()) {
        index_t u = sources.front();
        sources.pop();
        visited_nodes++;

        for (const index_t v : graph.outadj()[u]) {
            indeg[v]--;
            if (!fvs[v] && indeg[v] == 0)
                sources.push(v);
        }
    }

    return (visited_nodes == (graph.N() - size_fvs));
}

}