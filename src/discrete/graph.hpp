#pragma once

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <unistd.h>

#include "util/buffer.hpp"
#include "util/types.hpp"
#include "vertex_marker.hpp"

using FVS = VertexMarker;

class Graph
{
  private:
    index_t _N;
    index_t _M;
    IndexList _indeg;
    IndexList _outdeg;
    IndexListList _inadj;
    IndexListList _outadj;

    IndexList _tails;
    IndexList _heads;
    IndexListList _in2arc;
    IndexListList _out2arc;

  public:
    Graph(index_t N,
          index_t M,
          IndexList indeg,
          IndexList outdeg,
          IndexListList inadj,
          IndexListList outadj,
          IndexList tails,
          IndexList heads,
          IndexListList in2arc,
          IndexListList out2arc)
      : _N(N)
      , _M(M)
      , _indeg(std::move(indeg))
      , _outdeg(std::move(outdeg))
      , _inadj(std::move(inadj))
      , _outadj(std::move(outadj))
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _in2arc(std::move(in2arc))
      , _out2arc(std::move(out2arc))
    {
        assert(_indeg.size() == _N);
        assert(_outdeg.size() == _N);
        assert(_inadj.size() == _N);
        assert(
          std::all_of(_inadj.begin(), _inadj.end(), [this](IndexList& list) {
              return list.size() < this->_N;
          }));
        assert(_outadj.size() == _N);
        assert(
          std::all_of(_outadj.begin(), _outadj.end(), [this](IndexList& list) {
              return list.size() < this->_N;
          }));

        assert(_tails.size() == _M);
        assert(_heads.size() == _M);

        assert(_in2arc.size() == _N);

        assert(
          std::all_of(_in2arc.begin(), _in2arc.end(), [this](IndexList& list) {
              return list.size() < this->_N;
          }));
        assert(std::all_of(
          _out2arc.begin(), _out2arc.end(), [this](IndexList& list) {
              return list.size() < this->_N;
          }));
    }

    Graph(IndexList indeg, IndexList outdeg, IndexList tails, IndexList heads)
      : _N(indeg.size())
      , _M(tails.size())
      , _indeg(std::move(indeg))
      , _outdeg(std::move(outdeg))
      , _inadj(IndexListList(_N))
      , _outadj(IndexListList(_N))
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _in2arc(IndexListList(_N))
      , _out2arc(IndexListList(_N))
    {
        assert(_indeg.size() == _outdeg.size());
        assert(_tails.size() == _heads.size());

        _compute_adj_lists();
    }

    Graph(index_t N, IndexList tails, IndexList heads)
      : _N(N)
      , _M(tails.size())
      , _indeg(IndexList(N))
      , _outdeg(IndexList(N))
      , _inadj(IndexListList(N))
      , _outadj(IndexListList(N))
      , _tails(std::move(tails))
      , _heads(std::move(heads))
      , _in2arc(IndexListList(N))
      , _out2arc(IndexListList(N))
    {
        assert(_tails.size() == _heads.size());

        for (index_t i = 0; i < _M; i++) {
            _outdeg[_tails[i]]++;
            _indeg[_heads[i]]++;
        }

        _compute_adj_lists();
    }

    explicit Graph(const Graph& other) =
      default; // copy should not happen uintended
    Graph& operator=(const Graph& other) =
      delete; // if you need to copy -> must use explicit copy constructor

    Graph(Graph&& other) = default;
    Graph& operator=(Graph&& other) = default;

    ~Graph() = default;

    [[nodiscard]] IndexList& indeg() { return _indeg; }

    [[nodiscard]] const IndexList& indeg() const { return _indeg; }

    [[nodiscard]] IndexList& tails() { return _tails; }

    [[nodiscard]] const IndexList& tails() const { return _tails; }

    [[nodiscard]] IndexList& heads() { return _heads; }

    [[nodiscard]] const IndexList& heads() const { return _heads; }

    [[nodiscard]] IndexList& outdeg() { return _outdeg; }

    [[nodiscard]] const IndexList& outdeg() const { return _outdeg; }

    [[nodiscard]] IndexListList& inadj() { return _inadj; }

    [[nodiscard]] const IndexListList& inadj() const { return _inadj; }

    [[nodiscard]] IndexListList& outadj() { return _outadj; }

    [[nodiscard]] const IndexListList& outadj() const { return _outadj; }

    [[nodiscard]] IndexListList& in2arc() { return _in2arc; }

    [[nodiscard]] const IndexListList& in2arc() const { return _in2arc; }

