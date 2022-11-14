#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "fvs/discrete/discrete.hpp"

static const double INF = std::numeric_limits<double>::max();
static const double EPS = 1e-6;

class CycleSeparation // NOLINT
{
  public:
    CycleSeparation(const Graph& graph)
      : _graph(graph)
      , _pv(graph.N())
      , _pe(graph.N())
      , _pl(graph.N())
      , _dist(graph.N())
      , _weight(graph.M())
      , _heap()
      , _heapid(graph.M())
      , _heaprun(0)
      , _cuts()
    {
        _heap.reserve(graph.M());
        _cuts.reserve(graph.M());
    }

    ~CycleSeparation() = default;

    void separate(double* x)
    {
        update_weights(x);

        _cuts.resize(0);

        find_cycles<true>();
    }

    bool check(double* x)
    {
        update_weights(x);
        return !find_cycles<false>();
    }

    [[nodiscard]] const std::vector<std::vector<index_t>>& cuts() const
    {
        return _cuts;
    }

  private:
    const Graph& _graph;

    std::vector<std::vector<index_t>> _cuts;
    std::vector<index_t> _pe; // outgoing edge
    std::vector<index_t> _pv; // outgoing vertex connected with prev edge
    std::vector<index_t> _pl; // length of cycle?
    std::vector<double> _dist;
    std::vector<double> _weight;

    std::vector<index_t> _heap;
    std::vector<int> _heapid;
    int _heaprun; // NOLINT

    void update_weights(double* x)
    {
        for (index_t e = 0; e < _graph.M(); ++e)
            _weight[e] = (1.0 - x[e]); // NOLINT
    }

    template<bool cut>
    bool find_cycle_from(index_t s)
    {
        for (index_t i = 0; i < _graph.N(); ++i) {
            _dist[i] = INF;
            _pv[i] = std::numeric_limits<index_t>::max();
            _pl[i] = std::numeric_limits<index_t>::max();
            _heapid[i] = 0;
        }

        _heaprun = 0;
        _heap.resize(0);

        const index_t start = s;
        _dist[start] = 0.0;
        _pv[start] = 0;
        _pl[start] = 0;

        _heap.push_back(start);
        _heapid[start] = _heaprun++;

        for (index_t k = 0; k < _graph.indeg()[s]; ++k) {
            const index_t t = _graph.inadj()[s][k];

            if constexpr (cut)
                if (t < s)
                    continue;

            double w_cl = _weight[_graph.in2arc()[s][k]];

            if (w_cl + EPS > 1.0)
                continue;

            if (_dist[t] < INF && _heapid[t] < 0) {
                if (_dist[t] + w_cl + EPS < 1.0) {

                    if constexpr (cut)
                        cut_cycle(s, t, k);
                    else
                        return true;
                }
            }

            while (!_heap.empty()) {
                index_t node = _heap[0];
                std::pop_heap(_heap.begin(), _heap.end(), std::greater<>{});
                _heap.resize(_heap.size() - 1);

                _heapid[node] = -1;

                if (_dist[node] + EPS > 1.0) {
                    continue;
                }

                for (index_t l = 0; l < _graph.outdeg()[node]; ++l) {
                    const index_t j = _graph.outadj()[node][l];

                    if constexpr (cut)
                        if (j < s)
                            continue;

                    const double new_dist_j =
                      _weight[_graph.out2arc()[node][l]] + _dist[node];

                    if constexpr (cut)
                        if (new_dist_j >= 1.0)
                            continue;

                    if (_heapid[j] >= 0 &&
                        ((new_dist_j < _dist[j]) ||
                         (new_dist_j <= _dist[j] && _pl[node] + 1 < _pl[j]))) {
                        _dist[j] = new_dist_j;
                        _pv[j] = node;
                        _pe[j] = _graph.out2arc()[node][l];
                        _pl[j] = _pl[node] + 1;

                        if (!_heapid[j]) {

                            _heap.push_back(j);

                            std::push_heap(
                              _heap.begin(), _heap.end(), std::greater<>{});

                            _heapid[j] = _heaprun++;
                        } else {
                            if (!std::is_heap(
                                  _heap.begin(), _heap.end(), std::greater<>{}))
                                std::make_heap(
                                  _heap.begin(), _heap.end(), std::greater<>{});
                        }
                    }
                }

                if (node == t) {

                    if (_dist[node] + w_cl + EPS < 1.0) {
                        if constexpr (cut)
                            cut_cycle(s, t, k);
                        else
                            return true;
                    }

                    break;
                }
            }
        }
        return false;
    }

    template<bool cut>
    bool find_cycles()
    {
        for (index_t s = 0; s < _graph.N(); ++s) {
            bool result = find_cycle_from<cut>(s);
            if constexpr (!cut)
                if (result)
                    return true;
        }
        return false;
    }

    void cut_cycle(
      index_t s,
      index_t t,
      index_t k) // TODO: should i not be able to compute one of this on my own?
    {
        // we have found a cycle t -> s with edge between t->s is given by k-th
        // in edge of s.

        _cuts.emplace_back(_pl[t] + 1);

        for (index_t v = t; v != s; v = _pv[v])
            _cuts.back()[_pl[v]] = _pe[v];

        _cuts.back()[0] = _graph.in2arc()[s][k];
    }
};
