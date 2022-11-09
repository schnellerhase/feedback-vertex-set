#include <gtest/gtest.h>

#include "discrete/algorithm/two_cycles.hpp"

using namespace fvs;

TEST(double_edges_count, empty)
{
    SubGraph graph(0, IndexList(), IndexList(), IndexList());
    ASSERT_EQ(double_edges_count(graph), 0);
    ASSERT_EQ(two_cycle_edges(graph), IndexList());
}

TEST(double_edges_count, single_node)
{
    SubGraph graph(1, IndexList(), IndexList(), IndexList{ 0 });
    ASSERT_EQ(double_edges_count(graph), 0);
    ASSERT_EQ(two_cycle_edges(graph), IndexList());
}

TEST(double_edges_count, double_edge)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    ASSERT_EQ(double_edges_count(graph), 1);
    ASSERT_EQ(two_cycle_edges(graph), IndexList({ 0, 1 }));
}

TEST(double_edges_count, double_edge_plus_node)
{
    SubGraph graph(
      3, IndexList{ 0, 1, 1 }, IndexList{ 1, 0, 2 }, IndexList{ 0, 1, 2 });
    ASSERT_EQ(double_edges_count(graph), 1);
    ASSERT_EQ(two_cycle_edges(graph), IndexList({ 0, 1 }));
}

TEST(double_edges_count, two_double_edges)
{
    SubGraph graph(3,
                   IndexList{ 0, 1, 1, 2 },
                   IndexList{ 1, 0, 2, 1 },
                   IndexList{ 0, 1, 2 });
    ASSERT_EQ(double_edges_count(graph), 2);
    ASSERT_EQ(two_cycle_edges(graph), IndexList({ 0, 1, 2, 3 }));
}
