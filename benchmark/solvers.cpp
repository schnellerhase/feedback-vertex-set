#include <benchmark/benchmark.h>
#include <iomanip>
#include <iostream>

#include "fvs/discrete/discrete.hpp"
#include "solvers/reduce.hpp"
#include "solvers/naive.hpp"

auto read_graph(index_t graph_no)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(3) << graph_no;
    return Graph::read("tracks/e_" + ss.str());
}

static void
BM_naive(benchmark::State& state)
{  
    Graph graph = read_graph(state.range(0));
    for (auto _ : state)
        solve_naive(graph);
}
BENCHMARK(BM_naive)->DenseRange(1, 26, 2);


static void
BM_reduce(benchmark::State& state)
{
    SubGraph graph = SubGraph(read_graph(state.range(0)));
    for (auto _ : state)
        solve_reduce(graph);
}

// BENCHMARK(BM_reduce)->DenseRange(1, 50, 2);
BENCHMARK(BM_reduce)->DenseRange(1, 36, 2)->Iterations(10);
BENCHMARK_MAIN();
