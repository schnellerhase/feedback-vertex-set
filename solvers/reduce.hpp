#include "fvs/fvs_solver/fvs_solver.hpp"
#include "vc_reduce.hpp"
#include "fvs/solvers/vc.hpp"

using namespace fvs;

inline FVS
solve_reduce(SubGraph& graph)
{
    // std::cout << "N=" << graph.N() << ", M=" << graph.M() << std::endl;
    auto [subgraphs, fvs, vc_graphs] = fully_reduce_graph(graph);
    // std::cout << "subgraph count=" << subgraphs.size() << std::endl;
    // std::cout << "VC count=" << undirected_components.size() << std::endl;
    // exit(0);
    for (auto& subgraph : subgraphs) {
        // std::cout << "DIRECTED: N=" << subgraph.N() << ", M=" << subgraph.M()
        // << std::endl;
        const auto reduced_fvs = solve_fvs(subgraph);

        for (index_t i = 0; i < reduced_fvs.size(); i++)
            fvs[subgraph.local2global(i)] = reduced_fvs[i];
    }

    for (auto& ugraph : vc_graphs)
        vc_solve_reduce(ugraph, fvs);

    // std:: cout << "fvs[822] = " << fvs[822] << std::endl;
    // std:: cout << "fvs[823] = " << fvs[823] << std::endl;
    // std:: cout << "fvs[843] = " << fvs[843] << std::endl;
    // std:: cout << "fvs[844] = " << fvs[844] << std::endl;
    // std:: cout << "fvs[871] = " << fvs[871] << std::endl;
    // std:: cout << "fvs[873] = " << fvs[873] << std::endl;
    // write("solution_157_new_funnel_reduce", fvs );
    return fvs;
}
