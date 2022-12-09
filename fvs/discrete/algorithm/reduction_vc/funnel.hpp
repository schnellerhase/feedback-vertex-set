#pragma once

#include "fvs/discrete/undirected_graph.hpp"
#include "fvs/discrete/vc_funnel_handler.hpp"
#include "fvs/discrete/algorithm/neighborhood.hpp"
#include "fvs/discrete/algorithm/reduction/edge.hpp"
#include "fvs/discrete/algorithm/reduction/node.hpp"
#include "fvs/discrete/algorithm/vc.hpp"

namespace fvs {

using VC = VertexMarker;

inline void
reduce_FUNNEL(UndirectedGraph& graph,
              index_t v,
              index_t u,
              VC& vc,
              VCFunnelHandler& vcHandler)
{
    auto [intersection, vExclusive, uExclusive] = decompose_nhoods(graph, v, u);

    // update handler
    IndexList vExclusiveGlobal({});
    IndexList uExclusiveGlobal({});
    IndexList local2global;

    for (const auto& vNeighbor : vExclusive) {
        local2global = graph.local2global(vNeighbor);
        vExclusiveGlobal.insert(
          vExclusiveGlobal.end(), local2global.begin(), local2global.end());
    }

    for (auto uNeighbor : uExclusive) {
        local2global = graph.local2global(uNeighbor);
        uExclusiveGlobal.insert(
          uExclusiveGlobal.end(), local2global.begin(), local2global.end());
    }
    vcHandler.update(graph.local2global(v),
                     graph.local2globalInvert(v),
                     graph.local2global(u),
                     graph.local2globalInvert(u),
                     vExclusiveGlobal,
                     uExclusiveGlobal);

    // add edges
    for (const auto& v : vExclusive)
        for (const auto& u : uExclusive)
            add_edge(graph, v, u);

    // remove Intersection
    for (index_t node(intersection.size() - 1); (node + 1) >= 1; node--) {
        add_to_vc(vc, intersection[node], graph);
        remove_node(graph, intersection[node]);
        v -= (intersection[node] < v) * 1;
        u -= (intersection[node] < u) * 1;
    }

    // remove v and u
    remove_node(graph, v);
    u -= (v < u) * 1;
    remove_node(graph, u);

    return;
}
}