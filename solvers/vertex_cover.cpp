#include "template.hpp"
#include "vc_solver/vc_solver.hpp"

using namespace fvs;

// TODO: ugly!
inline void
export_solve_vc(UndirectedGraph& graph, VC& vc)
{
    VC sol = solve_vc(graph);
    for (index_t i = 0; i < sol.size(); i++)
        vc[i] = sol[i];
}

EXPORT_VC(export_solve_vc)