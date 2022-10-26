#include <gtest/gtest.h>

#include "discrete/algorithm/reduction/node.hpp"

TEST(node_reduction_undirected, isolated)
{
    UndirectedGraph graph(2, IndexList{}, IndexList{});
    remove_isolated_node(graph, 0);
    ASSERT_EQ(graph,
              UndirectedGraph(1, IndexList(), IndexList(), IndexList({ 1 })));
    remove_isolated_node(graph, 0);
    ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList(), IndexList()));
}

TEST(node_reduction_undirected, simple)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    remove_node(graph, 1);
    ASSERT_EQ(graph, UndirectedGraph(1, IndexList(), IndexList()));
}

TEST(node_reduction_undirected, three_nodes)
{
    UndirectedGraph graph(3, IndexList{ 0, 0 }, IndexList{ 1, 2 });
    remove_node(graph, 1);
    ASSERT_EQ(graph,
              UndirectedGraph(
                2, IndexList({ 0 }), IndexList({ 1 }), IndexList({ 0, 2 })));
}

TEST(node_reduction_undirected, cross)
{
    UndirectedGraph graph(5, IndexList{ 0, 0, 0, 0 }, IndexList{ 1, 2, 3, 4 });
    remove_node(graph, 0);
    ASSERT_EQ(
      graph,
      UndirectedGraph(4, IndexList(), IndexList(), IndexList({ 1, 2, 3, 4 })));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
