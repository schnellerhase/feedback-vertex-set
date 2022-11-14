#pragma once

#include <gtest/gtest.h>

#include "fvs/discrete/discrete.hpp"

using VC = VertexMarker;

// TODO: move of solver yields very ugly syntax
#define TEST_VC_TRACK(solver, no)                                              \
    TEST(solver, no)                                                           \
    {                                                                          \
        std::string track = #no;                                               \
        auto graph =                                                           \
          UndirectedGraph::read("data/vc/vc-exact_" + track + ".gr");          \
        auto copy =                                                            \
          UndirectedGraph::read("data/vc/vc-exact_" + track + ".gr");          \
        VC vc = VC::read("data/vc/vc-exact_" + track + ".gr.sol", graph.N());  \
        VC sol(graph.N(), false);                                              \
        solver(graph, sol);                                                    \
        ASSERT_TRUE(equivalent_vc(sol, vc, copy));                             \
    }

#define VC_SOLVER_TESTS(solver)                                                \
    TEST_VC_TRACK(solver, 001)                                                 \
    TEST_VC_TRACK(solver, 003)                                                 \
    TEST_VC_TRACK(solver, 005)                                                 \
    TEST_VC_TRACK(solver, 007)                                                 \
    TEST_VC_TRACK(solver, 011)
