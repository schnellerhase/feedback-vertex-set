#include <gtest/gtest.h>

#include "discrete/algorithm/neighborhood.hpp"

TEST(neighborhood, single_node)
{
    SubGraph graph(1, IndexList{}, IndexList{}, IndexList{ 0 });
    ASSERT_EQ(neighborhood(graph, 0), IndexList({ 0 }));
}

TEST(neighborhood, two_nodes_connected)
{
    SubGraph graph(2, IndexList{ 0 }, IndexList{ 1 }, IndexList{ 0, 1 });
    ASSERT_EQ(neighborhood(graph, 0), IndexList({ 0, 1 }));
}

TEST(neighborhood, two_nodes_unconnected)
{
    SubGraph graph(2, IndexList{}, IndexList{}, IndexList{ 0, 1 });
    ASSERT_EQ(neighborhood(graph, 0), IndexList({ 0 }));
}

TEST(neighborhood, four_nodes)
{
    SubGraph graph(
      4, IndexList{ 0, 1, 1 }, IndexList{ 1, 2, 3 }, IndexList{ 0, 1, 2, 3 });
    ASSERT_EQ(neighborhood(graph, 1), IndexList({ 0, 1, 2, 3 }));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
