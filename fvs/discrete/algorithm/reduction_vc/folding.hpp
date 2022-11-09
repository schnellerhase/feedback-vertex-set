#pragma once

#include "../../undirected_graph.hpp"
#include "../fvs.hpp"
#include "../reduction/edge.hpp"
#include "../reduction/node.hpp"

inline void
fold(UndirectedGraph& graph, index_t bendFrom, index_t bendTo)
{
    for (auto edge : graph.to_arc()[bendTo]) {
        if ((graph.tails()[edge] == bendFrom) ||
            (graph.heads()[edge] == bendFrom)) {
            remove_edge(graph, edge);
            break;
        }
    }

    graph.deg()[bendTo] += graph.deg()[bendFrom];
    graph.deg()[bendFrom] = 0;

    for (const auto& m : graph.to_arc()[bendFrom])
        if (graph.tails()[m] == bendFrom)
            graph.tails()[m] = bendTo;
        else if (graph.heads()[m] == bendFrom)
            graph.heads()[m] = bendTo;

    for (const auto& k : graph.adj()[bendFrom]) {
        auto& adjk = graph.adj()[k];
        auto ptr = std::find(adjk.begin(), adjk.end(), bendFrom);
        assert(ptr != adjk.end());
        adjk[std::distance(adjk.begin(), ptr)] = bendTo;
    }

    graph.to_arc()[bendTo].insert(graph.to_arc()[bendTo].end(),
                                  graph.to_arc()[bendFrom].begin(),
                                  graph.to_arc()[bendFrom].end());

    auto permutation = sort_permutation(graph.to_arc()[bendTo]);
    if (graph.to_arc()[bendTo].size() > 0)
        apply_permutation(graph.to_arc()[bendTo], permutation);
    graph.to_arc()[bendFrom].clear();

    graph.adj()[bendTo].insert(graph.adj()[bendTo].end(),
                               graph.adj()[bendFrom].begin(),
                               graph.adj()[bendFrom].end());
    if (graph.adj()[bendTo].size() > 0)
        apply_permutation(graph.adj()[bendTo], permutation);
    graph.adj()[bendFrom].clear();

    remove_isolated_node(graph, bendFrom);
    bendTo -= (bendTo > bendFrom) * 1;
    for (const auto& k : graph.adj()[bendTo]) {
        auto count =
          std::count(graph.adj()[bendTo].begin(), graph.adj()[bendTo].end(), k);
        assert(count <= 2);

        if (count <= 1)
            continue;

        for (const auto& f : graph.to_arc()[bendTo]) {
            if (graph.heads()[f] == k || graph.tails()[f] == k) {
                remove_edge(graph, f);
                break;
            }
        }
    }
}

inline void
reduce_folding(UndirectedGraph& graph, index_t i, IndexList foldingNodes)
{
    //*************************************** assuming disjointness TODO
    // foldingnodes are disjoint????
    // for (auto globalNode : graph.local2global(i))
    // {
    //     for (auto foldingNode : foldingNodes)
    //     {
    //         assert(std::find(std::begin(graph.local2global(foldingNode)),
    //         std::end(graph.local2global(foldingNode)), globalNode ) ==
    //         std::end(graph.local2global(foldingNode)));
    //         assert(std::find(std::begin(graph.local2globalInvert(foldingNode)),
    //         std::end(graph.local2globalInvert(foldingNode)), globalNode ) ==
    //         std::end(graph.local2globalInvert(foldingNode)));
    //     }
    // }
    // for (auto globalNode : graph.local2globalInvert(i))
    // {
    //     for (auto foldingNode : foldingNodes)
    //     {
    //         assert(std::find(std::begin(graph.local2global(foldingNode)),
    //         std::end(graph.local2global(foldingNode)), globalNode ) ==
    //         std::end(graph.local2global(foldingNode)));
    //         assert(std::find(std::begin(graph.local2globalInvert(foldingNode)),
    //         std::end(graph.local2globalInvert(foldingNode)), globalNode ) ==
    //         std::end(graph.local2globalInvert(foldingNode)));
    //     }
    // }
    //***************************************

    IndexList newlocal2global = graph.local2globalInvert(i);
    IndexList newlocal2globalInvert = graph.local2global(i);

    for (auto foldingNode : foldingNodes) {
        for (auto i : graph.local2global(foldingNode))
            newlocal2global.push_back(i);

        for (auto i : graph.local2globalInvert(foldingNode))
            newlocal2globalInvert.push_back(i);
    }

    graph.local2global()[i] = newlocal2global;
    graph.local2globalInvert()[i] = newlocal2globalInvert;

    for (index_t foldingNode(0); foldingNode < foldingNodes.size();
         foldingNode++) {
        fold(graph, foldingNodes[foldingNode], i);
        update_indices(foldingNodes, foldingNodes[foldingNode]);
        i -= (i > foldingNodes[foldingNode]);
    }
}
