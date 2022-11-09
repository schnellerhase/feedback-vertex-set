#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>

#include "util/types.hpp"

class UndirectedGraph
{
  private:
    index_t _N;
    index_t _M;
    IndexList _tails;
    IndexList _heads;
    IndexListList _local2global;
    IndexListList _local2globalInvert;
    IndexList _deg;
    IndexListList _adj;
    IndexListList _2arc;

  public:
    UndirectedGraph(UndirectedGraph&& other) = default;
    UndirectedGraph(const UndirectedGraph& other) = delete;
    UndirectedGraph& operator=(const UndirectedGraph& other) =
      default; // TODO!!!
    UndirectedGraph& operator=(UndirectedGraph&& other) = default;
    UndirectedGraph()
      : _N(0)
      , _M(0)
      , _tails()
      , _heads()
      , _local2global()
      , _local2globalInvert()
      , _deg()
      , _adj()
      , _2arc()
    {
    }

    UndirectedGraph(index_t N,
                    IndexList tails,
                    IndexList heads,
                    IndexListList local2global,
                    IndexListList local2globalInvert)
      : _N(N)
      , _M(tails.size())
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _local2global(std::move(local2global))
      , _local2globalInvert(std::move(local2globalInvert))
      , _deg(_N)
      , _adj(_N)
      , _2arc(_N)
    {
        assert(_tails.size() == _heads.size());
        assert(_local2global.size() == N);
        assert(_local2globalInvert.size() == N);
        _compute_adj_lists();
    }

    UndirectedGraph(index_t N, IndexList tails, IndexList heads)
      : _N(N)
      , _M(tails.size())
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _local2global(_N)
      , _local2globalInvert(_N)
      , _deg(_N)
      , _adj(_N)
      , _2arc(_N)
    {
        assert(_tails.size() == _heads.size());
        _compute_adj_lists();
        for (index_t node(0); node < _N; node++) {
            _local2global[node] = IndexList({ node });
        }
    }

    UndirectedGraph(index_t N,
                    IndexList tails,
                    IndexList heads,
                    IndexList local2global)
      : _N(N)
      , _M(tails.size())
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _local2global(_N)
      , _local2globalInvert(_N)
      , _deg(_N)
      , _adj(_N)
      , _2arc(_N)
    {
        assert(_tails.size() == _heads.size());
        _compute_adj_lists();
        for (index_t node(0); node < _N; node++) {
            _local2global[node] = IndexList({ local2global[node] });
        }
    }

    ~UndirectedGraph() = default;

    [[nodiscard]] index_t N() const { return _N; }

    void N(index_t N) { _N = N; }

    [[nodiscard]] index_t M() const { return _M; }

    void M(index_t M) { _M = M; }

    [[nodiscard]] const IndexList& tails() const { return _tails; }

    [[nodiscard]] IndexList& tails() { return _tails; }

    [[nodiscard]] const IndexList& heads() const { return _heads; }

    [[nodiscard]] IndexList& heads() { return _heads; }

    [[nodiscard]] const IndexList& deg() const { return _deg; }

    [[nodiscard]] IndexList& deg() { return _deg; }

    [[nodiscard]] const IndexListList& adj() const { return _adj; }

    [[nodiscard]] IndexListList& adj() { return _adj; }

    [[nodiscard]] const IndexListList& to_arc() const { return _2arc; }

    [[nodiscard]] IndexListList& to_arc() { return _2arc; }

    [[nodiscard]] const IndexListList& local2global() const
    {
        return _local2global;
    }

    [[nodiscard]] IndexListList& local2global() { return _local2global; }

    [[nodiscard]] IndexList local2global(index_t local_idx) const
    {
        return _local2global[local_idx];
    }

    [[nodiscard]] const IndexListList& local2globalInvert() const
    {
        return _local2globalInvert;
    }

