#ifndef SOLVER_TESTS_HPP
#define SOLVER_TESTS_HPP

#include <gtest/gtest.h>

#include "fvs/solvers/fvs.hpp"

#define TEST_SINGLE_NODE(solver)                                               \
    TEST(solver, single_node)                                                  \
    {                                                                          \
        SubGraph graph(1, IndexList{}, IndexList{});                           \
        FVS fvs{ false };                                                      \
        ASSERT_EQ(solver(graph), fvs);                                         \
    }

#define TEST_SIMPLE(solver)                                                    \
    TEST(solver, simple)                                                       \
    {                                                                          \
        SubGraph graph(                                                        \
          4, IndexList({ 0, 0, 1, 2, 3 }), IndexList({ 1, 2, 2, 3, 0 }));      \
        FVS fvs{ false, false, false, true };                                  \
        ASSERT_TRUE(equivalent_fvs(solver(graph), fvs, graph));                \
    }

#define TEST_2_SCCS(solver)                                                    \
    TEST(solver, again)                                                        \
    {                                                                          \
        SubGraph graph(                                                        \
          8,                                                                   \
          IndexList{ 0, 2, 0, 1, 1, 2, 2, 3, 5, 5, 4, 4, 3, 5, 6, 7, 6 },      \
          IndexList{ 2, 0, 1, 0, 2, 1, 3, 5, 3, 4, 5, 3, 4, 6, 7, 6, 0 });     \
        SubGraph subgraph(graph);                                              \
        FVS fvs{ true, true, false, true, true, false, true, false };          \
        ASSERT_TRUE(equivalent_fvs(solver(subgraph), fvs, graph));             \
    }

#define TEST_DISJOINT(solver)                                                  \
    TEST(solver, disjoint)                                                     \
    {                                                                          \
        SubGraph graph(6,                                                      \
                       IndexList({ 0, 1, 2, 3, 4, 5 }),                        \
                       IndexList({ 1, 2, 0, 4, 5, 3 }));                       \
        FVS fvs{ false, false, true, false, false, true };                     \
        ASSERT_TRUE(equivalent_fvs(solver(graph), fvs, graph));                \
    }

#define TEST_DOUBLE_SCC(solver)                                                \
    TEST(solver, double_scc)                                                   \
    {                                                                          \
        auto graph =                                                           \
          SubGraph(6,                                                          \
                   IndexList{ 0, 0, 1, 1, 2, 2, 0, 3, 4, 5, 3, 4, 5 },         \
                   IndexList{ 3, 1, 0, 2, 1, 0, 2, 4, 5, 3, 5, 3, 4 });        \
        FVS fvs = { false, true, true, false, true, true };                    \
        ASSERT_TRUE(equivalent_fvs(fvs, solver(graph), graph));                \
    }

#define TEST_TRACK(solver, no)                                                 \
    TEST(solver, no)                                                           \
    {                                                                          \
        std::string track = #no;                                               \
        auto graph = SubGraph(Graph::read(("../tracks/e_" + track).c_str()));  \
        auto fvs = FVS::read("data/e_" + track + ".sol", graph.N());           \
        ASSERT_TRUE(equivalent_fvs(solver(graph), fvs, graph));                \
    }

#define TEST_SORTED_ADJ(solver)                                                \
    TEST(solver, debug_graph)                                                  \
    {                                                                          \
        auto graph = SubGraph(Graph::read("data/debug_graph"));                \
        FVS fvsShould = solve_fvs(graph);                                      \
        FVS fvs = solver(graph);                                               \
        ASSERT_TRUE(equivalent_fvs(fvs, fvsShould, graph));                    \
    }

#endif // SOLVER_TESTS_HPP
