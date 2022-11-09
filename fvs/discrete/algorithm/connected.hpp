#pragma once

#include "../subgraph.hpp"
#include "../undirected_graph.hpp"
#include "dfs.hpp"

bool
is_connected(const UndirectedGraph& graph)
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
is_strongly_connected(const SubGraph& graph)
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