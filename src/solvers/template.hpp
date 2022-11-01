#pragma once

#include <cstdio>

#include "discrete/vc_funnel_handler.hpp"

// #define IN stdin
// #define OUT std::cout

// NOLINTBEGIN

#define IN argv[1]
#define OUT argv[1] + std::string(".sol")

#define EXPORT(x)                                                              \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") int main(         \
      int argc, char* argv[])                                                  \
    {                                                                          \
        auto graph = SubGraph(Graph::read(IN));                                \
        FVS fvs = x(graph);                                                    \
        FVS::write(OUT, fvs);                                                  \
        return 0;                                                              \
    }

#define EXPORT_VC(x)                                                           \
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"") int main(         \
      int argc, char* argv[])                                                  \
    {                                                                          \
        auto graph = UndirectedGraph::read(std::string(argv[1]));              \
        VC vc(graph.N(), false);                                               \
        x(graph, vc);                                                          \
        FVS::write(std::string(argv[1] + std::string(".sol")), vc);            \
        return 0;                                                              \
    }

// NOLINTEND