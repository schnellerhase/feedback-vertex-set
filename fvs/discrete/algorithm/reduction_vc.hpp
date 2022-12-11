#pragma once

#include <numeric>

#include "cc.hpp"
#include "connected.hpp"
#include "fvs/discrete/undirected_graph.hpp"
#include "reduction_vc/domination.hpp"
#include "reduction_vc/folding.hpp"
#include "reduction_vc/funnel.hpp"
#include "reduction_vc/simplicial.hpp"

namespace fvs {

using VC = VertexMarker;

std::list<UndirectedGraph>
reduce_graph_VC(UndirectedGraph& graph, VC& vc, VCFunnelHandler& vcHandler)
{
    std::list<UndirectedGraph> finished;
    std::list<UndirectedGraph> graphs;
    graphs.push_back(std::move(graph));
    // index_t funnelCounter(0);

    while (!graphs.empty()) {
        auto graph = std::move(graphs.back());
        graphs.pop_back();
        // std::cout << "New Graph for VC Reduction N=" << graph.N() << " M=" <<
        // graph.M() << std::endl;

        index_t last_N = graph.N() + 1;
        index_t last_M = graph.M() + 1;

        while (graph.N() < last_N || graph.M() < last_M) {
            // std::cout << "N=" << graph.N() << " M=" << graph.M() <<
            // std::endl;
            last_N = graph.N();
            last_M = graph.M();
            bool restart = false;

            for (index_t i = graph.N() - 1; i + 1 >= 1; i--) {
                assert(i < graph.N());
                auto deg = graph.deg()[i];

                if (deg == 0) {
                    add_invert_to_vc(vc, i, graph);
                    remove_isolated_node(graph, i);
                } else if (deg == 1) {
                    assert(graph.adj()[i].size() == 1);
                    auto j = graph.adj()[i][0];
                    add_to_vc(vc, j, graph);
                    add_invert_to_vc(vc, i, graph);
                    remove_node(graph, std::max(i, j));
                    remove_node(graph, std::min(i, j));
                    i -= (i > 0) * 1;
                }
            }

            if (graph.M() == 0 || graph.N() <= 1)
                continue;

            if (!connected(graph)) {
                for (const auto& cc : connected_components(graph))
                    graphs.push_back(extract_subgraph_undirected(graph, cc));

                graph = UndirectedGraph(0, IndexList(), IndexList());
                restart = true;
            }

            if (restart)
                continue;

            index_t i = graph.N() - 1;
            // index_t simplicialCounter(0);
            while (i + 1 >= 1) {
                if (neighborhood_is_clique(graph, i) && graph.deg()[i] > 1) {
                    // simplicialCounter++;
                    auto nhood = neighborhood(graph, i);
                    reduce_SIMPLICIAL(graph, i, nhood, vc);
                    restart = true;
                    i = std::min(i, graph.N() - 1);
                }
                i--;
            }

            // if (funnelCounter > 0)
            //     //std::cout << "SIMPLICIAL(): " << simplicialCounter <<
            //     std::endl;
            if (restart)
                continue;

            i = graph.N() - 1;

            // index_t dominationCounter(0);
            while (i + 1 >= 1) {
                auto nhoodI = neighborhood(graph, i);
                for (const index_t& j : graph.adj()[i]) {
                    if (domination_check(graph, i, nhoodI, j)) {
                        // dominationCounter++;
                        // std::cout << "DOMINATION()" std::endl;
                        reduce_domination(graph, i, vc);
                        restart = true;
                        break;
                    }
                }
                i--;
            }
            // if (funnelCounter > 0)
            //     std::cout << "Domination(): " << dominationCounter <<
            //     std::endl;

            if (restart)
                continue;

            i = graph.N() - 1;
            // index_t foldingCounter(0);

            while (i + 1 >= 1) {

                assert(graph.deg()[i] > 1);
                if (graph.deg()[i] == 2) // (foldingCounter  7)
                {

                    // foldingCounter++;
                    assert(!neighborhood_is_clique(graph, i));
                    reduce_folding(graph, i, graph.adj()[i]);
                    restart = true;
                    break;
                }
                i--;
            }

            // if (funnelCounter > 0)
            //     std::cout << "FOLDING(): " << foldingCounter << std::endl;

            if (restart)
                continue;

            i = graph.N() - 1;

            while (i + 1 >= 1) {
                assert(graph.deg()[i] > 1);
                for (const index_t& j : graph.adj()[i]) {
                    if (neighborhood_is_clique_funnel(graph, i, j)) {
                        // funnelCounter++;
                        assert(!neighborhood_is_clique(graph, i));
                        reduce_FUNNEL(graph, i, j, vc, vcHandler);
                        restart = true;
                        break;
                    }
                }
                if (restart)
                    break;
                i--;
            }
            // if (funnelCounter > 0)
            //     std::cout << "funnel(): " << funnelCounter << std::endl;
        }

        if (graph.N() != 0 && graph.M() != 0) {
            // std::cout<<
            // "!!!!!!!!!!!!!!!!!!!!!!!!!!!!FULLYREDUCEDCOMPONENT!!!!!!!!!!!!!!!:
            // " << graph.N() << "      " << graph.M() << std::endl;
            finished.push_back(std::move(graph));
        }
    }

    for (auto& g : finished)
        g.shrink_to_fit();

    return finished;
}
}