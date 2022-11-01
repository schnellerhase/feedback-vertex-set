#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>

#include "graph.hpp"

class SubGraph : public Graph
{

  private:
    IndexList _local2global;

  public:
    SubGraph(size_t N,
             size_t M,
             IndexList indeg,
             IndexList outdeg,
             IndexListList inadj,
             IndexListList outadj,
             IndexList tails,
             IndexList heads,
             IndexListList in2arc,
             IndexListList out2arc,
             IndexList local2global)
      : Graph(N, M, indeg, outdeg, inadj, outadj, tails, heads, in2arc, out2arc)
      , _local2global(std::move(local2global))
    {
        assert(_local2global.size() == this->N());
    }

    SubGraph(IndexList indeg,
             IndexList outdeg,
             IndexList tails,
             IndexList heads,
             IndexList local2global)
      : Graph(indeg, outdeg, tails, heads)
      , _local2global(std::move(local2global))
    {
        assert(_local2global.size() == this->N());
    }

    SubGraph(index_t N,
             IndexList tails,
             IndexList heads,
             IndexList local2global)
      : Graph(N, tails, heads)
      , _local2global(std::move(local2global))
    {
        assert(_local2global.size() == this->N());
    }

    SubGraph(index_t N, IndexList tails, IndexList heads)
      : Graph(N, tails, heads)
      , _local2global(N)
    {
        assert(_local2global.size() == this->N());
        std::iota(_local2global.begin(), _local2global.end(), 0);
    }

    explicit SubGraph(const SubGraph& other) =
      default; // copy should not happen uintended
    SubGraph& operator=(const SubGraph& other) =
      delete; // if you need to copy -> must use explicit copy constructor

    SubGraph(SubGraph&& other) = default;
    SubGraph& operator=(SubGraph&& other) = default;

    explicit SubGraph(Graph&& other)
      : Graph(other)
      , _local2global(other.N())
    {
        std::iota(_local2global.begin(), _local2global.end(), 0);
    }

    ~SubGraph() = default;

    [[nodiscard]] index_t local2global(index_t localIndex) const
    {
        assert(localIndex < this->N());
        return _local2global[localIndex];
    }

    [[nodiscard]] IndexList& local2global() { return _local2global; }

    [[nodiscard]] const IndexList& local2global() const
    {
        return _local2global;
    }

    // TODO: remove this
    [[nodiscard]] const IndexList local2global(IndexList localIndices) const
    {
        // TODO do we really want to perform the allocation here?
        IndexList globalIndices = IndexList(localIndices.size());
        for (index_t local_idx = 0; local_idx < localIndices.size();
             local_idx++)
            globalIndices[local_idx] =
              this->local2global(localIndices[local_idx]);

        return globalIndices;
    }

    [[nodiscard]] const IndexListList local2global(
      IndexListList localIndices) const
    {
        IndexListList globalIndices = IndexListList(localIndices.size());
        for (index_t local_idx(0); local_idx < localIndices.size(); local_idx++)
            globalIndices[local_idx] =
              this->local2global(localIndices[local_idx]);

        return globalIndices;
    }

    void shrink_to_fit()
    {
        Graph::shrink_to_fit();
        _local2global.shrink_to_fit();
    }

    bool operator==(const SubGraph& rhs) const
    {
        return (dynamic_cast<Graph const&>(rhs) ==
                dynamic_cast<Graph const&>(*this)) &&
               (this->_local2global == rhs._local2global);
    }

    static void write(const SubGraph& subgraph, const std::string& filename)
    {
        // we drop all info about local2global -> to read it as standalone graph
        // again, but keep this in mind.
        Graph::write(dynamic_cast<const Graph&>(subgraph), filename);
    }
    static bool has_no_double_edges(const SubGraph& graph)
    {
        if (graph.M() == 0)
            return true;

        for (index_t i = 0; i < graph.M() - 1; i++)
            for (index_t j = i + 1; j < graph.M(); j++) // TODO: only neigh
                if (graph.heads()[i] == graph.heads()[j] &&
                    graph.tails()[i] == graph.tails()[j])
                    return false;

        return true;
    }
    static bool has_no_self_loops(const SubGraph& graph)
    {
        if (graph.M() == 0)
            return true;

        for (index_t i = 0; i < graph.M() - 1; i++) {
            if (graph.heads()[i] == graph.tails()[i])
                return false;
        }
        return true;
    }

    static SubGraph extract_subgraph(const SubGraph& graph,
                                     const IndexList& nodes)
    {
        assert(std::is_sorted(nodes.begin(), nodes.end()));
        assert(std::all_of(nodes.begin(),
                           nodes.end(),
                           [graph](const index_t i) { return i < graph.N(); }));

        index_t N_l = nodes.size();

        // TODO: here we trade memory for runtime, right?, good trade?
        IndexList local_indices(graph.N());
        for (index_t i = 0; i < graph.N(); i++) {
            auto ptr = std::find(nodes.begin(), nodes.end(), i);
            local_indices[i] = std::distance(nodes.begin(), ptr);
            assert(local_indices[i] <= N_l);
        }

        IndexList tails_l; // TODO: reserve some appropiate size
        IndexList heads_l; // TODO: reserve some appropiate size
        for (index_t m = 0; m < graph.M(); m++) {
            auto tail = local_indices[graph.tails()[m]];
            auto head = local_indices[graph.heads()[m]];
            if ((tail < N_l) && (head < N_l)) {
                tails_l.push_back(tail);
                heads_l.push_back(head);
            }
        }

        IndexList local2global_l(N_l);
        for (index_t i = 0; i < N_l; i++)
            local2global_l[i] = graph.local2global(nodes[i]);

        return { N_l, tails_l, heads_l, local2global_l };
    }

    static bool is_undirected_graph(const SubGraph& graph)
    {
        if (graph.M() % 2 != 0)
            return false;

        index_t edges_visited = 0;
        for (index_t m = 0; m < graph.M(); m++) {
            const index_t& head = graph.heads()[m];
            const index_t& tail = graph.tails()[m];

            if (tail >= head)
                continue;

            const auto& outadj_j = graph.outadj()[head];
            if (std::find(outadj_j.begin(), outadj_j.end(), tail) ==
                outadj_j.end())
                return false;

            edges_visited++;
        }

        return (edges_visited * 2 == graph.M());
    }
};

std::ostream&
operator<<(std::ostream& os, const SubGraph& graph)
{
    os << dynamic_cast<const Graph&>(graph);
    os << "local2global=" << graph.local2global();
    return os;
}
