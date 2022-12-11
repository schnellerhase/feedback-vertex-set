#include <benchmark/benchmark.h>
#include <iomanip>
#include <iostream>

#include "fvs/discrete/discrete.hpp"
#include "solvers/reduce.hpp"

static void
BM_reduce(benchmark::State& state)
{
    const index_t& graph_no = state.range(0);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << graph_no;
    SubGraph graph = SubGraph(Graph::read("tracks/e_" + ss.str()));

    for (auto _ : state)
        solve_reduce(graph);
}

// BENCHMARK(BM_reduce)->DenseRange(1, 50, 2);
BENCHMARK(BM_reduce)->DenseRange(1, 36, 2);
BENCHMARK_MAIN();
