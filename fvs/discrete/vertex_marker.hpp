#pragma once

#include <sstream>
#include <vector>

namespace fvs {
class VertexMarker;

#include "undirected_graph.hpp"

class VertexMarker : public std::vector<bool>
{
    // TOOD: prob want to get a reference to the corresponding graph

  public:
    using vector = std::vector<bool>; // TODO
    using vector::vector;

    static void write(std::ostream& os, const VertexMarker& vec)
    {
        for (index_t i = 0; i < vec.size(); i++)
            if (vec[i])
                os << i + 1 << "\n";
    }

    static void write(const std::string& filename, const VertexMarker& vec)
    {
        std::ofstream out(filename);
        VertexMarker::write(out, vec);
        out.close();
    }

    // static VertexMarker read(const std::string& filename, const Graph& graph)
    static VertexMarker read(const std::string& filename, index_t size)
    {
        VertexMarker fvs(size, false);

        std::ifstream in(filename.c_str());
        if (!in) {
            std::cerr << "Error opening " << filename << std::endl;
            std::exit(-1);
        }

        std::string line;
        while (std::getline(in, line)) {
            std::stringstream ss(line);
            index_t u = 0;
            ss >> u;

            if (u > size) {
                std::cerr << "Invalid node in solution ( Node = " << u << " )"
                          << std::endl;
                std::exit(-1);
            }

            if (!ss.eof()) {
                std::cerr << "Line contains more than one node ( Line = '"
                          << line << "'" << std::endl;
                std::exit(-1);
            }

            if (fvs[u - 1]) {
                std::cerr << "Node " << u
                          << " is contained multiple times in solution"
                          << std::endl;
                std::exit(-1);
            }

            fvs[--u] = true;

            if (in.eof()) {
                break;
            }
        }

        return fvs;
    }
};
}