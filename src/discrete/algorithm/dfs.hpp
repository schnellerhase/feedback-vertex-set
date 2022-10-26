#pragma once

#include <cstdio>
#include <iostream>
#include <list>
#include <stack>
#include <vector>

#include "../subgraph.hpp"
#include "../undirected_graph.hpp"

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
    bool* _visited;
    std::vector<index_t> _stack;

  public:
    DFS(const graph_t& graph)
      : _graph(graph)
      , _current(0)
      , _visited(new bool[graph.N()])
      , _stack()
    {
        for (index_t i = 0; i < graph.N(); i++)
            _visited[i] = false;
    }

    ~DFS() { delete[] _visited; }

    void current(index_t i) { _current = i; }

    bool visited(index_t i)
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
