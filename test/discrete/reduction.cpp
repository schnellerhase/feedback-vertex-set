#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction.hpp"
#include "fvs/discrete/algorithm/reduction/in.hpp"
#include "fvs/discrete/algorithm/reduction/out.hpp"
#include "fvs/discrete/util/types.hpp"

// TEST(Reduction, empty)
// {
//     SubGraph graph(0, IndexList{}, IndexList{}, IndexList{});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(0, IndexList({}), IndexList({}),
//     IndexList({})));
// }

// TEST(Reduction, trivial_1_node)
// {
//     SubGraph graph(1, IndexList({}), IndexList({}), IndexList{0});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(0, IndexList({}), IndexList({}),
//     IndexList({})));
// }

// TEST(Reduction, trivial_with_circle)
// {
//     SubGraph graph(3, IndexList({1, 2}), IndexList({2, 1}),
//     IndexList{0,1,2}); ASSERT_FALSE(reduce_graph(graph)); ASSERT_EQ(graph,
//     SubGraph(2, IndexList({0, 1}), IndexList({1, 0}), IndexList({1, 2})));
// }

// TEST(Reduction, trivial_loop)
// {
//     SubGraph graph(1, IndexList({0}), IndexList({0}), IndexList{0});
//     auto fvs_true_idx = reduce_graph(graph);
//     ASSERT_EQ(*fvs_true_idx, 0);
//     ASSERT_EQ(graph, SubGraph(0, IndexList({}), IndexList({}),
//     IndexList({})));
// }

// TEST(Redcution, loop_simple)
// {
//     SubGraph graph(2, IndexList({0, 1, 0}), IndexList({1, 0, 0}),
//     IndexList{0, 1}); auto fvs_true_idx = reduce_graph(graph);
//     ASSERT_EQ(*fvs_true_idx, 0);
//     ASSERT_EQ(graph, SubGraph(1, IndexList({}), IndexList({}),
//     IndexList({1})));
// }

// TEST(Redcution, loop_further)
// {
//     SubGraph graph(3, IndexList({1, 1, 2, 1}), IndexList({0, 2, 0, 1}),
//     IndexList{0, 1, 2}); auto fvs_true_idx = reduce_graph(graph);
//     ASSERT_EQ(*fvs_true_idx, 1);
//     ASSERT_EQ(graph, SubGraph(2, IndexList({1}), IndexList({0}),
//     IndexList({0, 2})));
// }

// TEST(Reduction, trivial_loop_with_circle)
// {
//     SubGraph graph(3, IndexList({0, 1, 2}), IndexList({0, 2, 1}),
//     IndexList{0,1,2}); auto fvs_true_idx = reduce_graph(graph);
//     ASSERT_EQ(*fvs_true_idx, 0);
//     ASSERT_EQ(graph, SubGraph(2, IndexList({0, 1}), IndexList({1, 0}),
//     IndexList({1, 2})));
// }

// TEST(Reduction, two_node_circle)
// {
//     SubGraph graph(2, IndexList{0, 1}, IndexList{1, 0}, IndexList{0,1});
//     ASSERT_FALSE(reduce_graph(graph));
//     auto expected= SubGraph(1, IndexList({0}), IndexList({0}),
//     IndexList({1})); ASSERT_EQ(graph, expected);
// }

// TEST(Reduction, three_node_circle)
// {
//     SubGraph graph(3, IndexList{0, 1, 2}, IndexList{1, 2, 0},
//     IndexList{0,1,2}); ASSERT_FALSE(reduce_graph(graph)); ASSERT_EQ(graph,
//     SubGraph(2, IndexList({1, 0}), IndexList({0, 1}), IndexList({1, 2})));
// }

// TEST(Reduction, trivial_in_2_nodes)
// {
//     // • 🠒 •
//     SubGraph graph(2, IndexList{0}, IndexList{1}, IndexList{0,1});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(1, IndexList({}), IndexList({}),
//     IndexList({1})));
// }

// TEST(Reduction, trivial_out_2_nodes)
// {
//     // • 🠐 •
//     SubGraph graph(2, IndexList{1}, IndexList{0}, IndexList{0,1});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(1, IndexList({}), IndexList({}),
//     IndexList({1})));
// }

// TEST(Reduction, trivial_in_3_nodes)
// {
//     // • 🠒 • 🠒 •
//     SubGraph graph(3, IndexList{0, 1}, IndexList{1, 2}, IndexList{0,1,2});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(2, IndexList({0}), IndexList({1}),
//     IndexList({1, 2})));
// }

// TEST(Reduction, trivial_in_3_nodes_reversed)
// {
//     // • 🠐 • 🠐 •
//     SubGraph graph(3, IndexList{2, 1}, IndexList{1, 0}, IndexList{0,1,2});
//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(2, IndexList({1}), IndexList({0}),
//     IndexList({1, 2})));
// }

// TEST(Reduction, simple_in_4_nodes)
// {
//     SubGraph graph(4, IndexList{0, 1, 1}, IndexList{1, 2, 3},
//     IndexList{0,1,2,3}); ASSERT_FALSE(reduce_graph(graph)); ASSERT_EQ(graph,
//     SubGraph(3, IndexList({0, 0}), IndexList({1, 2}), IndexList({1, 2, 3})));
// }