    [[nodiscard]] IndexListList& local2globalInvert()
    {
        return _local2globalInvert;
    }

    [[nodiscard]] IndexList local2globalInvert(index_t local_idx) const
    {
        return _local2globalInvert[local_idx];
    }

    void shrink_to_fit()
    {
        _deg.shrink_to_fit();
        _tails.shrink_to_fit();
        _heads.shrink_to_fit();
        for (auto& l : _adj)
            l.shrink_to_fit();
        for (auto& l : _2arc)
            l.shrink_to_fit();
    }

    bool operator==(const UndirectedGraph& rhs) const
    {
        return (_N == rhs._N) && (_M == rhs._M) && (_deg == rhs._deg) &&
               (_tails == rhs._tails) && (_heads == rhs._heads) &&
               (_adj == rhs._adj) && (_2arc == rhs._2arc) &&
               (_local2global == rhs._local2global) &&
               (_local2globalInvert == rhs._local2globalInvert);
    }

    static UndirectedGraph read(const std::string& path)
    {
        std::ifstream infile(path);
        if (!infile) {
            std::cout << "Can not open file!" << std::endl;
            exit(2);
        }
        std::string p, td;
        index_t N = 0, M = 0;
        infile >> p >> td >> N >> M;

        IndexList tails, heads;
        tails.reserve(M);
        heads.reserve(M);

        index_t head = 0, tail = 0;
        while (infile >> tail >> head) {
            tails.push_back(tail - 1);
            heads.push_back(head - 1);
        }

        infile.close();

        return { N, tails, heads };
    }

    static void write(const UndirectedGraph& graph, std::ostream& os)
    {
        os << "p td " << graph.N() << " " << graph.M() << "\n";

        for (index_t m = 0; m < graph.M(); m++)
            os << graph.tails()[m] + 1 << " " << graph.heads()[m] + 1 << "\n";
    }

    static void write(const UndirectedGraph& graph, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Can not open file " << filename << "!" << std::endl;
            exit(2);
        }
        write(graph, file);
        file.close();
    }

    static inline bool has_edge(const UndirectedGraph& graph,
                                index_t from,
                                index_t to)
    {
        const auto& adj = graph.adj()[from];
        auto ptr = std::find(adj.begin(), adj.end(), to);
        return (ptr != adj.end());
    }

  protected:
    void _compute_adj_lists()
    {
        for (index_t i = 0; i < _M; i++) {
            _deg[_tails[i]]++;
            _deg[_heads[i]]++;
        }

        for (index_t i = 0; i < _N; ++i) {
            index_t size = _deg[i];
            _adj[i] = IndexList(size);
            _2arc[i] = IndexList(size);
        }

        IndexList idx_deg(_N);

        for (index_t e = 0; e < _M; e++) {
            index_t i = _tails[e];
            index_t j = _heads[e];

            _adj[j][idx_deg[j]] = i;
            _adj[i][idx_deg[i]] = j;

            _2arc[j][idx_deg[j]] = e;
            _2arc[i][idx_deg[i]] = e;

            idx_deg[j] += 1;
            idx_deg[i] += 1;
        }

        // HANDLE WITH CARE!!!!
        // for (auto &list : _adj)
        //     std::sort(list.begin(), list.end());

        for (auto& list : _2arc)
            std::sort(list.begin(), list.end());
    }
};

std::ostream&
operator<<(std::ostream& os, const UndirectedGraph& graph)
{
    os << "N= " << graph.N() << "\n"
       << "M= " << graph.M() << "\n"
       << "heads= " << graph.heads() << "\n"
       << "tails= " << graph.tails() << "\n"
       << "deg= " << graph.deg() << "\n"
       << "adj= " << graph.adj() << "\n"
       << "2arc= " << graph.to_arc() << "\n"
       << "local2global= " << graph.local2global() << "\n"
       << "local2globalInvert= " << graph.local2globalInvert() << "\n";
    return os;
}
