#include <gtest/gtest.h>

#include "discrete/discrete.hpp"

TEST(SimpleReductions, remove_edge_single)
{
    SubGraph graph(2, IndexList{ 0 }, IndexList{ 1 }, IndexList{ 0, 1 });
    remove_edge(graph, static_cast<index_t>(0));
    ASSERT_EQ(graph,
              SubGraph(2, IndexList({}), IndexList({}), IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_edge_double)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    remove_edge(graph, static_cast<index_t>(1));
    ASSERT_EQ(
      graph,
      SubGraph(2, IndexList({ 0 }), IndexList({ 1 }), IndexList({ 0, 1 })));
    remove_edge(graph, static_cast<index_t>(0));
    ASSERT_EQ(graph,
              SubGraph(2, IndexList({}), IndexList({}), IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_edge_double_loop)
{
    SubGraph graph(2, IndexList{ 0, 0 }, IndexList{ 1, 1 }, IndexList{ 0, 1 });
    remove_edge(graph, static_cast<index_t>(1));
    ASSERT_EQ(
      graph,
      SubGraph(2, IndexList({ 0 }), IndexList({ 1 }), IndexList({ 0, 1 })));
    remove_edge(graph, static_cast<index_t>(0));
    ASSERT_EQ(graph,
              SubGraph(2, IndexList({}), IndexList({}), IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_edge_two_nodes)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    remove_edge(graph, static_cast<index_t>(0));
    ASSERT_EQ(
      graph,
      SubGraph(2, IndexList({ 1 }), IndexList({ 0 }), IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_edge_3)
{
    SubGraph graph(1, IndexList{ 0, 0 }, IndexList{ 0, 0 }, IndexList{ 0 });
    remove_edge(graph, static_cast<index_t>(1));
    remove_edge(graph, static_cast<index_t>(0));
    ASSERT_EQ(graph,
              SubGraph(1, IndexList({}), IndexList({}), IndexList({ 0 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_edge_4)
{
    SubGraph graph(
      2, IndexList{ 0, 0, 1, 1 }, IndexList{ 1, 1, 0, 0 }, IndexList{ 0, 1 });
    remove_edge(graph, static_cast<index_t>(1));
    ASSERT_EQ(graph,
              SubGraph(2,
                       IndexList({ 0, 1, 1 }),
                       IndexList({ 1, 0, 0 }),
                       IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_isolated_node_0)
{
    SubGraph graph(1, IndexList{}, IndexList{}, IndexList{ 0 });
    remove_isolated_node(graph, static_cast<index_t>(0));
    ASSERT_EQ(graph, SubGraph(0, IndexList({}), IndexList({}), IndexList({})));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_isolated_node_1)
{
    SubGraph graph(
      3, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1, 2 });
    remove_isolated_node(graph, static_cast<index_t>(2));
    ASSERT_EQ(
      graph,
      SubGraph(
        2, IndexList({ 0, 1 }), IndexList({ 1, 0 }), IndexList({ 0, 1 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(SimpleReductions, remove_isolated_node_2)
{
    SubGraph graph(
      3, IndexList{ 1, 2 }, IndexList{ 2, 1 }, IndexList{ 0, 1, 2 });
    remove_isolated_node(graph, static_cast<index_t>(0));
    ASSERT_EQ(
      graph,
      SubGraph(
        2, IndexList({ 0, 1 }), IndexList({ 1, 0 }), IndexList({ 1, 2 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
