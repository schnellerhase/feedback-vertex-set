#pragma once

#include "discrete/discrete.hpp"
#include "fvs_solver/feedback.hpp"

FVS
solve_fvs(const Graph& graph)
{
    // if we want to run this on SCC, then we may assume strongly connected, as
    // specially for case N == 2.
    if (graph.N() == 0 || graph.N() == 1)
        return FVS(graph.N(), false);

    if (graph.N() == 2) {
        if (graph.M() == 2)
            return FVS({ false, true });
        else
            return FVS({ false, false });
    }
    FeedbackSolver solver(graph);
    bool solved = solver.solve();
    if (solved) {
        FVS solution(graph.N(), false);
        for (index_t i = 0; i < solution.size(); i++)
            solution[i] = (solver.solution()[i] > 0.9);

        return solution;
    }

    std::cout << "solve failed!" << std::endl;
    exit(2);
}

FVS
solve_fvs_with_initial_solution(const Graph& graph, const FVS& fvs)
{
    // if we want to run this on SCC, then we may assume strongly connected, as
    // specially for case N == 2.
    if (graph.N() == 0 || graph.N() == 1)
        return FVS(graph.N(), false);

    if (graph.N() == 2) {
        if (graph.M() == 2)
            return FVS({ false, true });
        else
            return FVS({ false, false });
    }

    FeedbackSolver solver(graph);
    solver.set_initial_solution(fvs);
    bool solved = solver.solve();
    if (solved) {
        FVS solution(graph.N());
        for (index_t i = 0; i < solution.size(); i++)
            solution[i] = (solver.solution()[i] > 0.9);

        return solution;
    }

    std::cout << "solve failed!" << std::endl;
    exit(2);
}
