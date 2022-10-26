#include <gtest/gtest.h>

// #include "discrete/discrete.hpp"

// TEST(Graph, pie_simple_basic_operations)
// {
//     auto tails = IndexList{0, 1, 2, 3, 3};
//     auto heads = IndexList{1, 2, 3, 0, 2};
//     Graph graph(4, tails, heads);
//     SubGraph subgraph(graph);

//     IndexList twoCycleEdgesShould{2,4};
//     IndexList twoCycleEdges = find_all_2_cycle_edges(subgraph);

//     index_t edgeItterator = twoCycleEdges.size();

//     while(edgeItterator > 0)
//     {
//         edgeItterator--;
//         remove_edge(subgraph, twoCycleEdges[edgeItterator]);
//     }

//     IndexList components = scc_pie_version(subgraph);
//     IndexList componentsShould({2,1,0,3});

//     std::vector<std::tuple<index_t, index_t>> tailsHeadsToRemove =
//     pie_reduction(subgraph); std::vector<std::tuple<index_t, index_t>>
//     tailsHeadsToRemoveShould;

//     tailsHeadsToRemoveShould.push_back(std::tuple<index_t, index_t>{0,1});
//     tailsHeadsToRemoveShould.push_back(std::tuple<index_t, index_t>{1,2});
//     tailsHeadsToRemoveShould.push_back(std::tuple<index_t, index_t>{3,0});

//     ASSERT_EQ(twoCycleEdges, twoCycleEdgesShould);
//     ASSERT_EQ(subgraph.M(), 3);
//     ASSERT_EQ(components, componentsShould);
//     ASSERT_EQ(tailsHeadsToRemove, tailsHeadsToRemoveShould);
//     // SubGraph subgraph(graph);
//     // double_reduction(subgraph, index_t(1), index_t(2));

//     // ASSERT_EQ(components.size(), 7);
//     // ASSERT_EQ(components, components_should);

//     // ASSERT_EQ(subgraph.N(), 7);
//     // ASSERT_EQ(subgraph.M(), 4);
//     // ASSERT_EQ(subgraph.indeg(), indegShould);
//     // ASSERT_EQ(subgraph.outdeg(), outdegShould);
//     // ASSERT_EQ(subgraph.heads(), headsShould);
//     // ASSERT_EQ(subgraph.tails(), tailsShould);
// }

// // TEST(Graph, fully_reduce_scc_complex_4)
// // {
// //     auto tails = IndexList{0, 1, 1, 1, 2, 2, 3, 4, 4};
// //     auto heads = IndexList{1, 0, 2, 3, 1, 3, 5, 1, 2};
// //     Graph graph(7, tails, heads);
// //     SubGraph subgraph(graph);

// //     auto indegShould = IndexList({1,2,1,0,0,0,0});
// //     auto outdegShould = IndexList({1,2,1,0,0,0,0});
// //     IndexList tailsShould = IndexList{0,1,1,2};
// //     IndexList headsShould = IndexList{1,0,2,1};

// //     fully_double_reduction(subgraph);

// //     ASSERT_EQ(subgraph.N(), 7);
// //     ASSERT_EQ(subgraph.M(), 4);
// //     ASSERT_EQ(subgraph.indeg(), indegShould);
// //     ASSERT_EQ(subgraph.outdeg(), outdegShould);
// //     ASSERT_EQ(subgraph.heads(), headsShould);
// //     ASSERT_EQ(subgraph.tails(), tailsShould);
// // }

// // TEST(Graph, fully_reduce_simple)
// // {
// //     auto tails = IndexList{0, 1, 2, 3, 3};
// //     auto heads = IndexList{1, 2, 3, 0, 2};
// //     Graph graph(4, tails, heads);
// //     SubGraph subgraph(graph);

// //     IndexList tailsShould = IndexList{2,3};
// //     IndexList headsShould = IndexList{3,2};

// //     fully_double_reduction(subgraph);
// //     auto nTwoCycles = find_all_2_cycles(subgraph).size();

// //     ASSERT_EQ(nTwoCycles, 1);
// //     ASSERT_EQ(subgraph.N(), 4);
// //     ASSERT_EQ(subgraph.M(), 2);
// //     ASSERT_EQ(subgraph.heads(), headsShould);
// //     ASSERT_EQ(subgraph.tails(), tailsShould);
// // }

// // TEST(Graph, fully_reduce_complex)
// // {
// //     auto tails = IndexList{0, 1, 2, 2, 0, 3, 3, 4, 5, 5, 6};
// //     auto heads = IndexList{1, 2, 0, 1, 3, 4, 6, 5, 0, 4, 0};
// //     Graph graph(7, tails, heads);
// //     SubGraph subgraph(graph);

// //     IndexList tailsShould = IndexList{1, 2, 0, 3, 4, 5, 6};
// //     IndexList headsShould = IndexList{2, 1, 3, 6, 5, 4, 0};

// //     fully_double_reduction(subgraph);
// //     auto nTwoCycles = find_all_2_cycles(subgraph).size();

// //     ASSERT_EQ(nTwoCycles, 2);
// //     ASSERT_EQ(subgraph.N(), 7);
// //     ASSERT_EQ(subgraph.M(), 7);
// //     ASSERT_EQ(subgraph.heads(), headsShould);
// //     ASSERT_EQ(subgraph.tails(), tailsShould);
// // }

// // TEST(Graph, pie_fully_reduce_simple)
// // {
// //     auto tails = IndexList{0, 1, 2, 3, 3};
// //     auto heads = IndexList{1, 2, 3, 0, 2};
// //     Graph graph(4, tails, heads);
// //     SubGraph subgraph(graph);

// //     IndexList tailsShould = IndexList{2,3};
// //     IndexList headsShould = IndexList{3,2};

// //     fully_double_reduction(subgraph);
// //     auto nTwoCycles = find_all_2_cycles(subgraph).size();

// //     ASSERT_EQ(nTwoCycles, 1);
// //     ASSERT_EQ(subgraph.N(), 4);
// //     ASSERT_EQ(subgraph.M(), 2);
// //     ASSERT_EQ(subgraph.heads(), headsShould);
// //     ASSERT_EQ(subgraph.tails(), tailsShould);
// // }

// // TEST(Graph, pie_fully_reduce_complex)
// // {
// //     auto tails = IndexList{0, 1, 2, 2, 0, 3, 3, 4, 5, 5, 6};
// //     auto heads = IndexList{1, 2, 0, 1, 3, 4, 6, 5, 0, 4, 0};
// //     Graph graph(7, tails, heads);
// //     SubGraph subgraph(graph);

// //     IndexList tailsShould = IndexList{1, 2, 0, 3, 4, 5, 6};
// //     IndexList headsShould = IndexList{2, 1, 3, 6, 5, 4, 0};

// //     fully_double_reduction(subgraph);
// //     auto nTwoCycles = find_all_2_cycles(subgraph).size();

// //     ASSERT_EQ(nTwoCycles, 2);
// //     ASSERT_EQ(subgraph.N(), 7);
// //     ASSERT_EQ(subgraph.M(), 7);
// //     ASSERT_EQ(subgraph.heads(), headsShould);
// //     ASSERT_EQ(subgraph.tails(), tailsShould);
// // }

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
