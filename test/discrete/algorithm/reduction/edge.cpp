#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction/edge.hpp"

using namespace fvs;

TEST(edge_redcution_undirected, simple)
{
    UndirectedGraph graph(2, IndexList{ 0 }, IndexList{ 1 });
    remove_edge(graph, 0);
    ASSERT_EQ(graph, UndirectedGraph(2, IndexList(), IndexList()));
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
