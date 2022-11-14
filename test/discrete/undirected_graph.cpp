#include <gtest/gtest.h>

#include "fvs/discrete/undirected_graph.hpp"

using namespace fvs;

TEST(UndirectedGraph, empty)
{
    UndirectedGraph graph(0, IndexList(), IndexList());
    ASSERT_EQ(graph.N(), 0);
    ASSERT_EQ(graph.M(), 0);
    ASSERT_EQ(graph.tails(), IndexList());
    ASSERT_EQ(graph.heads(), IndexList());
    ASSERT_EQ(graph.deg(), IndexList());
    ASSERT_EQ(graph.adj(), IndexListList());
    ASSERT_EQ(graph.to_arc(), IndexListList());
    ASSERT_EQ(graph.local2global(), IndexListList());
    ASSERT_EQ(graph.local2globalInvert(), IndexListList());
}

TEST(UndirectedGraph, single)
{
    UndirectedGraph graph(1, IndexList(), IndexList());
    ASSERT_EQ(graph.N(), 1);
    ASSERT_EQ(graph.M(), 0);
    ASSERT_EQ(graph.tails(), IndexList());
    ASSERT_EQ(graph.heads(), IndexList());
    ASSERT_EQ(graph.deg(), IndexList({ 0 }));
    ASSERT_EQ(graph.adj(), IndexListList({ IndexList() }));
    ASSERT_EQ(graph.to_arc(), IndexListList({ IndexList() }));
    ASSERT_EQ(graph.local2global(), IndexListList({ IndexList({ 0 }) }));
    ASSERT_EQ(graph.local2globalInvert(), IndexListList(1));
}

TEST(UndirectedGraph, two_nodes)
{
    UndirectedGraph graph(2, IndexList({ 0 }), IndexList({ 1 }));
    ASSERT_EQ(graph.N(), 2);
    ASSERT_EQ(graph.M(), 1);
    ASSERT_EQ(graph.tails(), IndexList({ 0 }));
    ASSERT_EQ(graph.heads(), IndexList({ 1 }));
    ASSERT_EQ(graph.deg(), IndexList({ 1, 1 }));
    ASSERT_EQ(graph.adj(),
              IndexListList({ IndexList({ 1 }), IndexList({ 0 }) }));
    ASSERT_EQ(graph.to_arc(),
              IndexListList({ IndexList({ 0 }), IndexList({ 0 }) }));
    ASSERT_EQ(graph.local2global(),
              IndexListList({ IndexList({ 0 }), IndexList({ 1 }) }));
    ASSERT_EQ(graph.local2globalInvert(), IndexListList(2));
}

TEST(UndirectedGraph, complex)
{
    UndirectedGraph graph(
      5, IndexList({ 0, 0, 0, 1, 2 }), IndexList({ 1, 3, 4, 2, 3 }));
    ASSERT_EQ(graph.N(), 5);
    ASSERT_EQ(graph.M(), 5);
    ASSERT_EQ(graph.tails(), IndexList({ 0, 0, 0, 1, 2 }));
    ASSERT_EQ(graph.heads(), IndexList({ 1, 3, 4, 2, 3 }));
    ASSERT_EQ(graph.deg(), IndexList({ 3, 2, 2, 2, 1 }));
    ASSERT_EQ(graph.adj(),
              IndexListList({ IndexList({ 1, 3, 4 }),
                              IndexList({ 0, 2 }),
                              IndexList({ 1, 3 }),
                              IndexList({ 0, 2 }),
                              IndexList({ 0 }) }));
    ASSERT_EQ(graph.to_arc(),
              IndexListList({ IndexList({ 0, 1, 2 }),
                              IndexList({ 0, 3 }),
                              IndexList({ 3, 4 }),
                              IndexList({ 1, 4 }),
                              IndexList({ 2 }) }));
    ASSERT_EQ(graph.local2global(),
              IndexListList({ IndexList({ 0 }),
                              IndexList({ 1 }),
                              IndexList({ 2 }),
                              IndexList({ 3 }),
                              IndexList({ 4 }) }));
    ASSERT_EQ(graph.local2globalInvert(), IndexListList(5));
}

// TODO FIX
// TEST(UndirectedGraph, IO)
// {
//     UndirectedGraph graph(5,
//                           IndexList({0, 0, 0, 1, 2}),
//                           IndexList({1, 3, 4, 2, 3}));
//     std::string path("undirected_out");
//     write(graph, path);
//     ASSERT_EQ(graph, read_instance(path));
// }

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
