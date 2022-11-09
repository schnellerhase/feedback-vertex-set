#include <gtest/gtest.h>

#include "discrete/algorithm/scc.hpp"

using namespace fvs;

TEST(scc, empty)
{
    SubGraph graph(0, IndexList{}, IndexList{}, IndexList{});
    ASSERT_EQ(strongly_connected_components(graph), IndexListList{});
}

TEST(scc, single_node)
{
    SubGraph graph(1, IndexList{}, IndexList{}, IndexList{ 0 });
    ASSERT_EQ(strongly_connected_components(graph), IndexListList{ { 0 } });
}

TEST(scc, simple)
{
    SubGraph graph(4,
                   IndexList{ 0, 0, 1, 2, 3 },
                   IndexList{ 1, 2, 2, 3, 0 },
                   IndexList{ 0, 1, 2, 3 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 0, 1, 2, 3 } }));
}

TEST(Graph, scc_2)
{
    SubGraph graph(2, IndexList{}, IndexList{}, IndexList{ 0, 1 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 0 }, { 1 } }));
}

TEST(Graph, scc_2_weakly_connected)
{
    SubGraph graph(2, IndexList{ 0 }, IndexList{ 1 }, IndexList{ 0, 1 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 1 }, { 0 } }));
}

TEST(Graph, scc_2_strongly_connected)
{
    SubGraph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 }, IndexList{ 0, 1 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 0, 1 } }));
}

TEST(Graph, scc_3)
{
    SubGraph graph(3, IndexList{}, IndexList{}, IndexList{ 0, 1, 2 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 0 }, { 1 }, { 2 } }));
}

TEST(Graph, scc_complex_4)
{
    SubGraph graph(7,
                   IndexList{ 0, 1, 1, 1, 2, 2, 3, 4, 4 },
                   IndexList{ 1, 0, 2, 3, 1, 3, 5, 1, 2 },
                   IndexList{ 0, 1, 2, 3, 4, 5, 6 });
    ASSERT_EQ(strongly_connected_components(graph),
              IndexListList({ { 5 }, { 3 }, { 0, 1, 2 }, { 4 }, { 6 } }));
}