// TEST(Reduction, double_edge)
// {
//     SubGraph graph(2, IndexList{0, 0, 0}, IndexList{1, 1, 1}, IndexList{0,
//     1}); ASSERT_FALSE(reduce_graph(graph)); ASSERT_EQ(graph, SubGraph(2,
//     IndexList({0, 0}), IndexList({1, 1}), IndexList({0, 1})));
// }

// TEST(Reduction, reduce_in_case)
// {
//     SubGraph graph(5, IndexList{0, 1, 0, 1, 2, 0,4, 1, 4}, IndexList{2, 2, 1,
//     0, 3, 4, 0, 4, 1}, IndexList{0, 1, 2, 3, 4}); reduce_graph(graph);
//     ASSERT_EQ(graph, SubGraph(4, IndexList({0, 1, 0, 1, 0, 3, 1, 3}),
//     IndexList({2, 2, 1, 0, 3, 0, 3, 1}), IndexList({0, 1, 3, 4})));
// }

// TEST(Reduction, simple_in_4_nodes_reversed)
// {
//     SubGraph graph(4, IndexList{2, 2, 3, 0, 1, 0, 1}, IndexList{0, 1, 2, 1,
//     0, 3, 3}, IndexList{0,1,2,3}); auto fvs_true_idx = reduce_graph(graph);
//     ASSERT_EQ(graph, SubGraph(3, IndexList({0, 0}), IndexList({1, 2}),
//     IndexList({1, 2, 3}))); ASSERT_EQ(fvs_true_idx, graph.N());
// }

TEST(FullReduction, empty)
{
    SubGraph empty(0, IndexList({}), IndexList({}));
    const auto [reduced, fvs, undirected_components] =
      fully_reduce_graph(empty);
    ASSERT_EQ(reduced.size(), 0);
    ASSERT_EQ(fvs, FVS({}));
    ASSERT_TRUE(Graph::has_valid_data_structure(empty));
}

TEST(FullReduction, single_node)
{
    SubGraph graph(1, IndexList({}), IndexList({}));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS(1, false));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(FullReduction, two_nodes)
{
    SubGraph graph(2, IndexList({ 0 }), IndexList({ 1 }));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS(2, false));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(FullReduction, two_nodes_circle)
{
    SubGraph graph(2, IndexList({ 0, 1 }), IndexList({ 1, 0 }));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS({ true, false }));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(FullReduction, three_nodes)
{
    SubGraph graph(3, IndexList({ 0, 1 }), IndexList({ 1, 2 }));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS(3, false));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(FullReduction, three_node_circle)
{
    SubGraph graph(3, IndexList({ 0, 1, 2 }), IndexList({ 1, 2, 0 }));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS({ true, false, false }));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

TEST(FullReduction, three_nodes_strongly_connected)
{
    SubGraph graph(
      3, IndexList({ 0, 1, 1, 2, 0, 2 }), IndexList({ 1, 0, 2, 1, 2, 0 }));
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(undirected_components.size(), 1);
    // ASSERT_EQ(fvs, FVS({true, true, false}));
    ASSERT_EQ(
      undirected_components.front(),
      UndirectedGraph(3, IndexList({ 0, 1, 0 }), IndexList({ 1, 2, 2 })));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
}

// TEST(FullRedcution, simple_step_by_step)
// {
//     SubGraph graph(4, List<index_t, index_t>({0, 0, 1, 2, 3}), List<index_t,
//     index_t>({1, 2, 2, 3, 0}), IndexList{0,1,2,3});

//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(3, IndexList({2, 2, 0, 1}), IndexList({0, 1, 1,
//     2}), IndexList({1, 2, 3})));

//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(2, IndexList({1, 1, 0}), IndexList({0, 0, 1}),
//     IndexList({2, 3})));

//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(2, IndexList({1, 0}), IndexList({0, 1}),
//     IndexList({2, 3})));

//     ASSERT_FALSE(reduce_graph(graph));
//     ASSERT_EQ(graph, SubGraph(1, IndexList({0}), IndexList({0}),
//     IndexList({3})));

//     auto i = reduce_graph(graph);
//     ASSERT_EQ(*i, 3);
//     ASSERT_EQ(graph, SubGraph(0, IndexList({}), IndexList({}),
//     IndexList({})));
// }

TEST(FullReduction, simple)
{
    SubGraph graph(4, IndexList{ 0, 0, 1, 2, 3 }, IndexList{ 1, 2, 2, 3, 0 });
    const auto [subgraphs, fvs, undirected_components] =
      fully_reduce_graph(graph);
    ASSERT_EQ(subgraphs.size(), 0);
    ASSERT_EQ(fvs, FVS({ true, false, false, false }));
}

TEST(Reduction, e_001)
{
    SubGraph graph(Graph::read(std::string("data/graph_e_001")));
    const auto [reduced, fvs, undirected_components] =
      fully_reduce_graph(graph);
}

TEST(Redcution, failing)
{
    SubGraph graph(Graph::read("data/e_005_scc_1"));
    const auto [reduced, fvs, undirected_components] =
      fully_reduce_graph(graph);
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
