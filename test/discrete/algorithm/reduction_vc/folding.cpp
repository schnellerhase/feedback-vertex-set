#include <gtest/gtest.h>

#include "discrete/algorithm/reduction_vc/folding.hpp"

TEST(reduce_folding, three_nodes)
{
    UndirectedGraph graph(3, IndexList{ 0, 1 }, IndexList{ 1, 2 });
    reduce_folding(graph, 1, graph.adj()[1]);
    ASSERT_EQ(graph,
              UndirectedGraph(1,
                              IndexList(),
                              IndexList(),
                              IndexListList({ IndexList({ 0, 2 }) }),
                              IndexListList({ IndexList({ 1 }) })));
}

TEST(reduce_folding, two_level)
{
    UndirectedGraph graph(9,
                          IndexList{ 0, 1, 2, 3, 4, 5, 6, 6 },
                          IndexList{ 2, 2, 3, 4, 5, 6, 7, 8 });
    reduce_folding(graph, 4, graph.adj()[4]);
    ASSERT_EQ(graph,
              UndirectedGraph(7,
                              IndexList({ 0, 1, 2, 3, 4, 4 }),
                              IndexList({ 2, 2, 3, 4, 5, 6 }),
                              IndexListList({ IndexList({ 0 }),
                                              IndexList({ 1 }),
                                              IndexList({ 2 }),
                                              IndexList({ 3, 5 }),
                                              IndexList({ 6 }),
                                              IndexList({ 7 }),
                                              IndexList({ 8 }) }),
                              IndexListList({ IndexList(),
                                              IndexList(),
                                              IndexList(),
                                              IndexList({ 4 }),
                                              IndexList(),
                                              IndexList(),
                                              IndexList() })));
    reduce_folding(graph, 3, graph.adj()[3]);
    ASSERT_EQ(graph,
              UndirectedGraph(5,
                              IndexList({ 0, 1, 2, 2 }),
                              IndexList({ 2, 2, 3, 4 }),
                              IndexListList({ IndexList({ 0 }),
                                              IndexList({ 1 }),
                                              IndexList({ 4, 2, 6 }),
                                              IndexList({ 7 }),
                                              IndexList({ 8 }) }),
                              IndexListList({ IndexList(),
                                              IndexList(),
                                              IndexList({ 3, 5 }),
                                              IndexList(),
                                              IndexList() })));
}

// TEST(reduce_SIMPLICIAL, three_nodes_attached)
// {
//     UndirectedGraph graph(4, IndexList{0, 1, 2, 2}, IndexList{1, 2, 0, 3});
//     VC vc(graph.N(), false);
//     reduce_SIMPLICIAL(graph, 0, IndexList({0, 1, 2}), vc);
//     ASSERT_EQ(graph, UndirectedGraph(1, IndexList(), IndexList(),
//     IndexList({3}))); ASSERT_EQ(vc, VC({false, true, true, false}));
//}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
