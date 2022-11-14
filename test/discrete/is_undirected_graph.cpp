#include <gtest/gtest.h>

#include "fvs/discrete/discrete.hpp"

TEST(is_undirected_graph, empty)
{
    SubGraph graph(0, IndexList{}, IndexList{}, IndexList{});
    ASSERT_TRUE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, sinlge_node)
{
    SubGraph graph(1, IndexList{}, IndexList{}, IndexList{ 0 });
    ASSERT_TRUE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, two_nodes_0)
{
    SubGraph graph(2, IndexList({ 0 }), IndexList({ 1 }), IndexList({ 0, 1 }));
    ASSERT_FALSE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, two_nodes_1)
{
    SubGraph graph(
      2, IndexList({ 0, 1 }), IndexList({ 1, 0 }), IndexList({ 0, 1 }));
    ASSERT_TRUE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, three_nodes_0)
{
    ASSERT_TRUE(SubGraph::is_undirected_graph(SubGraph(
      3, IndexList({ 0, 1 }), IndexList({ 1, 0 }), IndexList({ 0, 1, 2 }))));
}

TEST(is_undirected_graph, three_nodes_1)
{
    SubGraph graph(3,
                   IndexList({ 0, 1, 1, 2 }),
                   IndexList({ 1, 0, 2, 1 }),
                   IndexList({ 0, 1, 2 }));
    ASSERT_TRUE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, three_nodes_2)
{
    SubGraph graph(3,
                   IndexList({ 0, 1, 1, 2 }),
                   IndexList({ 1, 0, 2, 0 }),
                   IndexList({ 0, 1, 2 }));
    ASSERT_FALSE(SubGraph::is_undirected_graph(graph));
}

TEST(is_undirected_graph, e_051)
{
    SubGraph graph(Graph::read("../tracks/e_051"));
    ASSERT_TRUE(SubGraph::is_undirected_graph(graph));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
