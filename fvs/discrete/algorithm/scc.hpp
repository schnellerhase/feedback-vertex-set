#pragma once

#include <cassert>

#include "../subgraph.hpp"

namespace fvs
{


IndexList
low_links_sccs(const Graph& graph)
{
    // TODO: update with
    // https://www.cs.cmu.edu/~15451-f18/lectures/lec19-DFS-strong-components.pdf
    if (graph.N() == 0)
        return {};

    IndexList components(graph.N(), graph.N());
    IndexList low(graph.N(), graph.N());
    IndexList index(graph.N(), graph.N());
    IndexList neigh_count(graph.N(), static_cast<index_t>(0));

    std::stack<index_t> set;
    index_t nextIndex = 0;
    index_t comp_id = 0;

    for (index_t node = 0; node < graph.N(); node++) {
        if (index[node] != graph.N())
            continue;

        // index[node] == graph.N() -> i.e. not visited
        // start DFS in node
        std::stack<index_t> dfs_stack;
        dfs_stack.push(node);

        bool forwards = true;

        while (!dfs_stack.empty()) {
            bool restart = false;
            index_t i = dfs_stack.top();
            const auto& out_neigh = graph.outadj()[i];
            if (forwards) {
                index[i] = low[i] = nextIndex++;
                set.push(i);
            } else {
                index_t crt_out_neigh = out_neigh[neigh_count[i]++];
                low[i] = std::min(low[i], low[crt_out_neigh]);
            }

            while (neigh_count[i] < out_neigh.size() && !restart) {
                index_t crt_out_neigh = out_neigh[neigh_count[i]];

                if (index[crt_out_neigh] == graph.N()) {
                    dfs_stack.push(crt_out_neigh);
                    forwards = true;
                    restart = true;
                } else {
                    low[i] = std::min(low[i], low[crt_out_neigh]);
                    neigh_count[i]++;
                }
            }

            if (restart)
                continue;

            if (low[i] == index[i]) {
                index_t node = 0;
                do {
                    node = set.top();
                    set.pop();
                    components[node] = comp_id;
                    low[node] = graph.N();
                } while (node != i);
                comp_id++;
            }
            dfs_stack.pop();
            forwards = false;
        }
    }
    return components;
}

IndexListList
strongly_connected_components(const SubGraph& graph)
{
    // TODO!
    // https://github.com/tpet/lemon/blob/master/lemon/connectivity.h

    assert(SubGraph::has_no_self_loops(graph));

    IndexListList result;

    if (graph.N() == 0)
        return result;

    auto low = low_links_sccs(graph);
    result.resize(*std::max_element(low.begin(), low.end()) + 1);

    for (index_t i = 0; i < graph.N(); i++)
        result[low[i]].push_back(i);

    return result;
}

IndexList
scc_double_version(const Graph& graph, index_t removed_node)
{
    IndexList components(graph.N(), graph.N());

    if (graph.N() == 0)
        return {};

    IndexList lowLinks(graph.N(),
                       graph.N()); // lowest vertex(foundat value) reachable
                                   // from ith vertex(as in input graph)
    IndexList index(graph.N(), graph.N());
    IndexList neighborCounter(graph.N(), index_t(0));

    std::stack<index_t> set; // TODO: replace by vector
    index_t nextIndex = 0;
    index_t result = 0;

    // handle removed_node
    index[removed_node] = graph.N() + 1;

    for (index_t node(0); node < graph.N(); node++) {
        if (index[node] == graph.N()) {
            std::stack<index_t> stack;
            stack.push(node);
            bool forwards = true;

            while (!stack.empty()) {
                bool restartLoop = false;
                index_t currentNode = stack.top();
                auto crt_out_neighs =
                  graph.outadj()[currentNode]; // besser machen
                if (forwards) {
                    index[currentNode] = lowLinks[currentNode] = nextIndex++;
                    set.push(currentNode);
                } else {
                    index_t crt_out_neigh =
                      crt_out_neighs[neighborCounter[currentNode]++];
                    lowLinks[currentNode] =
                      std::min(lowLinks[currentNode], lowLinks[crt_out_neigh]);
                }

                while (neighborCounter[currentNode] < crt_out_neighs.size() &&
                       !restartLoop) {
                    index_t crt_out_neigh =
                      crt_out_neighs[neighborCounter[currentNode]];

                    if (index[crt_out_neigh] == graph.N()) {
                        stack.push(crt_out_neigh);
                        forwards = true;
                        restartLoop = true;
                    } else {
                        lowLinks[currentNode] = std::min(
                          lowLinks[currentNode], lowLinks[crt_out_neigh]);
                        neighborCounter[currentNode]++;
                    }
                }

                if (restartLoop) {
                    continue;
                }

                if (lowLinks[currentNode] == index[currentNode]) {
                    index_t node = 0;
                    do {
                        node = set.top();
                        set.pop();
                        components[node] = result;
                        lowLinks[node] = graph.N();
                    } while (node != currentNode);
                    result++;
                }
                stack.pop();
                forwards = false;
                // delete outedges???
            }
        }
    }
    components[removed_node] = result;
    return components;
}

}