#include "fvs/solvers/fvs.hpp"

#include "fvs/discrete/discrete.hpp"

inline FVS
solve_greedy_reduction(const SubGraph& graph)
{
    const auto scc = strongly_connected_components(graph);

    FVS fvs(graph.N(), false);
    for (const auto& comp : scc) {
        auto subgraph = SubGraph::extract_subgraph(SubGraph(graph), comp);
        index_t ccf(1);
        auto start_solution = greedy_reduction(subgraph, ccf);
        const auto sub_fvs =
          solve_fvs_with_initial_solution(subgraph, start_solution);
        append_fvs(sub_fvs, subgraph, fvs);
    }
    return fvs;
}