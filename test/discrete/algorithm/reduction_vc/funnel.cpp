#include <gtest/gtest.h>

#include "fvs/discrete/algorithm/reduction_vc/funnel.hpp"
// #include "discrete/util/types.hpp"
//#include "vc_solver/vc_solver.hpp"
#include "fvs/discrete/util/types.hpp"

using namespace fvs;

TEST(reduce_funnel, three_nodes_attached)
{
    UndirectedGraph graph(4, IndexList{ 0, 1, 2, 2 }, IndexList{ 1, 2, 0, 3 });
    // UndirectedGraph graph2(graph);

    VCFunnelHandler handlerShould(IndexListList({ IndexList({ 2 }) }),
                                  IndexListList({ IndexList() }),
                                  IndexListList({ IndexList({ 3 }) }),
                                  IndexListList({ IndexList() }),
                                  IndexListList({ IndexList({ 0, 1 }) }),
                                  IndexListList({ IndexList() }));
    // auto vcShould = solve_vc(graph(4, IndexList{0, 1, 2, 2}, IndexList{1, 2,
    // 0, 3}););

    VC vc(graph.N(), false);
    VCFunnelHandler handler;
    reduce_FUNNEL(graph, 2, 3, vc, handler);

    // std::cout << " before update vc: " << vc << std::endl;
    handler.update_vc_all(vc);

    // std::cout << " after update vc: " << vc << std::endl;

    ASSERT_EQ(graph, UndirectedGraph(2, IndexList({ 0 }), IndexList({ 1 })));
    // ASSERT_EQ(vc, VC({false, false, false, false}));
    ASSERT_EQ(handler, handlerShould);
    ASSERT_EQ(vc, VC({ false, false, true, false }));
}

TEST(reduce_funnel, three_nodes_attache2)
{
    UndirectedGraph graph(4, IndexList{ 0, 1, 2, 2 }, IndexList{ 1, 2, 0, 3 });
    // UndirectedGraph graph2(graph);

    VCFunnelHandler handlerShould(IndexListList({ IndexList({ 2 }) }),
                                  IndexListList({ IndexList() }),
                                  IndexListList({ IndexList({ 1 }) }),
                                  IndexListList({ IndexList() }),
                                  IndexListList({ IndexList({ 3 }) }),
                                  IndexListList({ IndexList() }));
    // auto vcShould = solve_vc(graph);

    VC vc(graph.N(), false);
    VCFunnelHandler handler;
    reduce_FUNNEL(graph, 2, 1, vc, handler);
    // std::cout << " before update vc: " << vc << std::endl;
    handler.update_vc_all(vc);

    // std::cout << " after update vc: " << vc << std::endl;

    ASSERT_EQ(graph,
              UndirectedGraph(1, IndexList(), IndexList(), IndexList({ 3 })));
    // ASSERT_EQ(vc, VC({true, false, false, false}));
    ASSERT_EQ(handler, handlerShould);
    ASSERT_EQ(vc, VC({ true, false, true, false }));
}
