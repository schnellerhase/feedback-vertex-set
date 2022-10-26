#pragma once

#include "../../subgraph.hpp"
#include "../scc.hpp"
#include "../two_cycles.hpp"
#include "edge.hpp"

#define PIE_DOUBLE_CYCLE_DENSITY_SWITCH                                        \
    .3 // TODO: this should be chose, s.t. the method to cosntruc tsubgraph is
       // always fastest

namespace {}

SubGraph
remove_double_edges(const SubGraph& graph)
{
    index_t double_edges = double_edges_count(graph);
    double perc = static_cast<double>(double_edges) * 2 / graph.M();

    if (perc > PIE_DOUBLE_CYCLE_DENSITY_SWITCH) {
        auto [tails, heads] =
          find_all_none_2_cycle_edges(graph, graph.M() - 2 * double_edges);
        return SubGraph(graph.N(), tails, heads, graph.local2global());
    } else {
        // remove double edges.
        IndexList to_remove = two_cycle_edges(graph);
        assert(std::is_sorted(to_remove.begin(), to_remove.end()));

        SubGraph result(graph);
        for (index_t m = to_remove.size() - 1; m + 1 >= 1; m--)
            remove_edge(result, to_remove[m]);

        return result;
    }
}

void
remove_edges_tails_heads_orderd(
  SubGraph& subgraph,
  std::vector<std::tuple<index_t, index_t>>& tailsHeadsToRemove)
{
    index_t edgeToRemoveItterator = tailsHeadsToRemove.size();
    index_t edgeItterator = subgraph.M();
    if (edgeToRemoveItterator == 0)
        return;

    index_t tempTail =
      std::get<0>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
    index_t tempHead =
      std::get<1>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
    while (edgeToRemoveItterator > 0) {
        edgeItterator--;
        if ((tempTail == subgraph.tails()[edgeItterator]) &&
            (tempHead == subgraph.heads()[edgeItterator])) {
            remove_edge(subgraph, edgeItterator);
            edgeToRemoveItterator--;
            if (edgeToRemoveItterator > 0) {
                tempTail =
                  std::get<0>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
                tempHead =
                  std::get<1>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
            }
        }
    }
    return;
}

void
reduce_PIE(SubGraph& graph)
{
    // TODO: this shoudl prob be an assertion rather?
    if (graph.M() == 0)
        return;

    SubGraph removed = remove_double_edges(graph);

    IndexList scc_comp = low_links_sccs(removed);
    // here we shoudl prob, rather create a list of edge indices to remvoe, as
    // speciall in remove this requires a full loop over graph
    std::vector<std::tuple<index_t, index_t>> edges_to_remove;
    for (index_t m = 0; m < removed.M(); m++) {
        index_t head = removed.heads()[m];
        index_t tail = removed.tails()[m];
        if (scc_comp[head] != scc_comp[tail])
            edges_to_remove.push_back(std::make_tuple(tail, head));
    }

    remove_edges_tails_heads_orderd(graph, edges_to_remove);
}
