#include "fvs/solvers/vc.hpp"
#include "vc_solver_tests.hpp"

// TODO: ugly!
inline void
export_solve_vc(UndirectedGraph& graph, VC& vc)
{
    VC sol = solve_vc(graph);
    for (index_t i = 0; i < sol.size(); i++)
        vc[i] = sol[i];
}

VC_SOLVER_TESTS(export_solve_vc)
