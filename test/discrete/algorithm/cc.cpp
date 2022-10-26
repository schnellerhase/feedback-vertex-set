#include <gtest/gtest.h>

#include "discrete/algorithm/cc.hpp"

TEST(connected, empty)
{
    UndirectedGraph graph(0, IndexList(), IndexList(), IndexList());
    FVS visited(0, false);
    // ASSERT_EQ(dfs(graph, 0, visited), IndexList());
    ASSERT_EQ(connected_components(graph), IndexListList());
}

TEST(connected, single)
{
    UndirectedGraph graph(1, IndexList(), IndexList());
    FVS visited(1, false);
    // ASSERT_EQ(dfs(graph, 0, visited), IndexList({0}));
    ASSERT_EQ(connected_components(graph), IndexListList({ IndexList({ 0 }) }));
}

TEST(connected, two_nodes_0)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    FVS visited(2, false);
    // ASSERT_EQ(dfs(graph, 0, visited), IndexList({0,1}));
    ASSERT_EQ(connected_components(graph),
              IndexListList({ IndexList({ 0, 1 }) }));
}

TEST(connected, four_node_circle)
{
    UndirectedGraph graph(4, IndexList{ 0, 1, 2, 3 }, IndexList{ 1, 2, 3, 0 });
    FVS visited(4, false);
    // ASSERT_EQ(dfs(graph, 2, visited), IndexList({0,1,2,3}));
    ASSERT_EQ(connected_components(graph),
              IndexListList({ IndexList({ 0, 1, 2, 3 }) }));
}

TEST(connected, scc_complex_4)
{
    UndirectedGraph graph(7, IndexList{ 0, 1, 3, 4 }, IndexList{ 1, 3, 5, 2 });
    FVS visited1(7, false);
    FVS visited2(7, false);
    FVS visited3(7, false);
    FVS visited4({ false, false, false, true, false, false, false });
    FVS visited5({ true, false, false, false, false, false, false });

    // ASSERT_EQ(dfs(graph, 6, visited3), IndexList({6}));
    // ASSERT_EQ(dfs(graph, 2, visited1), IndexList({2,4}));
    // ASSERT_EQ(dfs(graph, 0, visited2), IndexList({0,1,3,5}));
    // ASSERT_EQ(dfs(graph, 0, visited4), IndexList({0,1}));
    // ASSERT_EQ(dfs(graph, 0, visited5), IndexList({}));
    auto components = connected_components(graph);
    ASSERT_EQ(
      components,
      IndexListList(
        { IndexList({ 0, 1, 3, 5 }), IndexList({ 2, 4 }), IndexList({ 6 }) }));
    ASSERT_EQ(extract_subgraph_undirected(graph, components[0]),
              UndirectedGraph(4,
                              IndexList({ 0, 1, 2 }),
                              IndexList({ 1, 2, 3 }),
                              IndexList({ 0, 1, 3, 5 })));
    ASSERT_EQ(extract_subgraph_undirected(graph, components[1]),
              UndirectedGraph(
                2, IndexList({ 1 }), IndexList({ 0 }), IndexList({ 2, 4 })));
    ASSERT_EQ(
      extract_subgraph_undirected(graph, components[2]),
      UndirectedGraph(1, IndexList({}), IndexList({}), IndexList({ 6 })));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}