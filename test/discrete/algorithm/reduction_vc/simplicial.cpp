#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction_vc/simplicial.hpp"

using namespace fvs;

// TODO: this triggers assert, but is this not correct?!?
// TEST(reduce_SIMPLICIAL, empty)
// {
//     UndirectedGraph graph(0, IndexList(), IndexList());
//     VC vc(graph.N(), false);
//     reduce_SIMPLICIAL(graph, 0, IndexList(), vc);
//     ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList()));
// }

TEST(reduce_SIMPLICIAL, single_node)
{
    UndirectedGraph graph(1, IndexList(), IndexList());
    VC vc(graph.N(), false);
    reduce_SIMPLICIAL(graph, 0, IndexList({ 0 }), vc);
    ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList()));
}

// TODO: add tests for neigh
TEST(reduce_SIMPLICIAL, two_nodes)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    VC vc(graph.N(), false);
    reduce_SIMPLICIAL(graph, 0, IndexList({ 0, 1 }), vc);
    ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList()));
    ASSERT_EQ(vc, VC({ false, true }));
}

TEST(reduce_SIMPLICIAL, three_nodes)
{
    UndirectedGraph graph(3, IndexList{ 0, 1, 2 }, IndexList{ 1, 2, 0 });
    VC vc(graph.N(), false);
    reduce_SIMPLICIAL(graph, 0, IndexList({ 0, 1, 2 }), vc);
    ASSERT_EQ(graph, UndirectedGraph(0, IndexList(), IndexList()));
    ASSERT_EQ(vc, VC({ false, true, true }));
}

TEST(reduce_SIMPLICIAL, three_nodes_attached)
{
    UndirectedGraph graph(4, IndexList{ 0, 1, 2, 2 }, IndexList{ 1, 2, 0, 3 });
    VC vc(graph.N(), false);
    reduce_SIMPLICIAL(graph, 0, IndexList({ 0, 1, 2 }), vc);
    ASSERT_EQ(graph,
              UndirectedGraph(1, IndexList(), IndexList(), IndexList({ 3 })));
    ASSERT_EQ(vc, VC({ false, true, true, false }));
}
