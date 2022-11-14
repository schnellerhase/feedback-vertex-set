#include "fvs/discrete/discrete.hpp"
#include "fvs/fvs_solver/fvs_solver.hpp"

// TODO: add to testing

inline std::list<SubGraph>
reduce_deg_1_scc(SubGraph& graph, FVS& fvs)
{
    index_t last_N = graph.N() + 1;

    std::stack<SubGraph> to_reduce;
    to_reduce.push(std::move(graph));
    std::list<SubGraph> finished;

    while (!to_reduce.empty()) {
        auto graph = std::move(to_reduce.top());
        to_reduce.pop();
        while (graph.N() < last_N) {
            bool restart = false;
            last_N = graph.N();

            if (graph.N() == 0)
                break;

            for (index_t i = graph.N() - 1; i + 1 >= 1; i--) {
                auto indeg = graph.indeg()[i];
                auto outdeg = graph.outdeg()[i];
                if (indeg == 0 && outdeg == 0) {
                    remove_isolated_node(graph, i);
                } else if (indeg == 0 || outdeg == 0) {
                    remove_node(graph, i);
                } else if (graph.indeg()[i] == 1) {
                    bool removed_two_nodes = reduce_IN(graph, i, fvs);
                    i -= (removed_two_nodes && i > 0) * 1;
                } else if (graph.outdeg()[i] == 1) {
                    bool removed_two_nodes = reduce_OUT(graph, i, fvs);
                    i -= (removed_two_nodes && i > 0) * 1;
                }
            }

            if (graph.N() < last_N || graph.N() <= 1)
                continue;

            if (!strongly_connected(graph)) {
                IndexListList sccs = strongly_connected_components(graph);
                assert(sccs.size() > 1);
                for (const auto& scc : sccs)
                    to_reduce.push(SubGraph::extract_subgraph(graph, scc));

                graph = SubGraph(0, IndexList(), IndexList());
            }
        }
        finished.push_back(std::move(graph));
    }
    return finished;
}

inline FVS
solve_reduce_deg_1_scc(SubGraph& graph)
{
    FVS fvs(graph.N(), false);
    std::list<SubGraph> graphs = reduce_deg_1_scc(graph, fvs);
    for (const auto& graph : graphs)
        append_fvs(solve_fvs(graph), graph, fvs);
    return fvs;
}
