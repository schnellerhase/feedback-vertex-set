#pragma once

#include "vertexcover.hpp"

VC
solve_vc(const UndirectedGraph& graph)
{
    VCoverSolver solver(graph);
    return solver.solve();
}
