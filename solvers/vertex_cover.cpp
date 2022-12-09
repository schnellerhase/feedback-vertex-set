#include "fvs/solvers/vc.hpp"
#include "template.hpp"

using namespace fvs;

// TODO: ugly!
inline void
export_solve_vc(UndirectedGraph& graph, VC& vc)
{
    VC sol = solve_vc(graph);
    for (index_t i = 0; i < sol.size(); i++)
        vc[i] = sol[i];
}

#ifndef __unix__
#pragma warning(push)
#pragma warning(disable: 4068)
#endif

EXPORT_VC(export_solve_vc)

#ifndef __unix__
#pragma warning(pop)
#endif
