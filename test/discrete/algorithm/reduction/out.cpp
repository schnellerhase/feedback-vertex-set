#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction/out.hpp"

using namespace fvs;

TEST(reduce_OUT, double_edge)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    FVS fvs(2, false);
    ASSERT_TRUE(reduce_OUT(graph, static_cast<index_t>(0), fvs));
    ASSERT_EQ(fvs, FVS({ false, true }));
    ASSERT_EQ(graph, SubGraph(0, IndexList(), IndexList(), IndexList()));
}

TEST(reduce_OUT, reduce_out_0)
{
    SubGraph graph(5,
                   IndexList{ 3, 3, 3, 4 },
                   IndexList{ 0, 1, 2, 3 },
                   IndexList{ 0, 1, 2, 3, 4 });
    FVS fvs(5, false);
    ASSERT_FALSE(reduce_OUT(graph, static_cast<index_t>(4), fvs));
    ASSERT_EQ(graph,
              SubGraph(4,
                       IndexList({ 3, 3, 3 }),
                       IndexList({ 0, 1, 2 }),
                       IndexList({ 0, 1, 2, 3 })));
}

TEST(reduce_OUT, reduce_out_1)
{
    SubGraph graph(7,
                   IndexList{ 3, 3, 3, 4, 5, 6 },
                   IndexList{ 0, 1, 2, 3, 4, 4 },
                   IndexList{ 0, 1, 2, 3, 4, 5, 6 });
    FVS fvs(7, false);
    ASSERT_FALSE(reduce_OUT(graph, static_cast<index_t>(4), fvs));
    ASSERT_EQ(graph,
              SubGraph(6,
                       IndexList({ 3, 3, 3, 4, 5 }),
                       IndexList({ 0, 1, 2, 3, 3 }),
                       IndexList({ 0, 1, 2, 3, 5, 6 })));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
