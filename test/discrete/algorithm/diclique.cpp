#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/diclique.hpp"

using namespace fvs;

TEST(diclique, first)
{
    SubGraph graph(4, IndexList{ 0, 1, 1 }, IndexList{ 1, 2, 3 });
    ASSERT_FALSE(neighborhood_is_clique(graph, 0));
    ASSERT_FALSE(neighborhood_is_clique(graph, 1));
    ASSERT_FALSE(neighborhood_is_clique(graph, 2));
    ASSERT_FALSE(neighborhood_is_clique(graph, 3));
}

TEST(diclique, second)
{
    SubGraph graph(3, IndexList{ 0, 1 }, IndexList{ 1, 0 });
    ASSERT_TRUE(neighborhood_is_clique(graph, 0));
    ASSERT_TRUE(neighborhood_is_clique(graph, 1));
    ASSERT_TRUE(neighborhood_is_clique(graph, 2));
}

TEST(diclique, third)
{
    SubGraph graph(4,
                   IndexList({ 0, 1, 1, 2, 0, 2, 2 }),
                   IndexList({ 1, 0, 2, 1, 2, 0, 3 }));
    ASSERT_TRUE(neighborhood_is_clique(graph, 0));
    ASSERT_TRUE(neighborhood_is_clique(graph, 1));
    ASSERT_FALSE(neighborhood_is_clique(graph, 2));
    ASSERT_FALSE(neighborhood_is_clique(graph, 3));
}
