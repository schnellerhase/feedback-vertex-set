#pragma once

#include <algorithm>

#include "../undirected_graph.hpp"
#include "dfs.hpp"
#include "fvs.hpp"

namespace fvs
{

IndexListList
connected_components(const UndirectedGraph& graph)
{
    IndexListList result;
    if (graph.N() == 0)
        return result;

    DFS<UndirectedGraph, DFS_forward> dfs(graph);
    for (index_t i = 0; i < graph.N(); i++) {
        if (dfs.visited(i))
            continue;

        dfs.current(i);
        IndexList cc;
        while (dfs.next())
            cc.push_back(*dfs);

        std::sort(cc.begin(), cc.end());
        result.push_back(cc);
    }

    return result;
}

UndirectedGraph
extract_subgraph_undirected(const UndirectedGraph& graph,
                            const IndexList& nodes)
{
    assert(std::is_sorted(nodes.begin(), nodes.end()));
    assert(std::all_of(nodes.begin(), nodes.end(), [&graph](const index_t i) {
        return i < graph.N();
    }));

    index_t N_l = nodes.size();

    // TODO: here we trade memory for runtime, right?, good trade?
    IndexList local_indices(graph.N());
    for (index_t i = 0; i < graph.N(); i++) {
        auto ptr = std::find(nodes.begin(), nodes.end(), i);
        local_indices[i] = std::distance(nodes.begin(), ptr);
        assert(local_indices[i] <= N_l);
    }

    IndexList tails_l; // TODO: reserve some appropiate size
    IndexList heads_l; // TODO: reserve some appropiate size
    for (index_t m = 0; m < graph.M(); m++) {
        auto tail = local_indices[graph.tails()[m]];
        auto head = local_indices[graph.heads()[m]];
        if ((tail < N_l) && (head < N_l)) {
            tails_l.push_back(tail);
            heads_l.push_back(head);
        }
    }

    IndexListList local2global_l(N_l);
    IndexListList local2globalInvert_l(N_l);
    for (index_t i = 0; i < N_l; i++) {
        local2global_l[i] = graph.local2global(nodes[i]);
        local2globalInvert_l[i] = graph.local2globalInvert(nodes[i]);
    }

    return { N_l, tails_l, heads_l, local2global_l, local2globalInvert_l };
}

}