    [[nodiscard]] IndexListList& out2arc() { return _out2arc; }

    [[nodiscard]] const IndexListList& out2arc() const { return _out2arc; }

    [[nodiscard]] const index_t& N() const { return _N; }

    void N(index_t N)
    {
        assert(N <= _N);
        _N = N;
    }

    [[nodiscard]] const index_t& M() const { return _M; }

    void M(index_t M)
    {
        assert(M <= _M);
        _M = M;
    }

    void shrink_to_fit()
    {
        _indeg.shrink_to_fit();
        _outdeg.shrink_to_fit();
        _tails.shrink_to_fit();
        _heads.shrink_to_fit();

        _inadj.shrink_to_fit();
        for (auto& list : _inadj)
            list.shrink_to_fit();

        _outadj.shrink_to_fit();
        for (auto& list : _outadj)
            list.shrink_to_fit();

        _in2arc.shrink_to_fit();
        for (auto& list : _in2arc)
            list.shrink_to_fit();

        _out2arc.shrink_to_fit();
        for (auto& list : _out2arc)
            list.shrink_to_fit();
    }

    bool operator==(const Graph& rhs) const
    {
        return (_N == rhs._N) && (_M == rhs._M) && (_indeg == rhs._indeg) &&
               (_outdeg == rhs._outdeg) && (_tails == rhs._tails) &&
               (_heads == rhs._heads) && (_inadj == rhs._inadj) &&
               (_outadj == rhs._outadj) && (_in2arc == rhs._in2arc) &&
               (_out2arc == rhs._out2arc);
    }

    inline static bool has_edge(const Graph& graph, index_t from, index_t to)
    {
        const auto& outadj = graph.outadj()[from];
        auto ptr = std::find(outadj.begin(), outadj.end(), to);
        return (ptr != outadj.end());
    }

