#include <benchmark/benchmark.h>
#include <iomanip>
#include <iostream>

#include "discrete/discrete.hpp"
#include "solvers/naive.hpp"

static void
BM_naive(benchmark::State& state)
{
    const index_t& graph_no = state.range(0);
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << graph_no;
    Graph graph = Graph::read("tracks/e_" + ss.str());

    for (auto _ : state)
        solve_naive(graph);
}

BENCHMARK(BM_naive)->DenseRange(1, 50, 2);
BENCHMARK_MAIN();
