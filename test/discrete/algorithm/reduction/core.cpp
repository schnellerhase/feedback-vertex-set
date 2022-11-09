#include <gtest/gtest.h>

#include "discrete/algorithm/neighborhood.hpp"
#include "discrete/algorithm/reduction/core.hpp"

using namespace fvs;

// TEST(reduction_CORE, empty)
// {
//     SubGraph g(0, IndexList{}, IndexList{}, IndexList{});
//     FVS fvs;
//     reduce_CORE(g, 0, fvs);
//     ASSERT_EQ(g, SubGraph(0, IndexList(), IndexList(), IndexList()));
// }

TEST(reduction_CORE, single_node)
{
    SubGraph g(1, IndexList{}, IndexList{}, IndexList{ 0 });
    IndexList nhood = neighborhood(g, 0);
    ASSERT_EQ(nhood, IndexList({ 0 }));
    ASSERT_TRUE(neighborhood_is_clique(g, 0));

    FVS fvs(1, false);
    reduce_CORE(g, 0, nhood, fvs);
    ASSERT_EQ(g, SubGraph(0, IndexList({}), IndexList({}), IndexList()));
}

TEST(reduction_CORE, two_nodes)
{
    SubGraph g(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    IndexList nhood = neighborhood(g, 0);
    ASSERT_EQ(nhood, IndexList({ 0, 1 }));
    ASSERT_TRUE(neighborhood_is_clique(g, 0));

    FVS fvs(2, false);
    reduce_CORE(g, 0, nhood, fvs);
    ASSERT_EQ(g, SubGraph(0, IndexList(), IndexList(), IndexList()));
}

TEST(reduction_CORE, three_nodes)
{
    SubGraph g(3,
               IndexList{ 0, 0, 1, 1, 2, 2 },
               IndexList{ 1, 2, 0, 2, 0, 1 },
               IndexList{ 0, 1, 2 });
    IndexList nhood = neighborhood(g, 0);
    ASSERT_EQ(nhood, IndexList({ 0, 1, 2 }));
    ASSERT_TRUE(neighborhood_is_clique(g, 0));

    FVS fvs(3, false);
    reduce_CORE(g, 0, nhood, fvs);
    ASSERT_EQ(fvs, FVS({ false, true, true }));
    ASSERT_EQ(g, SubGraph(0, IndexList(), IndexList(), IndexList()));
}
