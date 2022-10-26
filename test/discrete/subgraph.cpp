#include <gtest/gtest.h>

#include "discrete/discrete.hpp"

// TEST(SubGraph, empty)
// {
//     Graph graph(0, IndexList({}), IndexList({}));
//     SubGraph subgraph = extract_subgraph(graph, IndexList({}));
//     ASSERT_EQ(graph, subgraph);
// }

// TEST(SubGraph, simple_1)
// {
//     Graph graph(4, IndexList{0, 0, 1, 2, 3}, IndexList{1, 2, 2, 3, 0});
//     auto subgraph1 = extract_subgraph(graph, IndexList({0, 2, 3}));

//     auto tails1 = IndexList{0, 2, 3};
//     auto heads1 = IndexList{2, 3, 0};
//     auto indeg1 = IndexList({1, 1, 1});
//     auto outdeg1 = IndexList({1, 1, 1});
//     auto inadj1 = IndexListList{{3}, {0}, {2}};
//     auto outadj1 = IndexListList{{2},{3},{0}};
//     ASSERT_EQ(subgraph1.N(), 3);
//     ASSERT_EQ(subgraph1.M(), 3);
//     ASSERT_EQ(subgraph1.indeg(), indeg1);
//     ASSERT_EQ(subgraph1.outdeg(), outdeg1);
//     ASSERT_EQ(subgraph1.local2global(subgraph1.inadj()), inadj1);
//     ASSERT_EQ(subgraph1.local2global(subgraph1.outadj()), outadj1);
//     ASSERT_EQ(subgraph1.local2global(subgraph1.heads()), heads1);
//     ASSERT_EQ(subgraph1.local2global(subgraph1.tails()), tails1);

// }

// TEST(SubGraph, simple_2)
// {
//     Graph graph(4, IndexList{0, 0, 1, 2, 3}, IndexList{1, 2, 2, 3, 0});
//     auto subgraph2 = extract_subgraph(graph, IndexList({1}));

//     auto tails2 = IndexList{};
//     auto heads2 = IndexList{};
//     auto indeg2 = IndexList({0});
//     auto outdeg2 = IndexList({0});
//     ASSERT_EQ(subgraph2.N(), 1);
//     ASSERT_EQ(subgraph2.M(), 0);
//     ASSERT_EQ(subgraph2.indeg(), indeg2);
//     ASSERT_EQ(subgraph2.outdeg(), outdeg2);
//     ASSERT_EQ(subgraph2.local2global(subgraph2.inadj()), IndexListList{{}});
//     ASSERT_EQ(subgraph2.local2global(subgraph2.outadj()), IndexListList{{}});
//     ASSERT_EQ(subgraph2.local2global(subgraph2.heads()), heads2);
//     ASSERT_EQ(subgraph2.local2global(subgraph2.tails()), tails2);
// }

// TEST(SubGraph, extract_subgraph)
// {
//     SubGraph a(3, IndexList{0, 1, 2}, IndexList{1, 2, 0}, IndexList{2, 5,
//     7}); auto b = extract_subgraph(a, IndexList{1, 2}); auto expected =
//     SubGraph(2, IndexList({0}), IndexList({1}), IndexList({5, 7}));
//     ASSERT_EQ(b, expected);
// }

// TEST(SubGraph, extract_subgraph_1)
// {
//     SubGraph a(4, IndexList{0, 1, 1, 2, 3}, IndexList{1, 2, 3, 0, 2},
//     IndexList{2, 5, 7, 9}); auto b = extract_subgraph(a, IndexList{1, 2});
//     auto expected = SubGraph(2, IndexList({0}), IndexList({1}), IndexList({5,
//     7})); ASSERT_EQ(b, expected);
// }

// TEST(SubGraph, e_001)
// {
//     Graph graph = read_instance("graph_e_001");
//     IndexList nodes(graph.N(), 0);
//     for (index_t i = 0; i < nodes.size(); i ++)
//         nodes[i] = i;

//     auto sub_graph = extract_subgraph(graph, nodes);
//     ASSERT_EQ(graph, sub_graph);
//     ASSERT_TRUE(graph == sub_graph);
// }

TEST(SubGraph, scc)
{
    SubGraph subgraph(Graph(6,
                            IndexList{ 0, 1, 0, 2, 3, 2, 4, 5 },
                            IndexList{ 1, 0, 2, 3, 2, 4, 5, 4 }));

    auto sccs = strongly_connected_components(subgraph);
    ASSERT_EQ(sccs.size(), 3);
    ASSERT_EQ(sccs[0], IndexList({ 4, 5 }));
    ASSERT_EQ(sccs[1], IndexList({ 2, 3 }));
    ASSERT_EQ(sccs[2], IndexList({ 0, 1 }));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
