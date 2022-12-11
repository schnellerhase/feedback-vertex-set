#pragma once

#include <tuple>

#include "fvs/discrete/subgraph.hpp"
#include "fvs/discrete/undirected_graph.hpp"

namespace fvs {

namespace {
void
sort_and_uniquify(IndexList& list)
{
    std::sort(list.begin(), list.end());
    auto new_end = std::unique(list.begin(), list.end());
    list.resize(std::distance(list.begin(), new_end));
}
}

IndexList
neighborhood(const UndirectedGraph& graph, index_t i)
{
    IndexList nhood;

    const auto& adj = graph.adj()[i];
    nhood.reserve(adj.size() + 1);
    nhood.push_back(i);
    nhood.insert(nhood.end(), adj.begin(), adj.end());

    sort_and_uniquify(nhood);
    return nhood;
}

IndexList
neighborhood_funnel(const UndirectedGraph& graph, index_t i, index_t j)
{
    IndexList nhood;

    const auto& adj = graph.adj()[i];

    nhood.reserve(adj.size());

    for (const auto& k : graph.adj()[i])
        if (k != j)
            nhood.push_back(k);

    sort_and_uniquify(nhood);
    return nhood;
}

std::tuple<IndexList, IndexList, IndexList>
decompose_nhoods(const UndirectedGraph& graph, index_t v, index_t u)
{
    IndexList intersection({});
    IndexList vExclusive({});
    IndexList uExclusive({});

    auto nhoodv = neighborhood_funnel(graph, v, u);
    auto nhoodu = neighborhood_funnel(graph, u, v);

    index_t nhooduItt(0);
    index_t nhoodvItt(0);

    while ((nhooduItt < nhoodu.size()) && (nhoodvItt < nhoodv.size())) {
        if (nhoodu[nhooduItt] < nhoodv[nhoodvItt])
            uExclusive.push_back(nhoodu[nhooduItt++]);

        else if (nhoodv[nhoodvItt] < nhoodu[nhooduItt])
            vExclusive.push_back(nhoodv[nhoodvItt++]);

        else {
            intersection.push_back(nhoodu[nhooduItt++]);
            nhoodvItt++;
        }
    }

    if (nhooduItt < nhoodu.size())
        uExclusive.insert(uExclusive.end(),
                          (nhoodu.begin() + static_cast<long>(nhooduItt)),
                          nhoodu.end());
    else if (nhoodvItt < nhoodv.size())
        vExclusive.insert(vExclusive.end(),
                          (nhoodv.begin() + static_cast<long>(nhoodvItt)),
                          nhoodv.end());

    return std::make_tuple(
      std::move(intersection), std::move(vExclusive), std::move(uExclusive));
}

IndexList
neighborhood(const SubGraph& graph, index_t i)
{
    IndexList nhood;

    const auto& inadj = graph.inadj()[i];
    const auto& outadj = graph.outadj()[i];

    nhood.reserve(outadj.size() + inadj.size() + 1);

    nhood.push_back(i);

    for (const auto& k : graph.outadj()[i])
        nhood.push_back(k);

    for (const auto& k : graph.inadj()[i])
        nhood.push_back(k);

    sort_and_uniquify(nhood);
    return nhood;
}
}