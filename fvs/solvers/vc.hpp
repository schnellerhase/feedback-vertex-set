#pragma once

#include "fvs/solvers/detail/vc.hpp"

namespace fvs {

VC
solve_vc(const UndirectedGraph& graph)
{
    detail::VCoverSolver solver(graph);
    return solver.solve();
}

}