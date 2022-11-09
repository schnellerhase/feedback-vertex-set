#pragma once

#include <list>

#include "../undirected_graph.hpp"

#include "connected.hpp"
#include "extract_undirected.hpp"
#include "neighborhood.hpp"
#include "scc.hpp"

#include "reduction/core.hpp"
#include "reduction/double.hpp"
#include "reduction/edge.hpp"
#include "reduction/in.hpp"
#include "reduction/node.hpp"
#include "reduction/out.hpp"
#include "reduction/pie.hpp"

using namespace fvs;

std::tuple<std::list<SubGraph>, FVS, std::list<UndirectedGraph>>
fully_reduce_graph(const SubGraph& graph)
{
    assert(Graph::has_valid_data_structure(graph));
    assert(SubGraph::has_no_double_edges(graph));
    assert(SubGraph::has_no_self_loops(graph));

    std::list<SubGraph> finished;
    std::list<UndirectedGraph> vc_components;
    std::stack<SubGraph> to_reduce;
    to_reduce.push(std::move(graph));

    FVS fvs(graph.N(), false);

    while (!to_reduce.empty()) {
        auto subgraph = std::move(to_reduce.top());
        to_reduce.pop();

        // std::cout << "New Graph for Reduction N=" << subgraph.N() << " M=" <<
        // subgraph.M() << std::endl;

        index_t last_N = subgraph.N() + 1;
        index_t last_M = subgraph.M() + 1;

        while (subgraph.N() < last_N || subgraph.M() < last_M) {
            // std::cout << "N=" << subgraph.N() << " M=" << subgraph.M() <<
            // std::endl;
            bool restart = false;

            assert(SubGraph::has_no_double_edges(subgraph));
            assert(SubGraph::has_no_self_loops(subgraph));
            assert(Graph::has_valid_data_structure(subgraph));

            last_N = subgraph.N();
            last_M = subgraph.M();

            if (subgraph.N() == 0 && subgraph.M() == 0)
                break;

            for (index_t i = subgraph.N() - 1; i + 1 >= 1; i--) {
                assert(i < subgraph.N());

                auto indeg = subgraph.indeg()[i];
                auto outdeg = subgraph.outdeg()[i];
                if (indeg == 0 && outdeg == 0) {
                    remove_isolated_node(subgraph, i);
                } else if (indeg == 0 || outdeg == 0) {
                    remove_node(subgraph, i);
                } else if (subgraph.indeg()[i] == 1) {
                    bool removed_two_nodes = reduce_IN(subgraph, i, fvs);
                    i -= (removed_two_nodes && i > 0) * 1;
                } else if (subgraph.outdeg()[i] == 1) {
                    bool removed_two_nodes = reduce_OUT(subgraph, i, fvs);
                    i -= (removed_two_nodes && i > 0) * 1;
                }
            }

            if (subgraph.N() < last_N || subgraph.N() <= 1) {
                // std::cout << "degree 1" << std::endl;
                continue;
            }

            if (!strongly_connected(
                  subgraph)) // TOOD: run this check once, if graph does not get
                             // reduced
            {
                IndexListList sccs = strongly_connected_components(subgraph);
                assert(sccs.size() > 1);
                for (const auto& scc : sccs)
                    to_reduce.push(SubGraph::extract_subgraph(subgraph, scc));

                restart = true;
            }

            if (restart)
                continue;

            if (SubGraph::is_undirected_graph(subgraph)) {
                vc_components.push_back(extract_undirected_graph(subgraph));
                subgraph = SubGraph(0,
                                    IndexList(),
                                    IndexList(),
                                    IndexList()); // TODO: improve this
                restart = true;
            }

            if (restart)
                continue;

            index_t i = subgraph.N() - 1;
            while (i + 1 >= 1) {
                if (neighborhood_is_clique(subgraph, i) &&
                    subgraph.indeg()[i] > 1) {
                    auto nhood = neighborhood(subgraph, i);
                    reduce_CORE(subgraph, i, nhood, fvs);
                    restart = true;
                    i = std::min(i, subgraph.N() - 1);
                }
                i--;
            }

            if (restart)
                continue;

            // for (index_t i = 0; i < subgraph.N(); i++)
            // {
            //     for (index_t out(0); out < subgraph.outadj()[i].size();
            //     out++)
            //     {
            //         if (subgraph.outadj()[i][out] <= i)
            //             continue;

            //         for (index_t inneighbor(0); inneighbor <
            //         subgraph.inadj()[i].size(); inneighbor++)
            //         {
            //             if (subgraph.outadj()[i][out] !=
            //             subgraph.inadj()[i][inneighbor] )
            //                 continue;

            //             // found a two cycle!
            //             IndexList edges_to_remove;
            //             std::cout << "DOUBLE CHEKC" << std::endl;
            //             reduce_DOUBLE(subgraph, i, subgraph.outadj()[i][out],
            //             edges_to_remove); if (edges_to_remove.size() > 0)
            //             {
            //                 std::cout << "reducing" << edges_to_remove.size()
            //                 << std::endl; for (index_t j =
            //                 edges_to_remove.size() - 1; j + 1 >= 1; j--)
            //                     remove_edge(subgraph, edges_to_remove[j]);

            //                 restart = true;
            //                 break;
            //             }
            //         }

            //         if (restart)
            //             break;
            //     }
            //     if (restart)
            //         break;
            // }

            // if (restart)
            //     continue;
            reduce_PIE(subgraph);
            restart = (last_M > subgraph.M());
            if (restart) {
                // std::cout << "PIE" << std::endl;
                continue;
            }

            finished.push_back(std::move(subgraph));
        }
    }

    for (auto& g : finished)
        g.shrink_to_fit();

    // TODO: reallcoate memory. At this point reduction of used memory may be
    // quite massive
    return std::make_tuple(std::move(finished),
                           std::move(fvs),
                           std::move(vc_components)); // TODO moves!
}
