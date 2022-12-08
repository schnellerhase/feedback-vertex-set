#include "fvs/discrete/discrete.hpp"
#include "fvs/solvers/fvs.hpp"

// TODO: add to testing
inline FVS
solve_reduce_deg_1(SubGraph& graph)
{
    // index_t init_N = graph.N();
    index_t last_N = graph.N() + 1;

    FVS fvs(graph.N(), false);
    while (graph.N() < last_N) {
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
    }

    // std::cout << "remvoed " << init_N - graph.N() << " of " << init_N << "
    // nodes" << std::endl;
    append_fvs(solve_fvs(graph), graph, fvs);
    return fvs;
}
