#include <gtest/gtest.h>

#include "discrete/discrete.hpp"

// TODO add test cases for append

// TEST(FVS, to_index_list_0)
// {
//     FVS fvs{true, true, false, false, false};
//     ASSERT_EQ(to_index_list(fvs, true), IndexList({0, 1}));
//     ASSERT_EQ(to_index_list(fvs, false), IndexList({2, 3, 4}));
// }

// TEST(FVS, to_index_list_1)
// {
//     FVS fvs{true, false, true, false};
//     ASSERT_EQ(to_index_list(fvs, true), IndexList({0, 2}));
//     ASSERT_EQ(to_index_list(fvs, false), IndexList({1, 3}));
// }

// TEST(FVS, edge_count_0)
// {
//     SubGraph graph(4, IndexList{0, 1, 1}, IndexList{1, 2, 3}, IndexList{0, 1,
//     2, 3}); FVS fvs = neighborhood(graph, 1); ASSERT_EQ(edge_count(graph,
//     fvs), 0); ASSERT_EQ(edge_count(graph, FVS(4, false)), 3);
// }

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}