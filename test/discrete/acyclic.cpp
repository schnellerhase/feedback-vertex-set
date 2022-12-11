#include <gtest/gtest.h>

#include "fvs/discrete/discrete.hpp" // TODO only import specifc file

TEST(is_acyclic, empty)
{
    Graph graph(0, IndexList{}, IndexList{});
    ASSERT_TRUE(is_acyclic(graph));
    ASSERT_TRUE(is_acyclic(graph, FVS({})));
}

TEST(is_acyclic, single_node)
{
    Graph graph(1, IndexList{}, IndexList{});
    ASSERT_TRUE(is_acyclic(graph));
    ASSERT_TRUE(is_acyclic(graph, FVS({ false })));
}

// TEST(is_acyclic, self_loop) // BY ASSUMPTION DOES NOT HAPPEN -> if it does
// needs fix!
// {
//     Graph graph(1, IndexList{0}, IndexList{0});
//     ASSERT_TRUE(is_acyclic(graph));
//     ASSERT_TRUE(is_acyclic(graph, FVS({false})));
// }

TEST(is_acyclic, simple)
{
    Graph graph(2, IndexList{ 0 }, IndexList{ 1 });
    ASSERT_TRUE(is_acyclic(graph));
    ASSERT_TRUE(is_acyclic(graph, FVS({ false, false })));
}