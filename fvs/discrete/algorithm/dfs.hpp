#pragma once

#include <cstdio>
#include <iostream>
#include <list>
#include <span>
#include <stack>
#include <vector>

#include "../undirected_graph.hpp"
#include "../subgraph.hpp"

namespace fvs
{

const IndexList&
DFS_forward(const SubGraph& graph, index_t i)
{
    return graph.outadj()[i];
}

const IndexList&
DFS_forward(const UndirectedGraph& graph, index_t i)
{
    return graph.adj()[i];
}

const IndexList&
DFS_backwards(const SubGraph& graph, index_t i)
{
    return graph.inadj()[i];
}

template<typename graph_t = SubGraph,
         const IndexList& (*neighbors)(const graph_t&, index_t) = DFS_forward>
class DFS
{
  private:
    const graph_t& _graph;
    index_t _current;
    std::span<bool> _visited;
    std::vector<index_t> _stack;

  public:
    DFS() = delete;
    DFS(const DFS& other) = delete;
    DFS(DFS&& other) = delete;

    DFS& operator=(DFS other) = delete;
    DFS& operator=(DFS&& other) = delete;

    explicit DFS(const graph_t& graph)
      : _graph(graph)
      , _current(0)
      , _visited(new bool[graph.N()], graph.N())
      , _stack()
    {
        for (auto& e : _visited)
            e = false;
    }

    ~DFS() { delete[] _visited.data(); }

    void current(index_t i) { _current = i; }

    [[nodiscard]] bool visited(index_t i) const
    {
        assert(i < _graph.N());
        return _visited[i];
    }

    index_t& operator*() { return _current; }

    bool next()
    {
        while (_visited[_current]) {
            if (_stack.empty())
                return false;

            _current = _stack.back();
            _stack.pop_back();
        }

        assert(!_visited[_current]);

        _visited[_current] = true;

        for (const auto& w : neighbors(_graph, _current))
            if (!_visited[w])
                _stack.push_back(w);

        return true;
    }
};

}