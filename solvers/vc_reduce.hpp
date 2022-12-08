#include <memory>

#include "fvs/solvers/vc.hpp"

using namespace fvs;

// TODO: should return a VC on graph, not output variable passed in
inline void
vc_solve_reduce(UndirectedGraph& graph, VC& vc)
{
    // VC vc(graph.N(), false);
    VCFunnelHandler vcHandler;
    for (const auto& ugraph : reduce_graph_VC(graph, vc, vcHandler)) {
        // std::cout << "VC instance N =" << ugraph.N() << " M=" << ugraph.M()
        // << std::endl;
        const auto red_vc = solve_vc(ugraph);
        // TODO: append usage!
        for (index_t i = 0; i < red_vc.size(); i++) {
            // assert(vc[ugraph.local2global(i)] == false); TODO: readd
            if (red_vc[i])
                add_to_vc(vc, i, ugraph);
            else
                add_invert_to_vc(vc, i, ugraph);
        }
    }
    vcHandler.update_vc_all(vc);
    // return vc;
}
