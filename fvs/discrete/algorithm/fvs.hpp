#pragma once

#include "fvs/discrete/graph.hpp"
#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/undirected_graph.hpp"
#include "fvs/discrete/algorithm/acyclic.hpp"
namespace fvs {

using FVS = VertexMarker;

inline void
append_fvs(const FVS& sub_fvs, const SubGraph& sub_graph, FVS& fvs)
{
    assert(sub_graph.N() == sub_fvs.size());
    assert(sub_graph.N() <= fvs.size());

    for (index_t i_l = 0; i_l < sub_graph.N(); i_l++)
        fvs[sub_graph.local2global(i_l)] = sub_fvs[i_l];
}

inline bool
equivalent_fvs(const FVS& a, const FVS& b, const Graph& graph)
{
    if (a.size() != graph.N() || b.size() != graph.N())
        return false;

    if (a == b)
        return true;

    auto count_a = std::count(a.begin(), a.end(), true);
    auto count_b = std::count(b.begin(), b.end(), true);
    if (count_a != count_b)
        return false;

    return is_acyclic(graph, a) && is_acyclic(graph, b);
}

}