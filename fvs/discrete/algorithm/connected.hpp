#pragma once

#include "fvs/discrete/undirected_graph.hpp"
#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/algorithm/dfs.hpp"

namespace fvs
{

bool
connected(const UndirectedGraph& graph)
{
    if (graph.N() == 0)
        return true;

    index_t count = 0;

    DFS<UndirectedGraph, DFS_forward> dfs(graph);
    while (dfs.next())
        count++;

    return (count == graph.N());
}

bool
strongly_connected(const SubGraph& graph)
{
    if (graph.N() == 0)
        return true;

    index_t count = 0;
    DFS<SubGraph, DFS_forward> dfs(graph);
    while (dfs.next())
        count++;

    if (count != graph.N())
        return false;

    count = 0;
    DFS<SubGraph, DFS_backwards> dfs_back(graph);
    while (dfs_back.next())
        count++;

    return (count == graph.N());
}

}