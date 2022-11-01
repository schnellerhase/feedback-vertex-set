#pragma once

#include "../../subgraph.hpp"
#include "../../undirected_graph.hpp"
#include "../../util/types.hpp"

inline void
update_indices(IndexList& list, index_t i)
{
    // TODO: std::for_each is very slow in comparison
    for (auto& e : list)
        e -= (e > i);
}

template<typename T>
void erase_index(std::vector<T>& vec, index_t idx)
{
    assert(0 <= idx && idx < vec.size());
    vec.erase(vec.begin() + static_cast<std::size_t>(idx));
}

inline bool
node_has_self_loop(const SubGraph& graph, index_t& i)
{
    return (std::count(graph.inadj()[i].begin(), graph.inadj()[i].end(), i) >
            0);
}

inline bool
node_has_self_loop(const UndirectedGraph& graph, index_t& i)
{
    return (std::count(graph.adj()[i].begin(), graph.adj()[i].end(), i) > 0);
}

// https://stackoverflow.com/questions/17074324/how-can-i-sort-two-vectors-in-the-same-way-with-criteria-that-uses-only-one-of

template<typename T>
std::vector<std::size_t>
sort_permutation(const std::vector<T>& vec)
{
    std::vector<std::size_t> p(vec.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](std::size_t i, std::size_t j) {
        return vec[i] < vec[j];
    });
    return p;
}

template<typename T>
void
apply_permutation(std::vector<T>& vec, const std::vector<std::size_t>& p)
{
    for (index_t i = 0; i < vec.size() - 1; i++) {
        auto ind = p[i];
        while (ind < i)
            ind = p[ind];

        std::swap(vec[i], vec[ind]);
    }
}
