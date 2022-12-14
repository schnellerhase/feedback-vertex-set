#pragma once

#include <optional>

#include "fvs/discrete/discrete.hpp"
#include "fvs/solvers/detail/fvs.hpp"

// using namespace fvs;

FVS
solve_fvs(const Graph& graph, const std::optional<FVS>& fvs)
{
    // if we want to run this on SCC, then we may assume strongly connected, as
    // specially for case N == 2.
    if (graph.N() == 0 || graph.N() == 1)
        return FVS(graph.N(), false); // NOLINT

    if (graph.N() == 2) {
        if (graph.M() == 2)
            return FVS({ false, true });
        else
            return FVS({ false, false });
    }
    detail::FeedbackSolver solver(graph);
    if (fvs)
        solver.set_initial_solution(*fvs);

    return solver.solve();
}

FVS
solve_fvs(const Graph& graph)
{
    return solve_fvs(graph, {});
}