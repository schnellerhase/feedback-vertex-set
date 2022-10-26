#include <gtest/gtest.h>

#include "discrete/algorithm/reduction/pie.hpp"

TEST(pie, empty)
{
    SubGraph graph(0, IndexList(), IndexList(), IndexList());
    reduce_PIE(graph);
    ASSERT_EQ(graph, SubGraph(0, IndexList(), IndexList()));
}

TEST(pie, single_node)
{
    SubGraph graph(1, IndexList(), IndexList(), IndexList{ 0 });
    reduce_PIE(graph);
    ASSERT_EQ(graph, SubGraph(1, IndexList(), IndexList()));
}

TEST(pie, simple)
{
    SubGraph graph(4, IndexList{ 0, 1, 2, 3, 3 }, IndexList{ 1, 2, 3, 0, 2 });
    reduce_PIE(graph);
    ASSERT_EQ(graph,
              SubGraph(4,
                       IndexList({ 2, 3 }),
                       IndexList({ 3, 2 }),
                       IndexList({ 0, 1, 2, 3 })));
}

TEST(pie, simple_dense)
{
    auto tails = IndexList{ 0, 1, 1, 2, 2, 3, 3 };
    auto heads = IndexList{ 1, 0, 2, 1, 3, 0, 2 };
    SubGraph graph(4, tails, heads);
    reduce_PIE(graph);
    ASSERT_EQ(graph,
              SubGraph(4,
                       IndexList({ 0, 1, 1, 2, 2, 3 }),
                       IndexList({ 1, 0, 2, 1, 3, 2 }),
                       IndexList({ 0, 1, 2, 3 })));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
