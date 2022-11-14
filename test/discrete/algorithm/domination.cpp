#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction_vc/domination.hpp"

using namespace fvs;

TEST(domination, two_nodes)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    VC vc(graph.N(), false);
    ASSERT_TRUE(domination_check(graph, 0, graph.adj()[0], 1));
    ASSERT_TRUE(domination_check(graph, 1, graph.adj()[1], 0));
    reduce_domination(graph, 0, vc);
    ASSERT_EQ(graph,
              UndirectedGraph(1, IndexList(), IndexList(), IndexList({ 1 })));
    ASSERT_EQ(vc, VC({ true, false }));
}

// TEST(reduce_domination, three_nodes)
// {
//     UndirectedGraph graph(3, IndexList{0, 1, 2}, IndexList{1, 2, 0});
//     VC vc(graph.N(), false);
//     reduce_SIMPLICIAL(graph, 0, IndexList({0, 1, 2}), vc);
//     ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList()));
//     ASSERT_EQ(vc, VC({false, true, true}));
// }

TEST(reduce_domination, three_nodes_attached)
{
    UndirectedGraph graph(4, IndexList{ 0, 0, 1, 2 }, IndexList{ 1, 2, 2, 3 });
    VC vc(graph.N(), false);
    ASSERT_TRUE(domination_check(graph, 0, graph.adj()[0], 1));
    ASSERT_FALSE(domination_check(graph, 0, graph.adj()[0], 2));
    ASSERT_TRUE(domination_check(graph, 0, graph.adj()[0], 3));
    ASSERT_TRUE(domination_check(graph, 1, graph.adj()[1], 0));
    ASSERT_FALSE(domination_check(graph, 1, graph.adj()[1], 2));
    ASSERT_TRUE(domination_check(graph, 1, graph.adj()[1], 3));
    ASSERT_TRUE(domination_check(graph, 2, graph.adj()[2], 0));
    ASSERT_TRUE(domination_check(graph, 2, graph.adj()[2], 1));
    ASSERT_TRUE(domination_check(graph, 2, graph.adj()[2], 3));
    ASSERT_FALSE(domination_check(graph, 3, graph.adj()[3], 0));
    ASSERT_FALSE(domination_check(graph, 3, graph.adj()[3], 1));
    ASSERT_FALSE(domination_check(graph, 3, graph.adj()[3], 2));
    reduce_domination(graph, 0, vc);
    ASSERT_EQ(
      graph,
      UndirectedGraph(
        3, IndexList({ 0, 1 }), IndexList({ 1, 2 }), IndexList({ 1, 2, 3 })));
    ASSERT_EQ(vc, VC({ true, false, false, false }));
}
