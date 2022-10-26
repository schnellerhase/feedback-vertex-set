#include <gtest/gtest.h>

#include "discrete/algorithm/dfs.hpp"

// TEST(DFS, empty)
// {
//     SubGraph graph(0, IndexList(), IndexList());
//     DFS dfs(graph);
//     ASSERT_FALSE(dfs.next());
// }
// TODO: more test cases!
TEST(DFS, two_nodes)
{
    SubGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    DFS<SubGraph, DFS_forward> dfs(graph);
    ASSERT_TRUE(dfs.next());
    ASSERT_EQ(*dfs, 0);
    ASSERT_TRUE(dfs.next());
    ASSERT_EQ(*dfs, 1);
    ASSERT_FALSE(dfs.next());
    ASSERT_FALSE(dfs.next());
}

TEST(DFS, two_nodes_undirected)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    DFS<UndirectedGraph, DFS_forward> dfs(graph);
    ASSERT_TRUE(dfs.next());
    ASSERT_EQ(*dfs, 0);
    ASSERT_TRUE(dfs.next());
    ASSERT_EQ(*dfs, 1);
    ASSERT_FALSE(dfs.next());
    ASSERT_FALSE(dfs.next());
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
