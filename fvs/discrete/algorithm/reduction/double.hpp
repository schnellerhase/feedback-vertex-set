#pragma once

#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/algorithm/scc.hpp"

namespace fvs {

void
reduce_DOUBLE(const SubGraph& subgraph,
              index_t v,
              index_t w,
              IndexList& edges_to_remove)
{
    assert(v < subgraph.N());
    assert(w < subgraph.N());
    // TODO: assert v, w, double cycle

    const IndexList sccWithoutV = scc_double_version(subgraph, v);
    const IndexList sccWithoutW = scc_double_version(subgraph, w);

    for (index_t m = 0; m < subgraph.M(); m++) {
        const index_t head = subgraph.heads()[m];
        const index_t tail = subgraph.tails()[m];

        if (head == v || tail == v) // only Ewv possible
        {
            if ((head != w) && (tail != w) &&
                (sccWithoutW[head] != sccWithoutW[tail]))
                edges_to_remove.push_back(m);
        } else if (head == w || tail == w) // only Evw possible
        {
            if ((head != v) && (tail != v) &&
                (sccWithoutV[head] != sccWithoutV[tail]))
                edges_to_remove.push_back(m);
        } else // Ev intersected Ew
        {
            if ((sccWithoutV[head] != sccWithoutV[tail]) &&
                (sccWithoutW[head] != sccWithoutW[tail]))
                edges_to_remove.push_back(m);
        }
    }
}

}