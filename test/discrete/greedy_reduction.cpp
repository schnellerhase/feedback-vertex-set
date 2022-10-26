#include <gtest/gtest.h>
#include <stack>

#include "discrete/discrete.hpp"

TEST(greedy_reduction, empty)
{
    Graph graph(0, IndexList{}, IndexList{});
    auto fvs = greedy_reduction(graph, 1);
    ASSERT_EQ(fvs, FVS({}));
}

TEST(greedy_reduction, single_node)
{
    Graph graph(1, IndexList{}, IndexList{});
    auto fvs = greedy_reduction(graph, 1);
    ASSERT_EQ(fvs, FVS({ false }));
}

TEST(greedy_reduction, two_nodes)
{
    Graph graph(2, IndexList{ 0 }, IndexList{ 1 });
    auto fvs = greedy_reduction(graph, 1);
    ASSERT_EQ(fvs, FVS({ false, false }));
}

TEST(greedy_reduction, two_nodes_strongly)
{
    Graph graph(2, IndexList{ 0, 1 }, IndexList{ 1, 0 });
    auto fvs = greedy_reduction(graph, 1);
    ASSERT_EQ(fvs, FVS({ true, false }));
}

TEST(Graph, scc_complex_4)
{
    Graph graph(7,
                IndexList{ 0, 1, 1, 1, 2, 2, 3, 4, 4 },
                IndexList{ 1, 0, 2, 3, 1, 3, 5, 1, 2 });

    ASSERT_FALSE(Graph::is_acyclic(graph));
    auto fvs = greedy_reduction(graph, 1);
    ASSERT_TRUE(Graph::is_acyclic(graph, fvs));
    // IndexList sorted_should({1, 2, 3, 0, 4, 5, 6});
    ASSERT_EQ(fvs, FVS({ true, false, true, false, false, false, false }));
}

// TEST(Graph, tree)
// {
//     IndexList tails{3, 3, 5, 4, 4, 2, 0};
//     IndexList heads{4, 5, 2, 1, 2, 0, 8};
//     Graph graph(10, tails, heads);

//     std::vector<int> marks(10, -1);
//     index_t marked_node_counter = 0;
//     index_t cycle_check_frequency = 200;

//     bool  result = is_acyclic(  graph.outadj(), marks, marked_node_counter);
//     std::tuple<int*, int> start_solution = greedy_reduction( graph,
//     cycle_check_frequency);

//     ASSERT_EQ(std::get<1>(start_solution), 0);
//     ASSERT_EQ(result, true);
// }

/*TEST(Graph, greedy_advanced)
{
    auto tails = List<index_t , index_t>{0, 1, 2};
    auto heads = List<index_t , index_t>{1, 2, 0};
    Graph graph(3, tails, heads);
    index_t cycle_check_frequency = 1;
    List<index_t, index_t> start_solution_1 = greedy_reduction( graph,
cycle_check_frequency); cycle_check_frequency++; List<index_t, index_t>
start_solution_2 = greedy_reduction( graph, cycle_check_frequency);
    cycle_check_frequency++;
    List<index_t, index_t> start_solution_3 = greedy_reduction( graph,
cycle_check_frequency);


    List<index_t, index_t> sortet_should({0,1,2});
    List<index_t,index_t> start_solution_should_1({1,0,0});
    List<index_t,index_t> start_solution_should_2({});


    ASSERT_EQ(start_solution_1, start_solution_should_1);
    ASSERT_EQ(start_solution_2, start_solution_should_1);
    ASSERT_EQ(start_solution_3, start_solution_should_1);


}*/

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}