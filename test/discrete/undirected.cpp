#include <gtest/gtest.h>

#include "discrete/discrete.hpp"

// TEST(Graph, undirected_simple)
// {
//     auto tails = IndexList{0, 1, 2, 3, 3};
//     auto heads = IndexList{1, 2, 3, 0, 2};
//     Graph graph(4, tails, heads);

//     IndexList pseudoTailsShould = IndexList{0,1,2,3};
//     IndexList pseudoHeadsShould = IndexList{1,2,3,0};

//     auto tailsHeadsTuple = extract_undirected_edges(graph, false);

//     ASSERT_EQ(std::get<0>(tailsHeadsTuple), pseudoTailsShould);
//     ASSERT_EQ(std::get<1>(tailsHeadsTuple), pseudoHeadsShould);
// }

// TEST(Graph, undirected_complex)
// {
//     auto tails = IndexList{0, 1, 2, 2, 0, 3, 3, 4, 5, 5, 6};
//     auto heads = IndexList{1, 2, 0, 1, 3, 4, 6, 5, 0, 4, 0};
//     Graph graph(7, tails, heads);

//     IndexList pseudoTailsShould = IndexList{0,1,2,0,3,3,4,5,6};
//     IndexList pseudoHeadsShould = IndexList{1,2,0,3,4,6,5,0,0};

//     auto tailsHeadsTuple = extract_undirected_edges(graph, false);

//     ASSERT_EQ(std::get<0>(tailsHeadsTuple), pseudoTailsShould);
//     ASSERT_EQ(std::get<1>(tailsHeadsTuple), pseudoHeadsShould);
// }

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}