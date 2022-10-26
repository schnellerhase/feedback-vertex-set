#include <gtest/gtest.h>

#include "discrete/algorithm/scc.hpp"
#include "discrete/graph.hpp"

TEST(Graph, empty)
{
    Graph graph(0, IndexList{}, IndexList{});
    ASSERT_EQ(graph.N(), 0);
    ASSERT_EQ(graph.M(), 0);
    ASSERT_EQ(graph.indeg(), IndexList{});
    ASSERT_EQ(graph.outdeg(), IndexList{});
    ASSERT_EQ(graph.inadj(), IndexListList{});
    ASSERT_EQ(graph.outadj(), IndexListList{});
    ASSERT_EQ(graph.heads(), IndexList{});
    ASSERT_EQ(graph.tails(), IndexList{});
    ASSERT_EQ(graph.out2arc(), IndexListList{});
    ASSERT_EQ(graph.in2arc(), IndexListList{});
}

TEST(Graph, single_node)
{
    Graph graph(1, IndexList{}, IndexList{});

    ASSERT_EQ(graph.N(), 1);
    ASSERT_EQ(graph.M(), 0);
    ASSERT_EQ(graph.indeg(), IndexList{ 0 });
    ASSERT_EQ(graph.outdeg(), IndexList{ 0 });
    ASSERT_EQ(graph.inadj(), IndexListList{ {} });
    ASSERT_EQ(graph.outadj(), IndexListList{ {} });
    ASSERT_EQ(graph.heads(), IndexList{});
    ASSERT_EQ(graph.tails(), IndexList{});
    ASSERT_EQ(graph.out2arc(), IndexListList{ {} });
    ASSERT_EQ(graph.in2arc(), IndexListList{ {} });
}

TEST(Graph, simple)
{
    IndexList tails{ 0, 0, 1, 2, 3 };
    IndexList heads{ 1, 2, 2, 3, 0 };
    Graph graph(4, tails, heads);

    auto indeg = IndexList{ 1, 1, 2, 1 };
    auto outdeg = IndexList{ 2, 1, 1, 1 };
    auto inadj = IndexListList{ { 3 }, { 0 }, { 0, 1 }, { 2 } };
    auto outadj = IndexListList{ { 1, 2 }, { 2 }, { 3 }, { 0 } };
    ASSERT_EQ(graph.N(), 4);
    ASSERT_EQ(graph.M(), 5);
    ASSERT_EQ(graph.indeg(), indeg);
    ASSERT_EQ(graph.outdeg(), outdeg);

    ASSERT_EQ(graph.inadj(), inadj);
    ASSERT_EQ(graph.outadj(), outadj);
    ASSERT_EQ(graph.heads(), heads);
    ASSERT_EQ(graph.tails(), tails);

    // TODO!!!
    //  ASSERT_EQ(graph.out2arc(), );
    //  ASSERT_EQ(graph.in2arc(), );

    const char* str = "4 5 0\n"
                      "2 3 \n"
                      "3 \n"
                      "4 \n"
                      "1 \n";
    Graph g_in = Graph::read("data/graph_simple");
    std::ostringstream ostream;
    Graph::write(g_in, ostream);
    EXPECT_EQ(str, ostream.str());
    EXPECT_EQ(graph, g_in);
}

std::string
read_file(const char* filename)
{
    std::ifstream in(filename);
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

TEST(Graph, rw_e_001)
{
    SubGraph graph(Graph::read("data/graph_e_001"));
    ASSERT_TRUE(Graph::has_valid_data_structure(graph));
    std::ostringstream ostream;
    Graph::write(graph, ostream);

    ASSERT_EQ(read_file("data/graph_e_001"), ostream.str());

    auto scc = strongly_connected_components(graph);
    ASSERT_EQ(scc.size(), 434);
}

int
main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
