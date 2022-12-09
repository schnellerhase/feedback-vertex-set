#pragma once

#include "fvs/discrete/vertex_marker.hpp"

namespace fvs {

using VC = VertexMarker;

inline bool
is_VC(const VC& vc, const UndirectedGraph& graph)
{
    assert(vc.size() == graph.N());

    for (index_t m = 0; m < graph.M(); m++) {
        assert(graph.tails()[m] < graph.N());
        assert(graph.heads()[m] < graph.N());

        if ((vc[graph.tails()[m]] == false) && (vc[graph.heads()[m]] == false))
            return false;
    }
    return true;
}

inline bool
equivalent_vc(const VC& a, const VC& b, const UndirectedGraph& graph)
{
    if (a.size() != graph.N() || b.size() != graph.N())
        return false;

    if (a == b)
        return true;

    if (!is_VC(a, graph) || !is_VC(b, graph))
        return false;

    auto count_a = std::count(a.begin(), a.end(), true);
    auto count_b = std::count(b.begin(), b.end(), true);

    return count_a == count_b;
}

inline void
add_to_vc(VC& vc, index_t i, const UndirectedGraph& graph)
{
    for (const auto& gl_i : graph.local2global(i))
        vc[gl_i] = true;
}

inline void
add_invert_to_vc(VC& vc, index_t i, const UndirectedGraph& graph)
{
    for (const auto& gl_i : graph.local2globalInvert(i))
        vc[gl_i] = true;
}
}
