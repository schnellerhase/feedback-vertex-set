#include <gtest/gtest.h>

#include "discrete/algorithm/connected.hpp"

TEST(is_connected, empty)
{
    UndirectedGraph graph(0, IndexList(), IndexList(), IndexList());
    ASSERT_TRUE(is_connected(graph));
}

TEST(is_connected, single)
{
    UndirectedGraph graph(1, IndexList(), IndexList(), IndexList{ 0 });
    ASSERT_TRUE(is_connected(graph));
}

TEST(is_connected, two_nodes_0)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    ASSERT_TRUE(is_connected(graph));
}

TEST(is_connected, two_nodes_1)
{
    UndirectedGraph graph(2, IndexList(), IndexList());
    ASSERT_FALSE(is_connected(graph));
}

TEST(is_connected, three_components)
{
    UndirectedGraph graph(5, IndexList{ 0, 2 }, IndexList{ 1, 3 });
    ASSERT_FALSE(is_connected(graph));
}

TEST(is_strongly_connected, empty)
{
    SubGraph graph(0, IndexList(), IndexList(), IndexList());
    ASSERT_TRUE(is_strongly_connected(graph));
}

TEST(is_strongly_connected, single)
{
    SubGraph graph(1, IndexList(), IndexList(), IndexList{ 0 });
    ASSERT_TRUE(is_strongly_connected(graph));
}

TEST(is_strongly_connected, two_nodes_0)
{
    SubGraph graph(2, IndexList{ 0 }, IndexList{ 1 }, IndexList{ 0, 1 });
    ASSERT_FALSE(is_strongly_connected(graph));
}

TEST(is_strongly_connected, two_nodes_1)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    ASSERT_TRUE(is_strongly_connected(graph));
}

TEST(is_strongly_connected, four_node_circle)
{
    SubGraph graph(4,
                   IndexList{ 0, 1, 2, 3 },
                   IndexList{ 1, 2, 3, 0 },
                   IndexList{ 0, 1, 2, 3 });
    ASSERT_TRUE(is_strongly_connected(graph));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