    static void write(const Graph& graph, std::ostream& os)
    {
        os << graph.N() << " " << graph.M() << " "
           << "0"
           << "\n";

        for (index_t i = 0; i < graph.N(); i++) {
            for (index_t j = 0; j < graph.outdeg()[i]; j++)
                os << graph.outadj()[i][j] + 1 << " ";

            os << "\n";
        }
    }
    static void write(const Graph& graph, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Can not open file " << filename << "!" << std::endl;
            exit(2);
        }
        write(graph, file);
        file.close();
    }

    static Graph read(FILE* fp)
    {
        buffer buf{};
        buf.line = (char*)malloc(buf.cap);

        if (buf.line == nullptr) {
            fprintf(stderr, "Fatal error while allocating small buffer\n");
            exit(1);
        }

        bool header(false);
        size_t n = 0, m = 0, e = 0;

        while (!header && buffer_line(fp, &buf)) {
            if (buf.line[0] != '#') {
                int r = sscanf(buf.line, "%zu %zu %zu", &n, &m, &e);

                if (r == 3 && n >= 1 && m >= 1 && e == 0)
                    header = true;

                break;
            }
        }

        if (!header) {
            fclose(fp);
            free(buf.line);

            fprintf(stdout,
                    "This should not happen: First line has invalid form.\n");
            exit(1);
        }

        IndexList tails(m, 0);
        IndexList heads(m, 0);

        IndexList indeg(n, 0);
        IndexList outdeg(n, 0);

        size_t eread = 0;

        for (unsigned int i = 0; i < n; ++i) {
            if (buffer_line(fp, &buf)) {
                if (strlen(buf.line) > 0 && buf.line[0] == '\n')
                    continue;

                size_t j = 0;

                int offset = 0;
                int bytes = 0;
                while (sscanf(buf.line + offset, "%zu%n", &j, &bytes) > 0) {
                    --j;

                    assert(i < n);
                    assert(j < n);

                    tails[eread] = i;
                    heads[eread] = j;
                    ++indeg[j];
                    ++outdeg[i];
                    ++eread;

                    offset += bytes;
                }
            } else {
                fprintf(
                  stdout,
                  "This should not happen: Could not read vertex line %d .\n",
                  i);
                exit(1);
            }
        }

        fclose(fp);
        free(buf.line);

        if (eread != m) {
            fprintf(
              stdout,
              "This should not happen: Either matrix-entry lines do not match "
              "specified number or some row or column index does not match the "
              "specified index bounds.\n");
            exit(1);
        }

        return Graph(indeg, outdeg, tails, heads);
    }
    static Graph read(const std::string& filename)
    {
        return read(fopen(filename.c_str(), "r"));
    }

    static bool is_acyclic(const Graph& graph)
    {
        // TODO assert graph does not have self loops

        if (graph.N() == 0)
            return true;

        std::queue<index_t> sources;
        for (index_t i = 0; i < graph.N(); i++)
            if (graph.indeg()[i] == 0)
                sources.push(i);

        if (sources.empty())
            return false;

        IndexList indeg(graph.indeg());

        index_t visited_nodes = 0;
        while (!sources.empty()) {
            index_t u = sources.front();
            sources.pop();
            visited_nodes++;

            for (const index_t v : graph.outadj()[u]) {
                indeg[v]--;
                if (indeg[v] == 0)
                    sources.push(v);
            }
        }

        return (visited_nodes == graph.N());
    }

    static bool is_acyclic(const Graph& graph, const FVS& fvs)
    {
        auto size_fvs = std::count(fvs.begin(), fvs.end(), true);

        if (graph.N() - size_fvs == 0)
            return true;

        IndexList indeg(graph.N(), 0);
        for (index_t u = 0; u < graph.N(); ++u)
            if (!fvs[u])
                for (const index_t v : graph.outadj()[u])
                    ++indeg[v];

        std::queue<index_t> sources;
        for (index_t i = 0; i < graph.N(); i++)
            if (!fvs[i] && indeg[i] == 0)
                sources.push(i);

        index_t visited_nodes = 0;
        while (!sources.empty()) {
            index_t u = sources.front();
            sources.pop();
            visited_nodes++;

            for (const index_t v : graph.outadj()[u]) {
                indeg[v]--;
                if (!fvs[v] && indeg[v] == 0)
                    sources.push(v);
            }
        }

        return (visited_nodes == (graph.N() - size_fvs));
    }

    static bool has_valid_data_structure(const Graph& graph)
    {
        if (std::accumulate(graph.indeg().begin(),
                            graph.indeg().end(),
                            index_t(0)) != graph.M())
            return false;

        if (std::accumulate(graph.outdeg().begin(),
                            graph.outdeg().end(),
                            index_t(0)) != graph.M())
            return false;

        // for (const auto &list : graph.inadj())
        //     if (! std::is_sorted(list.begin(), list.end()))
        //         return false;

        // for (const auto &list : graph.outadj())
        //     if (! std::is_sorted(list.begin(), list.end()))
        //         return false;

        for (const auto& list : graph.in2arc())
            if (!std::is_sorted(list.begin(), list.end()))
                return false;

        for (const auto& list : graph.out2arc())
            if (!std::is_sorted(list.begin(), list.end()))
                return false;

        // TODO: adjacency lists check for content.

        return true;
    }

  protected:
    void _compute_adj_lists()
    {
        for (index_t i = 0; i < _N; ++i) {
            index_t size_in = _indeg[i];
            _inadj[i] = IndexList(size_in);
            _in2arc[i] = IndexList(size_in);

            index_t size_out = _outdeg[i];
            _outadj[i] = IndexList(size_out);
            _out2arc[i] = IndexList(size_out);
        }

        IndexList idx_outdeg(_N);
        IndexList idx_indeg(_N);

        for (index_t e = 0; e < _M; e++) {
            index_t i = _tails[e];
            index_t j = _heads[e];

            _outadj[i][idx_outdeg[i]] = j;
            _out2arc[i][idx_outdeg[i]] = e;
            idx_outdeg[i] += 1;

            _inadj[j][idx_indeg[j]] = i;
            _in2arc[j][idx_indeg[j]] = e;
            idx_indeg[j] += 1;
        }

        // HANDLE WITH CARE!!!!
        // for (auto &list : _inadj)
        //     std::sort(list.begin(), list.end());

        // for (auto &list : _outadj)
        //     std::sort(list.begin(), list.end());

        for (auto& list : _in2arc)
            std::sort(list.begin(), list.end());

        for (auto& list : _out2arc)
            std::sort(list.begin(), list.end());
    }
};

std::ostream&
operator<<(std::ostream& os, const Graph& graph)
{
    // maybe use std::setw(...)
    os << "N=" << graph.N() << "\n"
       << "M=" << graph.M() << "\n"
       << "heads=" << graph.heads() << "\n"
       << "tails=" << graph.tails() << "\n"
       << "indeg=" << graph.indeg() << "\n"
       << "outdeg=" << graph.outdeg() << "\n"
       << "inadj=" << graph.inadj() << "\n"
       << "outadj=" << graph.outadj() << "\n"
       << "in2arc=" << graph.in2arc() << "\n"
       << "out2arc=" << graph.out2arc() << "\n";

    return os;
}
